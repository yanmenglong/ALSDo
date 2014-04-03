#ifndef SEEDGENERATOR_H
#define SEEDGENERATOR_H

#include "global.h"
class WorkSpace;
class GridIndex;

#include <vector>

class SeedGenerator
{
public:
	SeedGenerator(WorkSpace *workSpace, const double &maxObjSize, const double &seedCellSize);
	virtual ~SeedGenerator();

	// Find seeds of local minimum z-value by grid min stand point
	void findMinSeeds(std::vector<PT> &seeds); 
	// Find seeds of local minimum z-value by grid max stand point
	void findMaxSeeds(std::vector<PT> &seeds);

private:
	// Find seeds in smaller size of minimum z-value
	void findMinSeedsInCell(const int &wL, const int &wH, const int &hL, const int &hH, std::vector<PT> &seeds);
	// Find seeds at four corners
	void findMinSeedsAtCorners(std::vector<PT> &seeds);

protected:
	WorkSpace *_workSpace;
	double _maxObjSize;
	double _seedCellSize; 

	// Just for better performance
	GridIndex *_gridIndex;
	int _gridW;
	int _gridH;
};

#endif

