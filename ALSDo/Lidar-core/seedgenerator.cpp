#include "seedgenerator.h"
#include "workspace.h"
#include "gridindex.h"

#include <io.h>

SeedGenerator::SeedGenerator(WorkSpace *workSpace, const double &maxObjSize, const double &seedCellSize)
{
	this->_workSpace = workSpace;
	this->_maxObjSize = maxObjSize;
	this->_seedCellSize = seedCellSize;
	this->_gridIndex = this->_workSpace->getGridIndex();
	this->_gridW = this->_gridIndex->getGridWidth();
	this->_gridH = this->_gridIndex->getGridHeight();

	if(this->_seedCellSize > this->_maxObjSize)
	{
		this->_seedCellSize = this->_maxObjSize;
	}
}


SeedGenerator::~SeedGenerator()
{
}

/**
* Find seeds of local minimum z-value by grid min stand point, and if the 'special_region.rect' file exist in the result folder, the window size to 
* search seeds will be minimized.
*
* seeds: seeds collection
*/
void SeedGenerator::findMinSeeds(std::vector<PT> &seeds)
{
	std::string specialRegionPath;
	FILE *fp = NULL;
	int regionSize = 0, regionWN = 0, regionHN = 0, x = 0, y = 0, gw = 0, gh = 0, wL = 0, wH = 0, hL = 0, hH = 0, h = 0, w = 0, pos = 0;
	bool *specialRegionFlag = NULL;
	double maxObjSize, zMin = MAXV;
	PT3D *pt3d = NULL;
	PT pt;

	regionSize = this->_maxObjSize / this->_gridIndex->getCellSize();
	regionWN = int(this->_gridW / regionSize) + 1;
	regionHN = int(this->_gridH / regionSize) + 1;
	this->_seedCellSize = this->_seedCellSize / this->_gridIndex->getCellSize();
	if(this->_seedCellSize < 5.0)
	{
		this->_seedCellSize = 5.0;
	}

	specialRegionFlag = new bool[regionHN * regionWN];
	memset(specialRegionFlag, 0, regionHN * regionWN * sizeof(bool));
	specialRegionPath = std::string(this->_workSpace->getResFolder()) + std::string("special_region.rect"); 

	if((_access(specialRegionPath.c_str(), 0)) != -1)
	{
		fp = fopen(specialRegionPath.c_str(), "r");
		//first values is the size of max object in the given region 
		fscanf(fp, "%lf\n", &maxObjSize);

		while (fscanf(fp, "%d %d\n", &x, &y) != EOF)
		{
			pos = y * regionWN + x;
			specialRegionFlag[pos] = true;
		}		
		fclose(fp);
		fp = NULL;
	}
	
	// Find seeds in grid
	for(gh = 0; gh < regionHN; ++gh)
	{
		for(gw = 0; gw < regionWN; ++gw)
		{
			hL = gh * regionSize;
			hH = (gh + 1) * regionSize;
			wL = gw * regionSize;
			wH = (gw + 1) * regionSize;

			pos = gh * regionWN + gw;
			if(specialRegionFlag[pos])
			{
				this->findMinSeedsInCell(wL, wH, hL, hH, seeds);
				continue;
			}
						
			zMin = MAXV;
			pt.x = -1;
			pt.y = -1;
			for(h = hL; h < hH; ++h)
			{
				for(w = wL; w < wH; ++w)
				{
					pt3d = this->_gridIndex->getMinPT3D(w, h);
					if(pt3d == NULL)
					{
						continue;
					}

					if(pt3d->z < zMin)
					{
						zMin = pt3d->z;
						pt.x = w;
						pt.y = h;
					}
				}
			}
			
			if(pt.x != -1)
			{
				seeds.push_back(pt);
			}
		}
	}

	// Take the four corners as seeds
	this->findMinSeedsAtCorners(seeds);

	if(specialRegionFlag != NULL)
	{
		delete []specialRegionFlag;
		specialRegionFlag = NULL;
	}

#if LOG_OUT
	std::string seed_path = std::string(this->_workSpace->getResFolder()) + std::string("seeds.pt"); 
	fp = fopen(seed_path.c_str(), "w+");
	for (vector<PT>::iterator it = seeds.begin(); it != seeds.end(); ++it)
	{
		fprintf(fp, "%d %d\n", it->x, it->y);
	}
	fflush(fp);
	fclose(fp);
	fp = NULL;
#endif
}

/**
* Find seeds of local minimum z-value by grid max stand point
*
* seeds: seeds collection
*/
void SeedGenerator::findMaxSeeds(std::vector<PT> &seeds)
{
	int regionSize = 0, regionWN = 0, regionHN = 0, gw = 0, gh = 0, wL = 0, wH = 0, hL = 0, hH = 0, h = 0, w = 0, pos = 0;
	double zMax = MINV;
	PT3D *pt3d = NULL;
	PT pt;

	regionSize = this->_maxObjSize / this->_gridIndex->getCellSize();
	regionWN = int(this->_gridW / regionSize) + 1;
	regionHN = int(this->_gridH / regionSize) + 1;

	for(gh = 0; gh < regionHN; ++gh)
	{
		for(gw = 0; gw < regionWN; ++gw)
		{
			hL = gh * regionSize;
			hH = (gh + 1) * regionSize;
			wL = gw * regionSize;
			wH = (gw + 1) * regionSize;

			pos = gh * regionWN + gw;
			zMax = MINV;
			pt.x = -1;
			pt.y = -1;
			for(h = hL; h < hH; ++h)
			{
				for(w = wL; w < wH; ++w)
				{
					pt3d = this->_gridIndex->getMaxPT3D(w, h);
					if(pt3d == NULL)
					{
						continue;
					}

					if(pt3d->z > zMax)
					{
						zMax = pt3d->z;
						pt.x = w;
						pt.y = h;
					}
				}
			}
			
			if(pt.x != -1)
			{
				seeds.push_back(pt);
			}
		}
	}

#if LOG_OUT
	std::string seed_path = std::string(this->_workSpace->getResFolder()) + "seeds.pt"; 
	FILE *fp = fopen(seed_path.c_str(), "w+");
	for (vector<PT>::iterator it = seeds.begin(); it != seeds.end(); ++it)
	{
		fprintf(fp, "%d %d\n", it->x, it->y);
	}
	fflush(fp);
	fclose(fp);
#endif
}

/**
* Find seeds in smaller size of minimum z-value
*
* wL: lower x of bigger window
* wH: higher x of bigger window
* hL: lower y of bigger window
* hH: higher y of bigger window
* seeds: seeds collection
*/
void SeedGenerator::findMinSeedsInCell(const int &wL, const int &wH, const int &hL, const int &hH, std::vector<PT> &seeds)
{
	int gw = 0, gh = 0, cwL = 0, cwH = 0, chL = 0, chH = 0, ch = 0, cw = 0;
	PT3D *pt3d = NULL;
	double zMin = MAXV;
	PT pt;

	for(gh = hL; gh < hH; gh += this->_seedCellSize)
	{
		for(gw = wL; gw < wH; gw += this->_seedCellSize)
		{
			chL = gh;
			chH = gh + this->_seedCellSize;
			cwL = gw;
			cwH = gw + this->_seedCellSize;

			zMin = MAXV;
			pt.x = -1;
			pt.y = -1;
			for(ch = chL; ch < chH; ++ch)
			{
				for(cw = cwL; cw < cwH; ++cw)
				{
					pt3d = this->_gridIndex->getMinPT3D(cw, ch);
					if(pt3d == NULL)
					{
						continue;
					}

					if(pt3d->z < zMin)
					{
						zMin = pt3d->z;
						pt.x = cw;
						pt.y = ch;
					}
				}
			}
			if(pt.x != -1)
			{
				seeds.push_back(pt);
			}			
		}
	}
}

/**
* Find seeds at four corners. It assumes that the four corners should be consider as seed pointsners
*
* seeds: seeds collection
*/
void SeedGenerator::findMinSeedsAtCorners(std::vector<PT> &seeds)
{
	LASHeader *lasHeader = NULL;
	PT3D *pt3d = NULL, cornerPt3d[4];
	double minDis[4], tmpDis = 0.0;
	int w = 0, h = 0, ii = 0;
	PT cornerPt[4];

	lasHeader = this->_gridIndex->getLASHeader();
	cornerPt3d[0].x = 0;
	cornerPt3d[0].y = 0;
	cornerPt3d[1].x = lasHeader->xMax - lasHeader->xMin;
	cornerPt3d[1].y = 0;
	cornerPt3d[2].x = lasHeader->xMax - lasHeader->xMin;
	cornerPt3d[2].y = lasHeader->yMax - lasHeader->yMin;
	cornerPt3d[3].x = 0;
	cornerPt3d[3].y = lasHeader->yMax - lasHeader->yMin;
	minDis[0] = minDis[1] = minDis[2] = minDis[3] = MAXV;

	for(h = 0; h < this->_gridH; ++h)
	{
		for(w = 0; w < this->_gridW; ++w)
		{
			pt3d = this->_gridIndex->getMaxPT3D(w, h);
			if(pt3d == NULL)
			{
				continue;
			}

			for(ii = 0; ii < 4; ++ii)
			{
				tmpDis = sqrt(pow(pt3d->x - cornerPt3d[ii].x, 2.0) + pow(pt3d->y - cornerPt3d[ii].y, 2.0));
				if(tmpDis < minDis[ii])
				{
					minDis[ii] = tmpDis;
					cornerPt[ii].x = w;
					cornerPt[ii].y = h;
				}
			}
		}
	}

	for(ii = 0; ii < 4; ++ii)
	{
		seeds.push_back(cornerPt[ii]);
	}
}