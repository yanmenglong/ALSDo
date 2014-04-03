#ifndef GROUNDFILTER_H
#define GROUNDFILTER_H

#include "global.h"

class WorkSpace;
class GridIndex;

#include <vector>

class LIDARCORE_API TINFilter
{
public:
	TINFilter(WorkSpace *workSpace, const double &maxObjSize, const double &seedCellSize, const double &angleIter, const double &disIter);
	virtual ~TINFilter();

	int filterGround(const double &filteringHeightTh, const double &filteringSlopeTh);

private:
	// fist fillter the grid 
	void filterGrid();

private:
	// Work space
	WorkSpace *_workSpace;

	// Maximum object size and smaller window to search seeds
	double _maxObjSize;
	double _seedCellSize;
	// For filtering parameters 
	double _angleIter;
	double _disIter;

	// Type for each grid 
	bool *_groundTag;

	//Just for better performance
	GridIndex *_gridIndex;
	int _gridW;
	int _gridH;
};

#endif