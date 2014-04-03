#ifndef OBASEGMENT_H
#define OBASEGMENT_H

#include "global.h"
#include "lasio.h"
#include "gridindex.h"
#include "entity.h"

class WorkSpace;

#include <hash_map>
#include <deque>
#include <iostream>
#include <string>
using namespace std;
#ifdef WIN32
	using namespace stdext;
#else
	using namespace __gun_cxx;
#endif

class LIDARCORE_API OBASegment
{
public:
	OBASegment(WorkSpace *workSpace);
	virtual ~OBASegment();

	// Set maximum object size for seeg generation
	virtual void segmentRegion(const double &heightT, const double &angleT);
	virtual void findBounding();
	virtual void sortBounding();
	virtual void findTop();
	virtual int removeHole(const int &size);
	virtual int removeEntity(const int &size);
	virtual void statisticEntityInfo(const int &sizeTh);

	// Get information of object 
	virtual int getObjNum();
	virtual int getEntityID(const int &x, const int &y);
	virtual Entity* getEntity(const int &entityID);
	virtual Entity* getEntity(const int &x, const int &y);
	virtual hash_map<int, Entity*>* getEntitys();

protected:
	// Method just for segmentation 
	virtual void enterQueue(PT& pt, PointType type, bool *enterQueueFlag, const double &segHeightTh, const double &segSlopeTh);
	virtual PT popQueue();
	virtual void clearQueue(bool *enterQueueFlag);
	virtual bool isQueueEmpty();
	virtual void updateTop(const int &desID, const int &ID);
	
	// Dispose garbage
	virtual void dispose();

protected:
	WorkSpace *_workSpace;
	// For segmentation 
	int *_entityIDs;
	hash_map<int, Entity*> _entitys;
	deque<PT> _queue;

	// Just for better speed
	GridIndex *_gridIndex;
	int _gridW;
	int _gridH;
};

#endif
