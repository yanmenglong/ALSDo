#ifndef GRIDINDEX_H
#define GRIDINDEX_H

#include "lasio.h"
#include "global.h"
class WorkSpace;

#include <deque>

/**
* Core structure of grid index
*/
struct bucket 
{
public:
	bucket()
	{
	}

	~bucket()
	{
		v.clear();
	}

	std::deque<int> v;
};

/**
* Grid index is the core index of the whole process system.
* 
*/
class LIDARCORE_API GridIndex 
{
public:
	GridIndex(WorkSpace *workSpace, const double &cellSize);
	~GridIndex();

	// Import the DTM to get the height of each point
	void importDTM(const string &dtmPath);
	// Get minimum PT3D value of cell at given (ix, iy)
	PT3D* getMinPT3D(const int &ix, const int &iy);
	// Get maximum PT3D value of cell at given (ix, iy)
	PT3D* getMaxPT3D(const int &ix, const int &iy);
	// Get 4 neighborhood PTs at given (ix, iy)
	void get4NeighPTs(const int &ix, const int &iy, PT nhbor[4]);
	// Get 8 neighborhood PTs at given (ix, iy)
	void get8NeighPTs(const int &ix, const int &iy, PT nhbor[8]); 
	// Get basic information of the grid index
	LASHeader* getLASHeader();
	double getCellSize();
	int getGridHeight();
	int getGridWidth(); 	

protected:
	// Create the grid index
	void createIndex();	
	// Dispose garbage
	void dispose();

private:
	// Put all points in a bucket buffer to create the grid index
	void putLASPt2GridCell(LASHeader *lasHeader, LASPoint *lasPts, const int &ptN, bucket *bs);
	// Remove the high and low points
	void removeHighLowPoints(LASPoint *lasPts, bucket *bs, const double &zTh);

private:
	// Workspace to store the index instance
	WorkSpace *_workSpace;
	// Meta information of original las file
	LASHeader *_lasHeader;
	// Cell size for grid index
	double _cellSize;
	// Height for grid index matrix
	int _gridHeight;
	// Width for grid index matrix
	int _gridWidth;
	// Cell minimum stand points and maximum stand points
	PT3D *_cellMinPts;
	PT3D *_cellMaxPts;
};

#endif