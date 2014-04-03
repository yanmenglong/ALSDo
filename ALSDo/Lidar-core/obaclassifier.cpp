#include "obaclassifier.h"
#include "seedgenerator.h"
#include "util.h"
#include "workspace.h"

#include <io.h>

OBAClassifier::OBAClassifier(WorkSpace *workSpace)
	: OBASegment(workSpace)
{
	this->_firstFlag = true;
}


OBAClassifier::~OBAClassifier()
{

}

void OBAClassifier::segmentRegionBySeeds(const double &maxObjSize, const double &segHeightTh, const double &segSlopeTh)
{
	int ii = 0, count = 0, iy = 0, ix = 0, pos = 0, id = 0, entityID = 0;
	double ptZ = 0.0;
	bool *enter_queue = NULL, *localEdge = NULL;
	vector<PT> seeds;
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
	localEdge = new bool[count];
	memset(localEdge, 0, count*sizeof(bool));
	getLocalEdge(this->_workSpace, 1.0, localEdge);

	// Find ground seeds, and segment from ground seeds
	SeedGenerator seedGenerator(this->_workSpace, maxObjSize, 20.0);
	seedGenerator.findMaxSeeds(seeds);
	for (ptIter = seeds.begin(); ptIter != seeds.end(); ++ptIter)
	{
		pt = *ptIter;
		pos = pt.y * this->_gridW + pt.x;
		if(this->_entityIDs[pos] != NAN) 
		{
			continue;
		}
		
		type = PointType::p_unclassified;
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
			pt3d = this->_gridIndex->getMaxPT3D(pt.x, pt.y);
			entity->addPoint(pt, pt3d->z);

			if(localEdge[pos])
			{
				continue;
			}
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
			pt3d = this->_gridIndex->getMaxPT3D(ix, iy);
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

				pt3d = this->_gridIndex->getMaxPT3D(pt.x, pt.y);
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

				if(localEdge[pos])
				{
					continue;
				}
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
	if(localEdge != NULL)
	{
		delete []localEdge;
		localEdge = NULL;
	}
}

/**
* Classify entitys by default
*/
void OBAClassifier::classifyEntitys()
{
	std::string specialRegionPath;
	int regionSize = 0, regionWN = 0, regionHN = 0, x = 0, y = 0, pos = 0;
	FILE *fp = NULL;
	double maxObjSize = 0.0;
	bool *specialRegionFlag = NULL;
	hash_map<int, Entity*>::iterator entityIter;
	Entity *entity = NULL, *nearEntity = NULL;
	std::vector<PT> pts;
	std::vector<PT>::iterator ptIter;
	bool inSpecialRegion = false, topMax = true;
	std::map<std::string, double> parameters;
	std::map<int, int> topKeys;
	std::map<int, int>::iterator topKeyIter;

	specialRegionPath = std::string(this->_workSpace->getResFolder()) + std::string("special_region.rect"); 
	if((_access(specialRegionPath.c_str(), 0)) != -1)
	{
		fp = fopen(specialRegionPath.c_str(), "r");
		//first values is the size of max object in the given region 
		fscanf(fp, "%lf\n", &maxObjSize);
		regionSize = maxObjSize / this->_gridIndex->getCellSize();
		regionWN = int(this->_gridW / regionSize) + 1;
		regionHN = int(this->_gridH / regionSize) + 1;
		specialRegionFlag = new bool[regionHN * regionWN];
		memset(specialRegionFlag, 0, regionHN * regionWN * sizeof(bool));

		while (fscanf(fp, "%d %d\n", &x, &y) != EOF)
		{
			pos = y * regionWN + x;
			specialRegionFlag[pos] = true;
		}		
		fclose(fp);
		fp = NULL;
	}

	for(entityIter = this->_entitys.begin(); entityIter != this->_entitys.end(); ++entityIter)
	{
		entity = entityIter->second;
		if(entity->getType() == PointType::p_created)
		{
			continue;
		}
		entity->setType(PointType::p_high_vegetation);

		if(specialRegionFlag != NULL)
		{	
			inSpecialRegion = false;
			pts = entity->getAllPts();
			for(ptIter = pts.begin(); ptIter != pts.end(); ++ptIter)
			{	
				x = int(ptIter->x/regionSize);
				y = int(ptIter->y/regionSize);
				pos = y * regionWN + x;
				if(specialRegionFlag[pos])
				{
					inSpecialRegion = true;
					break;
				}
			}
			if(inSpecialRegion)
			{
				continue;
			}
		}	
			
		parameters = entity->getParameters();
		if(parameters[areaTag]>=100 && parameters[areaTag]<=5000 && parameters[meanTag]>1)
		{
			if(parameters[cellZDiffTag]<=parameters[boundingZDiffTag])
			{
				entity->setType(PointType::p_building);
			}

			topMax = true;
			topKeys = entity->getTopKeys();
			for(topKeyIter = topKeys.begin(); topKeyIter != topKeys.end(); ++topKeyIter)
			{
				nearEntity = this->_entitys[topKeyIter->first];
				parameters = nearEntity->getParameters();
				if(parameters[areaTag]>=100 && parameters[areaTag]<=5000 && nearEntity->getType()!= PointType::p_building 
					&& nearEntity->getMean() > entity->getMean())
				{
					topMax = false;
					break;
				}
			}
			if(topMax)
			{
				entity->setType(PointType::p_building);
			}
		}		
	}

	if(specialRegionFlag != NULL)
	{
		delete []specialRegionFlag;
		specialRegionFlag = NULL;
	}
	this->_firstFlag = false;
}

/**
* Classify entitys by paramTag
*/
void OBAClassifier::classifyEntitys(const string &paramTag, const double &valueMin, const double &valueMax)
{
	hash_map<int, Entity*>::iterator entityIter;
	Entity *entity = NULL;
	std::map<std::string, double> parameters;
	double value = 0.0;

	for(entityIter = this->_entitys.begin(); entityIter != this->_entitys.end(); ++entityIter)
	{
		entity = entityIter->second;
		if(entity->getType() == PointType::p_created)
		{
			continue;
		}

		parameters = entity->getParameters();
		value = parameters[paramTag];
		if(this->_firstFlag)
		{
			if(value >= valueMin && value <= valueMax)
			{
				entity->setType(PointType::p_building);
			}
			else 
			{
				entity->setType(PointType::p_high_vegetation);
			}
		}
		else 
		{
			if(value < valueMin || value > valueMax)
			{
				entity->setType(PointType::p_high_vegetation);
			}
		}		
	}
	this->_firstFlag = false;
}

/**
* Rest the type of entitys
*/
void OBAClassifier::resetEntitysType()
{
	hash_map<int, Entity*>::iterator entityIter;
	Entity *entity = NULL;

	for(entityIter = this->_entitys.begin(); entityIter != this->_entitys.end(); ++entityIter)
	{
		entity = entityIter->second;
		if(entity->getType() == PointType::p_created)
		{
			continue;
		}
		entity->setType(PointType::p_unclassified);
	}
	this->_firstFlag = true;
}

void OBAClassifier::enterQueue(PT& pt, PointType type, bool *enterQueueFlag, const double &segHeightTh, const double &segSlopeTh)
{
	PT nhbor[4];
	int ii = 0, pos = 0;
	double hdiff = 0.0, angle = 0.0;
	PT3D *pt3d1 = NULL, *pt3d2 = NULL;
	
	pt3d1 = this->_gridIndex->getMaxPT3D(pt.x, pt.y);
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
		if(this->_entityIDs[pos] != NAN)
		{
			continue;		
		}

		pt3d2 = this->_gridIndex->getMaxPT3D(nhbor[ii].x, nhbor[ii].y);
		/* If point type is invalid, only hole is allowed to enter into the queue */
		if(type == PointType::p_created) 
		{
			if(pt3d2 == NULL) 
			{
				this->_queue.push_back(nhbor[ii]);
				enterQueueFlag[pos] = true;
			}
			else
			{
				continue;
			}			
		}
		else 
		{
			// If given local minimum index of point does not exist, ignore it
			if(pt3d2 == NULL)  
			{
				continue;
			}
			
			getHeightDiffandAngleDiff(*pt3d1, *pt3d2, hdiff, angle);
			if(hdiff > segHeightTh || angle > segSlopeTh)
			{
				continue;
			}
			
			this->_queue.push_back(nhbor[ii]);
			enterQueueFlag[pos] = true;
		}
	}	
}

void OBAClassifier::dispose()
{
	OBASegment::dispose();
}