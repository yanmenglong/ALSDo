#include "gridfilter.h"
#include "workspace.h"
#include "gridindex.h"
#include "util.h"
#include "seedgenerator.h"
#include "matrix.h"

#include <list>
using namespace std;

GridFilter::GridFilter(WorkSpace *workSpace)
{
	this->_workSpace = workSpace;

	this->_gridIndex = this->_workSpace->getGridIndex();
	this->_gridW = this->_gridIndex->getGridWidth();
	this->_gridH = this->_gridIndex->getGridHeight();

	this->_groundTag = NULL;
}


GridFilter::~GridFilter()
{
}

/**
* Do ssed expand with given threshold
* maxObjSize: maximum size of object in given scence
* seedCellSize: smaller window size for searching seeds
* heightTh: height threshold for seed expand
* slopeTh: slope threshold for seed expand
*/
void GridFilter::seedExpand(const double &maxObjSize, const double &seedCellSize, const double &heightTh, 
	const double &slopeTh)
{
	int count = 0, pos = 0;
	bool *enter_queue = NULL;
	vector<PT> groundSeeds;
	vector<PT>::iterator ptIter;
	PT pt;

	// First dispose garbage and allocate memory
	this->dispose();
	count = this->_gridH * this->_gridW;
	this->_groundTag = new bool[count];
	memset(this->_groundTag, 0, count*sizeof(bool));
	enter_queue = new bool[count];
	memset(enter_queue, 0, count*sizeof(bool));

	// Find ground seeds
	calcComplexByLocalEdge(this->_workSpace, 1.0, maxObjSize);
	SeedGenerator seedGenerator(this->_workSpace, maxObjSize, seedCellSize);
	seedGenerator.findMinSeeds(groundSeeds);

	// Expand from ground seeds
	for (ptIter = groundSeeds.begin(); ptIter != groundSeeds.end(); ++ptIter)
	{
		pt = *ptIter;
		pos = pt.y * this->_gridW + pt.x;
		if(this->_groundTag[pos]) 
		{
			continue;
		}

		this->_queue.push_back(pt);
		enter_queue[pos] = true;

		while(!this->isQueueEmpty()) 
		{
			pt = this->popQueue();
			pos = pt.y * this->_gridW + pt.x;
			enter_queue[pos] = false;

			this->_groundTag[pos] = true;
			this->enterQueue(pt, enter_queue, heightTh, slopeTh);
		}
	}

	if(enter_queue != NULL) 
	{
		delete []enter_queue;
		enter_queue = NULL;
	}
}

// Fileter points
void GridFilter::filterPoints(const double &filteringHeightTh, const double &filteringSlopeTh, const double &dtmFactor,
	const double &objectHT)
{
	// Filter the gird twice, first get the initialize surface and then calculate the surface again	
	int pos = 0, h = 0, w = 0;
	float *rasterZ = NULL, hTH = 0.5f;
	PT3D *pt3d = NULL;

	// Filter the whole region with original LAS file
	filterPtsByGrid(this->_workSpace, filteringHeightTh, filteringSlopeTh, this->_groundTag);
	
	// Interpolate the rasterZ
	rasterZ = new float[this->_gridH * this->_gridW];
	memset(rasterZ, 0, this->_gridH * this->_gridW * sizeof(float));
	las2RasterZ(this->_workSpace, this->_gridIndex->getCellSize(), this->_gridW, this->_gridH, rasterZ);

	//
	for(h = 0; h < this->_gridH; ++h)
	{
		for(w = 0; w < this->_gridW; ++w)
		{
			pos = h * this->_gridW + w;
			if(this->_groundTag[pos] || rasterZ[pos] == NAN)
			{
				continue;
			}

			pt3d = this->_gridIndex->getMinPT3D(w, h);
			if(pt3d == NULL)
			{
				continue;
			}			
			
			if(pt3d->z - rasterZ[pos] <= hTH)
			{
				this->_groundTag[pos] = true;
			}
		}
	}

	// Filter the whole region with original LAS file
	filterPtsByGrid(this->_workSpace, filteringHeightTh, filteringSlopeTh, this->_groundTag);

	// Interpolate DEM and extract object points
	las2Raster(this->_workSpace, dtmFactor);
	extractObjectPoints(this->_workSpace, dtmFactor, objectHT);
}

// Fileter the grid
void GridFilter::filterGrid()
{
	
}

/**
* Put the neighborhood points of given pt into queue for segmentation
*
* pt: current PT
* type: current type of entity
* enterQueueFlag: check whether the point is already in queue
* hT: height threshold to check whether the neighborhood PTs of pt should be put into the queue
* angleT: angle threshold to check whether the neighborhood PTs of pt should be put into the queue
*/
void GridFilter::enterQueue(PT& pt, bool *enterQueueFlag, const double &heightTh, const double &slopeTh) 
{
	PT nhbor[4];
	int ii = 0, pos = 0;
	double hdiff = 0.0, angle = 0.0;
	PT3D *pt3d1 = NULL, *pt3d2 = NULL;
	
	pt3d1 = this->_gridIndex->getMinPT3D(pt.x, pt.y);
	this->_gridIndex->get4NeighPTs(pt.x, pt.y, nhbor);
	for(ii = 0; ii < 4; ++ii) 
	{
		/* If the (x,y) ut of the range,ignore it */
		if(nhbor[ii].x < 0 || nhbor[ii].x >= this->_gridW || nhbor[ii].y < 0 || nhbor[ii].y >= this->_gridH)
		{
			continue;
		}
		pos = nhbor[ii].y * this->_gridW + nhbor[ii].x;
		/* If the point already enters the queue,ignore it */
		if(enterQueueFlag[pos]) 
		{
			continue;
		}
		if(this->_groundTag[pos])
		{
			continue;		
		}

		pt3d2 = this->_gridIndex->getMinPT3D(nhbor[ii].x, nhbor[ii].y);
		// If given local minimum index of point does not exist, ignore it
		if(pt3d2 == NULL)  
		{
			continue;
		}
			
		getHeightDiffandAngleDiff(*pt3d1, *pt3d2, hdiff, angle);
		if(hdiff > heightTh || angle > slopeTh)
		{
			continue;
		}
			
		this->_queue.push_back(nhbor[ii]);
		enterQueueFlag[pos] = true;
	}	
}

/**
* Pop a PT from the queue
*/
PT GridFilter::popQueue() 
{
	PT pt = this->_queue.front();
	this->_queue.pop_front();
	return pt;
}

/**
* Check whether the queue is empty
*/
bool GridFilter::isQueueEmpty() 
{
	return this->_queue.empty();
}

void GridFilter::dispose()
{
	if(this->_groundTag != NULL)
	{
		delete []this->_groundTag;
		this->_groundTag = NULL;
	}
}
