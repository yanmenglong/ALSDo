#include "tinfilter.h"
#include "global.h"
#include "tin.h"
#include "gridindex.h"
#include "workspace.h"
#include "seedgenerator.h"
#include "util.h"

#include <io.h>
#include <cmath>
#include <algorithm>
#include <string>

static void getPt2TriangleInfo(triangle *t, double *pt, double &angle, double &dis, double &min_edge_len) 
{
	//get equation of the plane (http://paulbourke.net/geometry/planeeq/)
	double A = 0.0, B = 0.0, C = 0.0, D = 0.0, t0 = 0.0, x0 = 0.0, y0 = 0.0, z0 = 0.0, tmp_dis[3], min_dis;
	A = t->V[0][1] * (t->V[1][2] - t->V[2][2]) + t->V[1][1] * (t->V[2][2] - t->V[0][2]) + t->V[2][1] * (t->V[0][2] - t->V[1][2]);
	B = t->V[0][2] * (t->V[1][0] - t->V[2][0]) + t->V[1][2] * (t->V[2][0] - t->V[0][0]) + t->V[2][2] * (t->V[0][0] - t->V[1][0]);
	C = t->V[0][0] * (t->V[1][1] - t->V[2][1]) + t->V[1][0] * (t->V[2][1] - t->V[0][1]) + t->V[2][0] * (t->V[0][1] - t->V[1][1]);
	D = -(t->V[0][0] * (t->V[1][1]*t->V[2][2] - t->V[2][1]*t->V[1][2]) + t->V[1][0] * (t->V[2][1]*t->V[0][2] - t->V[0][1]*t->V[2][2]) + t->V[2][0] * (t->V[0][1]*t->V[1][2] - t->V[1][1]*t->V[0][2]));

	//find projection of point(http://www.9math.com/book/projection-point-plane)
	t0 = -1.0 *(A*pt[0] + B*pt[1] + C*pt[2] + D)/(A*A + B*B + C*C);
	x0 = pt[0] + A*t0;
	y0 = pt[1] + B*t0;
	z0 = pt[2] + C*t0;

	dis = sqrt(pow(x0 - pt[0], 2.0) + pow(y0 - pt[1], 2.0) + pow(z0 - pt[2], 2.0));
	tmp_dis[0] = sqrt(pow(x0 - t->V[0][0], 2.0) + pow(y0 - t->V[0][1], 2.0) + pow(z0 - t->V[0][2], 2.0));
	tmp_dis[1] = sqrt(pow(x0 - t->V[1][0], 2.0) + pow(y0 - t->V[1][1], 2.0) + pow(z0 - t->V[1][2], 2.0));
	tmp_dis[2] = sqrt(pow(x0 - t->V[2][0], 2.0) + pow(y0 - t->V[2][1], 2.0) + pow(z0 - t->V[2][2], 2.0));
	min_dis = min(min(tmp_dis[0], tmp_dis[1]), tmp_dis[2]); 
	angle = atan(dis/min_dis);

	tmp_dis[0] = sqrt(pow(t->V[0][0] - t->V[1][0], 2.0) + pow(t->V[0][1] - t->V[1][1], 2.0) + pow(t->V[0][2] - t->V[1][2], 2.0));
	tmp_dis[1] = sqrt(pow(t->V[0][0] - t->V[2][0], 2.0) + pow(t->V[0][1] - t->V[2][1], 2.0) + pow(t->V[0][2] - t->V[2][2], 2.0));
	tmp_dis[2] = sqrt(pow(t->V[1][0] - t->V[2][0], 2.0) + pow(t->V[1][1] - t->V[2][1], 2.0) + pow(t->V[1][2] - t->V[2][2], 2.0));
	min_edge_len = min(min(tmp_dis[0], tmp_dis[1]), tmp_dis[2]);	
} 

TINFilter::TINFilter(WorkSpace *workSpace, const double &maxObjSize, const double &seedCellSize, const double &angleIter, const double &disIter)
{
	this->_workSpace = workSpace;
	this->_maxObjSize = maxObjSize;
	this->_seedCellSize = seedCellSize;
	this->_angleIter = angleIter;
	this->_disIter = disIter;
	this->_groundTag = NULL;

	this->_gridIndex = this->_workSpace->getGridIndex();
	this->_gridW = this->_gridIndex->getGridWidth();
	this->_gridH = this->_gridIndex->getGridHeight();	
}

TINFilter::~TINFilter()
{
	if (this->_groundTag != NULL)
	{
		delete []this->_groundTag;
		this->_groundTag = NULL;
	}
}

int TINFilter::filterGround(const double &filteringHeightTh, const double &filteringSlopeTh) 
{
	int count = 0;
	double hdiff = 0.0, angle = 0.0;

	if (this->_groundTag != NULL)
	{
		delete []this->_groundTag;
		this->_groundTag = NULL;
	}
	count = this->_gridH * this->_gridW;
	this->_groundTag = new bool[count];
	memset(this->_groundTag, 0, count * sizeof(bool));

	this->filterGrid();
	filterPtsByGrid(this->_workSpace, filteringHeightTh, filteringSlopeTh, this->_groundTag);
	return 0;
}

void TINFilter::filterGrid()
{
	int countN = 0, w = 0, h = 0, acceptN = 0, preAcceptN = 0, size = 0, ii = 0, jj = 0, pos = 0;
	double cellSize = 0.0, p[3], *C[4], offset = 10.0, angleT = 0.0, disT = 0.0, angle = 0.0, dis = 0.0, min_edge_len = 0.0, x = 0.0, y = 0.0;
	double minDis = MAXV, minZ = 0.0;
	triangle* t = NULL;
	PT3D *pt3d = NULL;
	vector<PT> seeds;
	PT seed;	
	list<PT3D*> left_points;

	cellSize = this->_gridIndex->getCellSize();

	// Find seeds
	SeedGenerator seedGenerator(this->_workSpace, this->_maxObjSize, this->_seedCellSize);
	seedGenerator.findMinSeeds(seeds);

	// Get the count of valid grid points
	countN = 0;
	for(h = 0; h < this->_gridH; ++h) 
	{
		for(w = 0; w < this->_gridW; ++w) 
		{
			pt3d = this->_gridIndex->getMinPT3D(w, h);
			if(pt3d == NULL) 
			{
				continue;
			}

			++countN;
		}
	}
	printf("original: %d\n", countN);

	offset = 10.0;
	double xC[4] = {-offset, this->_gridW*cellSize + offset,  this->_gridW*cellSize + offset, -offset};
	double yC[4] = {-offset, -offset, this->_gridH*cellSize + offset, this->_gridH*cellSize + offset};
	// add the edge points of grid as seeds
	TIN tin(countN);

	for(ii = 0; ii < 4; ++ii)
	{
		C[ii] = new double[3];
		C[ii][0] = xC[ii];
		C[ii][1] = yC[ii];
		
		minDis = MAXV;
		size = seeds.size();
		for (jj = 0; jj < size; ++jj)
		{
			seed = seeds[jj];
			pt3d = this->_gridIndex->getMinPT3D(seed.x, seed.y);
			dis = sqrt(pow(pt3d->x - C[ii][0], 2.0) + pow(pt3d->y - C[ii][1], 2.0));
			if(dis < minDis)
			{
				minDis = dis;
				minZ = pt3d->z;
			}
		}	
		C[ii][2] = minZ;
	}
	tin.initialize(C[0], C[1], C[2]);
	tin.add(C[3]);

	size = seeds.size();
	for (ii = 0; ii < size; ++ii)
	{
		seed = seeds[ii];
		pt3d = this->_gridIndex->getMinPT3D(seed.x, seed.y);
		p[0] = pt3d->x;
		p[1] = pt3d->y;
		p[2] = pt3d->z;
		tin.add(p);

		pos = seed.y * this->_gridW + seed.x;
		this->_groundTag[pos] = true;
		acceptN++;
	}
	printf("accept: %d\n", acceptN);

	// First start to accept some points, and put the left points into list
	angleT =  this->_angleIter;
	disT = this->_disIter;
	for(h = 0; h < this->_gridH; ++h) 
	{
		for(w = 0; w < this->_gridW; ++w) 
		{
			pos = h * this->_gridW + w;
			pt3d = this->_gridIndex->getMinPT3D(w, h);
			if(pt3d==NULL || this->_groundTag[pos])
			{
				continue;
			}

			pt3d->pos = pos;
			p[0] = pt3d->x;
			p[1] = pt3d->y;
			p[2] = pt3d->z;
			t = tin.locate(p);
			if (t==NULL || !t->V[0])
			{
				left_points.push_back(pt3d);
				continue;
			}

			getPt2TriangleInfo(t, p, angle, dis, min_edge_len);
			if (angle <= angleT && dis <= disT)
			{
				tin.add(p);
				this->_groundTag[pos] = true;
				acceptN++;
			}
			else
			{
				left_points.push_back(pt3d);
			}
		}
	}
	
	printf("accept: %d\n", acceptN);

	preAcceptN = -1;
	bool flag = 0;
	double angle_tmp = 0.0;
	double percent = 1.0;
	const int MAX_N = 4;
	int time_N = 0;
	double max_angle = 88.0*PI/180.0;

	while(percent > 0.001)
	{
		time_N++;
		preAcceptN = acceptN;
		
		if(angleT <= max_angle)
		{
			angleT +=  this->_angleIter;
			disT += this->_disIter;
		}

		angle_tmp = angleT*180.0/PI;
		printf("angleT: %lf ,disT: %lf\n", angle_tmp, disT);

		flag = false;
		for(list<PT3D*>::iterator it = left_points.begin(); it != left_points.end();)
		{
			pt3d = *it;
			p[0] = pt3d->x;
			p[1] = pt3d->y;
			p[2] = pt3d->z;
			t = tin.locate(p);
			if (t == NULL || !t->V[0])
			{
				++it;
				continue;
			}

			getPt2TriangleInfo(t, p, angle, dis, min_edge_len);
			if(min_edge_len < 5.0)
			{
				if(!flag)
				{
					this->_angleIter = this->_angleIter*0.8;
					this->_disIter = this->_disIter*0.8;
					flag = true;
				}
			}
			if (angle <= angleT && dis <= disT)
			{
				//if(min_edge_len >= 1.5)
				//{
					tin.add(p);
				//}
				
				this->_groundTag[pt3d->pos] = true;
				acceptN++;

				it = left_points.erase(it);
			}
			else
			{
				++it;
			}		
		}
		printf("accept in iteration: %d\n", acceptN);
		percent = abs(acceptN - preAcceptN)*1.0/preAcceptN;
		if(time_N + 1 >= MAX_N)
		{
			break;
		}
	}	

#if LOG_OUT
	size = tin.tinCount();
	string tin_path = std::string(this->_workSpace->getResFolder()) +"tin.tin";
	FILE *fp2 = fopen(tin_path.c_str(), "w+");
	for (ii = 0; ii < size; ii++, t++)
	{
		t = tin.triangleIndex(ii);

		if (t->next < 0) // if not deleted
		{
			if (t->V[0]) // if not infinite
			{
				x = t->V[0][0];
				y = t->V[0][1];
				w = int(x/cellSize);
				h = int(y/cellSize);
				fprintf(fp2, "%d %d\n", w, h);

				x = t->V[1][0];
				y = t->V[1][1];
				w = int(x/cellSize);
				h = int(y/cellSize);
				fprintf(fp2, "%d %d\n", w, h);

				x = t->V[2][0];
				y = t->V[2][1];
				w = int(x/cellSize);
				h = int(y/cellSize);
				fprintf(fp2, "%d %d\n", w, h);			
			}
		}
		fflush(fp2);
	}
	fclose(fp2);
#endif

	for(ii = 0; ii < 4; ++ii)
	{
		if(C[ii] != NULL)
		{
			delete C[ii];
			C[ii] = NULL;
		}
	}
}

