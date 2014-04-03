#ifndef GRIDFILTER_H
#define GRIDFILTER_H

#include <deque>
using namespace std;

#include "global.h"
class WorkSpace;
class GridIndex;

class LIDARCORE_API GridFilter 
{
public:
	GridFilter(WorkSpace *workSpace);
	virtual ~GridFilter();

	// Do ssed expand with given threshold
	void seedExpand(const double &maxObjSize, const double &seedCellSize, const double &heightTh, 
		const double &slopeTh);
	// Filetering method 
	void filterPoints(const double &filteringHeightTh, const double &filteringSlopeTh, const double &dtmFactor,
		const double &objectHT);

protected:
	// Filtering grid
	void filterGrid();

	// Method just for seed expand 
	void enterQueue(PT& pt, bool *enterQueueFlag, const double &heightTh, const double &slopeTh);
	PT popQueue();
	bool isQueueEmpty();
	
	// Dispose the garbage
	void dispose();

private:
	// ground tag
	bool *_groundTag;

	WorkSpace *_workSpace;

	// Just for better speed
	GridIndex *_gridIndex;
	int _gridW;
	int _gridH;
	
	// For seed expand 
	deque<PT> _queue;	
};

#endif
