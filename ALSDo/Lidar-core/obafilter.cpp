#include "obafilter.h"
#include "matrix.h"
#include "workspace.h"
#include "seedgenerator.h"
#include "util.h"

#include <cmath>
#include <algorithm>
#include <io.h>

OBAFilter::OBAFilter(WorkSpace *workSpace) 
	: OBASegment(workSpace)
{
}

OBAFilter::~OBAFilter()
{
	this->dispose();
}

/**
* First search seeds by OBA strategy, then egment region by seeds with local minimum z-value
*/
void OBAFilter::segmentRegionBySeeds(const double &seedHeightTh, const double &seedSlopeTh, const double &maxObjSize,
	const double &seedCellSize, const double &segHeightTh, const double &segSlopeTh)
{
	int ii = 0, count = 0, iy = 0, ix = 0, pos = 0, id = 0, entityID = 0;
	double ptZ = 0.0;
	bool *enter_queue = NULL;
	vector<PT> groundSeeds;
	vector<PT>::iterator ptIter;
	PT pt;
	PointType type;
	Entity* entity = NULL;
	PT3D *pt3d;

	// First dispose garbage
	this->dispose();
	count = this->_gridH * this->_gridW;
	this->_entityIDs = new int[count];
	for(ii = 0; ii < count; ++ii)
	{
		this->_entityIDs[ii] = NAN;
	}
	enter_queue = new bool[count];
	memset(enter_queue, 0, count * sizeof(bool));

	// Find ground seeds, and segment from ground seeds
	calcComplexByObj(this->_workSpace, maxObjSize, seedHeightTh, seedSlopeTh);
	SeedGenerator seedGenerator(this->_workSpace, maxObjSize, seedCellSize);
	seedGenerator.findMinSeeds(groundSeeds);
	for (ptIter = groundSeeds.begin(); ptIter != groundSeeds.end(); ++ptIter)
	{
		pt = *ptIter;
		pos = pt.y * this->_gridW + pt.x;
		if(this->_entityIDs[pos] != NAN) 
		{
			continue;
		}
		type = PointType::p_ground;
		entityID++;
		entity = new Entity(entityID, type);
		this->_queue.push_back(pt);
		enter_queue[pos] = true;

		while(!this->isQueueEmpty()) 
		{
			pt = this->popQueue();
			pos = pt.y * this->_gridW + pt.x;
			enter_queue[pos] = false;

			this->_entityIDs[pos] = entityID;
			pt3d = this->_gridIndex->getMinPT3D(pt.x, pt.y);
			entity->addPoint(pt, pt3d->z);

			this->enterQueue(pt, type, enter_queue, segHeightTh, segSlopeTh);
		}
		this->_entitys[entityID] = entity;
	}

	// Segment for the left points 
	for(iy = 0; iy < this->_gridH; ++iy)
	{
		for(ix = 0; ix < this->_gridW; ++ix) 
		{
			pos = iy * this->_gridW + ix;
			if(this->_entityIDs[pos] != NAN) 
			{
				continue;
			}

			// Set an object as an unclassified object if it is not a hole
			type = PointType::p_unclassified;
			pt3d = this->_gridIndex->getMinPT3D(ix, iy);
			if(pt3d == NULL) 
			{
				// Set an object as an created object if it is a hole
				type = PointType::p_created;
			}

			entityID++;
			entity = new Entity(entityID, type);
			pt.x = ix;
			pt.y = iy;
			this->_queue.push_back(pt);
			enter_queue[pos] = true;
			
			// When the queue is empty, a new object is derived
			while(!this->isQueueEmpty()) 
			{
				pt = this->popQueue();
				pos = pt.y * this->_gridW + pt.x;
				enter_queue[pos] = false;

				pt3d = this->_gridIndex->getMinPT3D(pt.x, pt.y);
				if(pt3d == NULL) 
				{
					ptZ = 0.0;
				}
				else
				{
					ptZ = pt3d->z;
				}

				this->_entityIDs[pos] = entityID;
				entity->addPoint(pt, ptZ);
				this->enterQueue(pt, type, enter_queue, segHeightTh, segSlopeTh);
			}
			this->_entitys[entityID] = entity;
		}
	}

	if(enter_queue != NULL) 
	{
		delete []enter_queue;
		enter_queue = NULL;
	}
}

// Filetering method 
void OBAFilter::filterRegion(const double &filteringHeightTh, const double &filteringSlopeTh, const double &dtmFactor, const double &objectHT)
{
	bool *groundTag = NULL;
	int pos1 = 0, pos2 = 0, h = 0, w = 0;
	Entity *entity = NULL;
	std::vector<PT> pts;
	std::vector<PT>::iterator ptIter;
	float *rasterZ = NULL;
	PT3D *pt3d = NULL;
	
	groundTag = new bool[this->_gridH * this->_gridW];
	memset(groundTag, 0, this->_gridH * this->_gridW * sizeof(bool));
	
	this->filterEntitys();
	for(hash_map<int, Entity*>::iterator it = this->_entitys.begin(); it != this->_entitys.end(); ++it) 
	{
		entity = it->second;
		// Use the type to identify 
		if (entity->getType() == PointType::p_ground)
		{
			pts = entity->getAllPts();
			for(ptIter = pts.begin(); ptIter != pts.end(); ++ptIter)
			{
				pos1 = ptIter->y * this->_gridW + ptIter->x;
				groundTag[pos1] = true;
			}
		}
	}
	// Filter the whole region with original LAS file
	filterPtsByGrid(this->_workSpace, filteringHeightTh, filteringSlopeTh, groundTag);

	// Interpolate the rasterZ
	rasterZ = new float[this->_gridH * this->_gridW];
	memset(rasterZ, 0, this->_gridH * this->_gridW * sizeof(float));
	las2RasterZ(this->_workSpace, this->_gridIndex->getCellSize(), this->_gridW, this->_gridH, rasterZ);

	for(h = 0; h < this->_gridH; ++h)
	{
		for(w = 0; w < this->_gridW; ++w)
		{
			pos1 = h * this->_gridW + w;
			pos2 = (this->_gridH - 1 - h) * this->_gridW + w;
			groundTag[pos1] = false;
			if(rasterZ[pos2] == NAN)
			{
				continue;
			}

			pt3d = this->_gridIndex->getMinPT3D(w, h);
			if(pt3d == NULL)
			{
				continue;
			}			
			else 
			{
				if(pt3d->z - rasterZ[pos2] <= 0.5)
				{
					groundTag[pos1] = true;

					entity = this->_entitys[this->_entityIDs[pos1]];
					entity->setType(PointType::p_ground);
				}
			}
		}
	}

	// Filter the whole region with original LAS file
	filterPtsByGrid(this->_workSpace, filteringHeightTh, filteringSlopeTh, groundTag);

	// Interpolate DEM and extract object points
	las2Raster(this->_workSpace, dtmFactor);
	extractObjectPoints(this->_workSpace, dtmFactor, objectHT);

	if (groundTag != NULL)
	{
		delete []groundTag;
		groundTag = NULL;
	}
}

void OBAFilter::dispose()
{
	OBASegment::dispose();
}

/**
* Filter Entitys
*/
void OBAFilter::filterEntitys()
{
	hash_map<int, Entity*>::iterator entityIter;
	Entity *entity = NULL, *nearEntity = NULL;	
	double area = 0.0;
	std::map<std::string, double> params;
	std::vector<PT> pts;
	std::vector<PT>::iterator ptIter;
	int pos = 0;
	bool *localEdge = NULL;
	bool localEdgeTag = false;

	localEdge = new bool[this->_gridW*this->_gridH];
	memset(localEdge, 0, this->_gridW*this->_gridH*sizeof(bool));
	getLocalEdge(this->_workSpace, 1.0, localEdge);

	for(entityIter = this->_entitys.begin(); entityIter != this->_entitys.end(); ++entityIter) 
	{
		entity = entityIter->second;
		// Use the type to identify 
		if (entity->getType()!=PointType::p_unclassified)
		{
			continue;
		}

		// Use area information
		params = entity->getParameters();
		area = params[areaTag];
		//Too big
		if(area == 0.0)
		{
			// Too big
			entity->setType(PointType::p_ground);
			continue;
		}

		// Use edge information
		localEdgeTag = false;
		pts = entity->getAllPts();
		for(ptIter = pts.begin(); ptIter != pts.end(); ++ptIter)
		{
			pos = ptIter->y * this->_gridW + ptIter->x;
			if(localEdge[pos])
			{
				localEdgeTag = true;
				break;
			}
		}
		if(!localEdgeTag && area >= 500)
		{
			entity->setType(PointType::p_ground);
		} else 
		{
			entity->setType(PointType::p_nonground);
		}
	}

	if(localEdge != NULL)
	{
		delete []localEdge;
		localEdge = NULL;
	}
}


