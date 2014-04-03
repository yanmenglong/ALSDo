#include "gridindex.h"
#include "workspace.h"

#include "gdal_priv.h"

#include <cmath>

/**
* Constructor
*/
GridIndex::GridIndex(WorkSpace *workSpace, const double &cellSize) 
{
	this->_workSpace = workSpace;
	this->_lasHeader = NULL;
	this->_cellSize = cellSize;
	this->_gridHeight = 0;
	this->_gridWidth = 0;
	this->_cellMinPts = NULL;
	this->_cellMaxPts = NULL;

	//create the grid index
	this->createIndex();
}

/**
* Deconstructor
*/
GridIndex::~GridIndex()
{	
	this->dispose();
}

/**
* Import the DTM to get the height of each point
*/
void GridIndex::importDTM(const string &dtmPath)
{
	GDALDataset *inDS = NULL;
	int rasterW = 0, rasterH = 0, w = 0, h = 0, pos1 = 0, iw = 0, ih = 0, pos2 = 0;
	float *rasterZ = NULL, rZ = 0.0;
	double geoTf[6];
	PT3D *pt3d = NULL;

	GDALAllRegister();
	inDS = (GDALDataset *)GDALOpen(dtmPath.c_str(), GA_ReadOnly);
	inDS->GetGeoTransform(geoTf);
	GDALRasterBand *poBand = inDS->GetRasterBand(1);
	rasterW = poBand->GetXSize();
	rasterH = poBand->GetYSize();
	rasterZ = new float[rasterW * rasterH];
	poBand->RasterIO(GF_Read, 0, 0, rasterW, rasterH, rasterZ, rasterW, rasterH, GDT_Float32, 0, 0); 	
	GDALClose(inDS);

	//double geoTf[6] = { lasHeader->xMin, dtmFactor, 0.0, lasHeader->yMin, 0.0, dtmFactor};
	// The grid index and the raster dtm should have the same cell size
	for(h = 0; h < this->_gridHeight; ++h)
	{
		for(w = 0; w < this->_gridWidth; ++w)
		{
			pos1 = h * this->_gridWidth + w;
			pt3d = &this->_cellMinPts[pos1];
			iw = int((pt3d->x + this->_lasHeader->xMin - geoTf[0])/geoTf[1]);
			ih = int((pt3d->y + this->_lasHeader->yMin - geoTf[3])/geoTf[5]);
			pos2 = ih * rasterW + iw;
			rZ = rasterZ[pos2];
			pt3d->z = pt3d->z - rZ;

			pt3d = &this->_cellMaxPts[pos1];
			iw = int((pt3d->x + this->_lasHeader->xMin - geoTf[0])/geoTf[1]);
			ih = int((pt3d->y + this->_lasHeader->yMin - geoTf[3])/geoTf[5]);
			pos2 = ih * rasterW + iw;
			rZ = rasterZ[pos2];
			pt3d->z = pt3d->z - rZ;
		}
	}

	if(rasterZ != NULL)
	{
		delete []rasterZ;
		rasterZ = NULL;
	}
}

/**
* Get minimum PT3D value of cell at given (ix, iy)
*/
PT3D* GridIndex::getMinPT3D(const int &ix, const int &iy) 
{
	PT3D *pt3d = NULL;
	int pos = 0;
	if(ix >=0 && ix < this->_gridWidth && iy >= 0 && iy < this->_gridHeight)
	{
		pos = iy * this->_gridWidth + ix;
		pt3d = &this->_cellMinPts[pos];
		if(pt3d->type == PointType::p_created)
		{
			pt3d = NULL;
		}
	}
	return pt3d;
}

/**
* Get maximum PT3D value of cell at given (ix, iy)
*/
PT3D* GridIndex::getMaxPT3D(const int &ix, const int &iy) 
{
	PT3D *pt3d = NULL;
	int pos = 0;
	if(ix >=0 && ix < this->_gridWidth && iy >= 0 && iy < this->_gridHeight)
	{
		pos = iy * this->_gridWidth + ix;
		pt3d = &this->_cellMaxPts[pos];
		if(pt3d->type == PointType::p_created)
		{
			pt3d = NULL;
		}
	}
	return pt3d;
}

/**
* Get 4 neighborhood PTs at given (ix, iy)
*/
void GridIndex::get4NeighPTs(const int &ix, const int &iy, PT nhbor[4])
{
	/** 
	* 4 neighbor PTs 
	*    1                
	* 0 pt 2
	*   3
	*/
	nhbor[0].x = ix-1;
	nhbor[0].y = iy;
	nhbor[1].x = ix;
	nhbor[1].y = iy-1;
	nhbor[2].x = ix+1;
	nhbor[2].y = iy;
	nhbor[3].x = ix;
	nhbor[3].y = iy+1;
}

/**
* Get 8 neighborhood PTs at given (ix, iy)
*/
void GridIndex::get8NeighPTs(const int &ix, const int &iy, PT nhbor[8])
{
	/**
	* 8 neighbor PTs
	* 1  2  3               
	* 0 pt  4
	* 7  6  5
	*/
	nhbor[0].x = ix-1;
	nhbor[0].y = iy;
	nhbor[1].x = ix-1;
	nhbor[1].y = iy-1;
	nhbor[2].x = ix;
	nhbor[2].y = iy-1;
	nhbor[3].x = ix+1;
	nhbor[3].y = iy-1;
	nhbor[4].x = ix+1;
	nhbor[4].y = iy;
	nhbor[5].x = ix+1;
	nhbor[5].y = iy+1;
	nhbor[6].x = ix;
	nhbor[6].y = iy+1;
	nhbor[7].x = ix-1;
	nhbor[7].y = iy+1;
}

/**
* Meta information of original las file
*/
LASHeader* GridIndex::getLASHeader()
{
	return this->_lasHeader;
}

double GridIndex::getCellSize()
{
	return this->_cellSize; 
}

int GridIndex::getGridHeight() 
{
	return this->_gridHeight;
}

int GridIndex::getGridWidth() 
{
	return this->_gridWidth;
}

/**
* Create the grid index
*/
void GridIndex::createIndex()
{
	LasIO *lasIO = NULL;
	LASHeader *lasHeader = NULL;
	LASPoint *lasPts = NULL, *lasPt = NULL;
	int count = 0, ptN = 0, ii = 0, hh = 0, ww = 0, pos = 0;
	bucket *bs = NULL;
	std::deque<int>::iterator iter;

	//first dispose the objects that should be NULL
	this->dispose();
	lasIO = new LasIO(string(this->_workSpace->getLasPath()));
	lasHeader = lasIO->open();
	if(lasHeader == NULL)
	{
		return;
	}
	this->_lasHeader = new LASHeader;
	memcpy(this->_lasHeader, lasHeader, sizeof(LASHeader));
	this->_gridWidth = (int)((this->_lasHeader->xMax - this->_lasHeader->xMin) / this->_cellSize) + 1;
	this->_gridHeight = (int)((this->_lasHeader->yMax - this->_lasHeader->yMin) / this->_cellSize) + 1;	
	count = this->_gridHeight * this->_gridWidth;

	this->_cellMinPts = new PT3D[count];
	if(this->_cellMinPts == NULL)
	{
		return;
	}
	this->_cellMaxPts = new PT3D[count];
	if(this->_cellMaxPts == NULL)
	{
		return;
	}

	lasPts = lasIO->readAllPts();
	ptN = lasHeader->records_number;
	for(ii = 0; ii < ptN; ++ii)
	{
		lasPts[ii].class_id = PointType::p_unclassified;
	} 

	bs = new bucket[count];
	if(bs == NULL)
	{
		return;
	}

	// Put the las points 2 
	this->putLASPt2GridCell(lasHeader, lasPts, ptN, bs);
	// Fill cellMinPts and cellMaxPts
	for(hh = 0; hh < this->_gridHeight; ++hh)
	{
		for(ww = 0; ww < this->_gridWidth; ++ww)
		{
			pos = hh*this->_gridWidth + ww;
			this->_cellMinPts[pos].pos = pos;
			this->_cellMinPts[pos].x = NAN;
			this->_cellMinPts[pos].y = NAN;
			this->_cellMinPts[pos].z = MAXV;
			this->_cellMinPts[pos].type = PointType::p_created;
			this->_cellMaxPts[pos].pos = pos;
			this->_cellMaxPts[pos].x = NAN;
			this->_cellMaxPts[pos].y = NAN;
			this->_cellMaxPts[pos].z = MINV;
			this->_cellMaxPts[pos].type = PointType::p_created;
			for(iter = bs[pos].v.begin(); iter != bs[pos].v.end(); ++iter)
			{
				lasPt = &lasPts[*iter];
				if(lasPt->z < this->_cellMinPts[pos].z)
				{
					this->_cellMinPts[pos].x = lasPt->x - this->_lasHeader->xMin;
					this->_cellMinPts[pos].y = lasPt->y - this->_lasHeader->yMin;
					this->_cellMinPts[pos].z = lasPt->z;
					this->_cellMinPts[pos].type = PointType::p_unclassified;
				}
				if(lasPt->z > this->_cellMaxPts[pos].z)
				{
					this->_cellMaxPts[pos].x = lasPt->x - this->_lasHeader->xMin;
					this->_cellMaxPts[pos].y = lasPt->y - this->_lasHeader->yMin;
					this->_cellMaxPts[pos].z = lasPt->z;
					this->_cellMaxPts[pos].type = PointType::p_unclassified;
				}				
			}
		}
	}

	// Remove high points and low points
	// removeHighLowPoints(lasPts, bs, 20.0);

	if(lasIO != NULL)
	{
		delete lasIO;
		lasIO = NULL;
	}
	if(lasPts != NULL)
	{
		delete []lasPts;
		lasPts = NULL;
	}
	if(bs != NULL)
	{
		for(ii = 0; ii < count; ++ii)
		{
			bs[ii].v.clear();
		}
		delete []bs;
		bs = NULL;
	}
}

/**
* Dispose the garbage and free memory
*/
void GridIndex::dispose()
{
	if (this->_lasHeader != NULL)
	{
		delete this->_lasHeader;
		this->_lasHeader = NULL;
	}
	if (this->_cellMinPts != NULL)
	{
		delete []this->_cellMinPts;
		this->_cellMinPts = NULL;
	}
	if(this->_cellMaxPts != NULL)
	{
		delete []this->_cellMaxPts;
		this->_cellMaxPts = NULL;
	}
}

/**
* Put all points in a bucket buffer to create the grid index
*/
void GridIndex::putLASPt2GridCell(LASHeader *lasHeader, LASPoint *lasPts, const int &ptN, bucket *bs)
{
	int ii = 0, ix = 0, iy = 0, pos = 0;
	unsigned char type = 0;

	for(ii = 0; ii < ptN; ++ii)
	{
		type = lasPts[ii].class_id;
		if (type == PointType::p_created || type == PointType::p_high_point || type == PointType::p_low_point)
		{
			continue;
		}

		ix = (int)((lasPts[ii].x - lasHeader->xMin) / this->_cellSize);
		iy = (int)((lasPts[ii].y - lasHeader->yMin) / this->_cellSize);
		pos = iy * this->_gridWidth + ix;
		bs[pos].v.push_back(ii);
	}
}

/**
* Remove high points and low points and save the valid points to result_folder+'/valid.las'
*/
void GridIndex::removeHighLowPoints(LASPoint *lasPts, bucket *bs, const double &zTh)
{
	int gridW = 0, gridH = 0, cellSize = 0, w = 0, h = 0, ii = 0, pos = 0;
	PT nhbor[8];
	PT3D *pt3d = NULL;
	double nZMin = MAXV, nZMax = MINV;
	std::deque<int>::iterator iter;	
	LASPoint *lasPt = NULL;
	LasIO *outLasIO = NULL;
	string outLasPath;

	for(h = 0; h < this->_gridHeight; ++h)
	{
		for(w = 0; w < this->_gridWidth; ++w)
		{
			pos = h * this->_gridWidth + w;
			if(bs[pos].v.size() == 0)
			{
				continue;
			}

			nZMin = MAXV;
			nZMax = MINV;
			this->get8NeighPTs(w, h, nhbor);
			for(ii = 0; ii < 8; ++ii)
			{
				pt3d = this->getMinPT3D(nhbor[ii].x, nhbor[ii].y);
				if(pt3d == NULL)
				{
					continue;
				}
				if(pt3d->z < nZMin)
				{
					nZMin = pt3d->z;
				}
				pt3d = this->getMaxPT3D(nhbor[ii].x, nhbor[ii].y);
				if(pt3d->z > nZMax)
				{
					nZMax = pt3d->z;
				}
			}

			for(iter = bs[pos].v.begin(); iter != bs[pos].v.end(); ++iter)
			{
				lasPt = &lasPts[*iter];
				lasPt->class_id = PointType::p_unclassified;
				if(nZMin - lasPt->z > zTh)
				{
					lasPt->class_id = PointType::p_low_point;
				}
				if(lasPt->z - nZMax > zTh)
				{
					lasPt->class_id = PointType::p_high_point;
				}
			}
		}
	}

	outLasPath = string(this->_workSpace->getResFolder()) + "original.las";
	outLasIO = new LasIO(outLasPath);
	outLasIO->open(true);
	outLasIO->writePts(lasPts, this->_lasHeader->records_number, PointType::p_unclassified);	
	outLasIO->close();

	if(outLasIO != NULL)
	{
		delete outLasIO;
		outLasIO = NULL;
	}	
}

