#include "obasegment.h"
#include "workspace.h"
#include "util.h"

#include <cmath>
#include <algorithm>
#include <io.h>

/**
* Constructor
*
* workSpace: the basic object to retrieve the space information
*/
OBASegment::OBASegment(WorkSpace *workSpace) 
{
	this->_workSpace = workSpace;
	this->_entityIDs = NULL; 

	this->_gridIndex = this->_workSpace->getGridIndex();
	this->_gridW = this->_gridIndex->getGridWidth();
	this->_gridH = this->_gridIndex->getGridHeight();
}

/**
* Deconstructor
* It will be called dynamically 
*/
OBASegment::~OBASegment()
{
	this->dispose();
}

/**
* Segment region without seeds
*
* heightT: height threshold
* angleT : angle threshold
*/
void OBASegment::segmentRegion(const double &heightT, const double &angleT)
{
	int ii = 0, count = 0, iy = 0, ix = 0, pos = 0, id = 0, entityID = 0;
	double ptZ = 0.0;
	bool *enter_queue = NULL;
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
				this->enterQueue(pt, type, enter_queue, heightT, angleT);
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

/**
* Find bounding of each entity
*/
void OBASegment::findBounding() 
{
	hash_map<int,Entity*>::iterator it;	
	Entity* entity = NULL;

	for(it = this->_entitys.begin(); it != this->_entitys.end(); ++it) 
	{
		entity = it->second;
		entity->findBounding(this->_gridW, this->_gridH, this->_entityIDs);
	}
}

/**
* Sort the bounding points of the entity
*/
void OBASegment::sortBounding()
{
	hash_map<int,Entity*>::iterator it;	
	Entity *entity = NULL;
	bool *bdFlag = NULL, *visitFlag = NULL;

	bdFlag = new bool[this->_gridW * this->_gridH];
	visitFlag = new bool[this->_gridW * this->_gridH];
	memset(bdFlag, 0, this->_gridW * this->_gridH * sizeof(bool));
	memset(visitFlag, 0, this->_gridW * this->_gridH * sizeof(bool));

	for(it = this->_entitys.begin(); it != this->_entitys.end(); ++it) 
	{
		entity = it->second;
		if(entity->getType() != PointType::p_building)
		{
			continue;
		}
		entity->sortBounding(this->_gridW, this->_gridH, bdFlag, visitFlag);
	}

	if(bdFlag != NULL)
	{
		delete []bdFlag;
		bdFlag = NULL;
	}
	if(visitFlag != NULL)
	{
		delete []visitFlag;
		visitFlag = NULL;
	}
}

/**
* Find topology information of each entity
*/
void OBASegment::findTop()
{
	hash_map<int,Entity*>::iterator it;	
	Entity* entity = NULL;

	for(it = this->_entitys.begin(); it != this->_entitys.end(); ++it) 
	{
		entity = it->second;
		entity->findTop(this->_gridW, this->_gridH, this->_entityIDs);
	}
}

/**
* Remove the invalid entity
*
* size: size threshold of entity
*/
int OBASegment::removeHole(const int &size)
{
	int count = 0, ii = 0, id = 0, des_id = 0, ptNum = 0, max_ptNum = 0, jj = 0, re_obj_num = 0;
	hash_map<int, Entity*>::iterator obj_it;
	vector<int> all_keys;
	Entity *entity = NULL;
	map<int, int> top_keys;
	Entity *near_entity = NULL;
	vector<PT> pts;
	PT pt;

	for(obj_it = this->_entitys.begin(); obj_it != this->_entitys.end(); ++obj_it) 
	{
		all_keys.push_back(obj_it->first);
	}

	count = all_keys.size();
	for(ii = 0; ii < count; ++ii) 
	{
		entity = this->_entitys[all_keys[ii]];

		// If its size is bigger than given threshold or it is not the right type, just pass it
		if(entity->getPtNum() > size || entity->getType() != PointType::p_created) 
		{
			continue;
		}

		top_keys = entity->getTopKeys();
		max_ptNum = -1;
		des_id = -1;
		for(map<int, int>::iterator key_itor = top_keys.begin(); key_itor != top_keys.end(); ++key_itor) 
		{
			id = key_itor->first;
			near_entity = this->_entitys[id];

			ptNum = near_entity->getPtNum();
			if(ptNum > max_ptNum) 
			{
				max_ptNum = ptNum;
				des_id = id;						
			}
		}

		if(des_id == -1) 
		{
			continue;
		}
		
		pts = entity->getAllPts();
		ptNum = pts.size();
		for(jj = 0; jj < ptNum; ++jj) 
		{
			pt = pts[jj];
			this->_entityIDs[pt.y * this->_gridW + pt.x] = des_id;			
		}
		near_entity = this->_entitys[des_id];
		near_entity->addPoints(pts);

		this->updateTop(des_id, entity->getID());

		this->_entitys.erase(entity->getID());
		delete entity;
		entity = NULL;

		re_obj_num++;
	}	
	return re_obj_num;	
}

/**
* Remove the an entity if it is too small
*
* size: size threshold of entity
*/
int OBASegment::removeEntity(const int &size)
{
	int count = 0, ii = 0, id = 0, des_id = 0, ptNum = 0, max_ptNum = 0, jj = 0, re_obj_num = 0;
	hash_map<int, Entity*>::iterator obj_it;
	vector<int> all_keys;
	Entity *entity = NULL;
	map<int, int> top_keys;
	Entity *near_entity = NULL;
	vector<PT> pts;
	PT pt;

	for(obj_it = this->_entitys.begin(); obj_it != this->_entitys.end(); ++obj_it) 
	{
		all_keys.push_back(obj_it->first);
	}

	count = all_keys.size();
	for(ii = 0; ii < count; ++ii) 
	{
		entity = this->_entitys[all_keys[ii]];

		if(entity->getPtNum() > size || entity->getTopKeys().size() != 1) 
		{
			continue;
		}

		top_keys = entity->getTopKeys();
		max_ptNum = -1;
		des_id = -1;
		for(map<int, int>::iterator key_itor = top_keys.begin(); key_itor != top_keys.end(); ++key_itor) 
		{
			id = key_itor->first;
			near_entity = this->_entitys[id];

			if(near_entity->getType() == PointType::p_created)
			{
				continue;
			}

			ptNum = near_entity->getPtNum();
			if(ptNum > max_ptNum) 
			{
				max_ptNum = ptNum;
				des_id = id;						
			}
		}

		if(des_id == -1) 
		{
			continue;
		}
		
		pts = entity->getAllPts();
		ptNum = pts.size();
		for(jj = 0; jj < ptNum; ++jj) 
		{
			pt = pts[jj];
			this->_entityIDs[pt.y * this->_gridW + pt.x] = des_id;			
		}
		near_entity = this->_entitys[des_id];
		near_entity->addPoints(pts);

		this->updateTop(des_id, entity->getID());

		this->_entitys.erase(entity->getID());
		delete entity;
		entity = NULL;

		re_obj_num++;
	}	
	return re_obj_num;	
}

/**
* Statistic the basic information of each entity
*
* sizeTh: size threshold of entity, the entity whose size is bigger than the given sizeTh will be ignored 
*/
void OBASegment::statisticEntityInfo(const int &sizeTh)
{
	GridIndex *gridIndex = NULL;
	Entity *entity = NULL;

	gridIndex = this->_workSpace->getGridIndex();
	for(hash_map<int, Entity*>::iterator it = this->_entitys.begin(); it != this->_entitys.end(); ++it)
	{
		entity = it->second;
		entity->statistic(gridIndex->getCellSize(), sizeTh, gridIndex);
	}
}

/**
* Retrival total number of entities
*/
int OBASegment::getObjNum()
{
	return this->_entitys.size();
}

/**
* Retrival entity id at (x, y)
*
* x, y -> (x, y)
*/
int OBASegment::getEntityID(const int &x, const int &y)
{
	return this->_entityIDs[y * this->_gridW + x];
}

/**
* Retrival entity by given entity id
*
* entityID: the ID of entity to be retrieved
*/ 
Entity* OBASegment::getEntity(const int &entityID)
{
	if(this->_entitys.find(entityID) != this->_entitys.end())
	{
		return this->_entitys[entityID];	
	}
	return NULL;
}

/**
* Retrival entity by (x, y) in entity matrix
*
* x, y -> (x, y)
*/
Entity* OBASegment::getEntity(const int &x, const int &y)
{
	return this->_entitys[this->_entityIDs[y * this->_gridW + x]];	
}

/**
* Retrival entities
*/
hash_map<int, Entity*>* OBASegment::getEntitys()
{
	return &this->_entitys;
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
void OBASegment::enterQueue(PT& pt, PointType type, bool *enterQueueFlag, const double &segHeightTh, const double &segSlopeTh) 
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
		if(this->_entityIDs[pos] != NAN)
		{
			continue;		
		}

		pt3d2 = this->_gridIndex->getMinPT3D(nhbor[ii].x, nhbor[ii].y);
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

/**
* Pop a PT from the queue
*/
PT OBASegment::popQueue() 
{
	PT pt = this->_queue.front();
	this->_queue.pop_front();
	return pt;
}

/**
* Clear the queue
*
* enterQueueFlag: the entering queue flag
*/
void OBASegment::clearQueue(bool *enterQueueFlag)
{
	int pos = 0;
	PT pt;

	for(std::deque<PT>::iterator it = this->_queue.begin(); it != this->_queue.end(); ++it)
	{
		pt = *it;
		pos = pt.y * this->_gridW + pt.x;
		enterQueueFlag[pos] = false;
	}
	this->_queue.clear();
}

/**
* Check whether the queue is empty
*/
bool OBASegment::isQueueEmpty() 
{
	return this->_queue.empty();
}

/**
* Merge entity with identity ID to entity with identity desID
*
* desID: the destination identity
* ID: the identity of entity to be merged
*/
void OBASegment::updateTop(const int &desID, const int &ID) 
{
	Entity *desEntity = NULL, *entity = NULL, *nearEntity = NULL;

	// Remove the top relationship between entity of des_id and entity of id 
	desEntity = this->_entitys[desID];
	desEntity->removeTop(ID);
	entity = this->_entitys[ID];
	entity->removeTop(desID);

	// Update relationship of the neighbour of entity of id 
	map<int, int> topkeys = entity->getTopKeys();
	for(map<int, int>::iterator keyIt = topkeys.begin(); keyIt != topkeys.end(); ++keyIt) 
	{
		entity->removeTop(keyIt->first);
		desEntity->addTop(keyIt->first);
		
		nearEntity = this->_entitys[keyIt->first];
		nearEntity->removeTop(ID);
		nearEntity->addTop(desID);
	}
}

/**
* Dispose the garbage
*/
void OBASegment::dispose()
{
	if(this->_entityIDs != NULL) 
	{
		delete []this->_entityIDs;
		this->_entityIDs = NULL;
	}
	
	for(hash_map<int,Entity*>::iterator it = this->_entitys.begin(); it != this->_entitys.end(); ++it)
	{
		Entity* entity = it->second;
		if(entity != NULL)
		{
			delete entity;
			entity = NULL;
		}
	}
	this->_entitys.clear();

	this->_queue.clear();
}



