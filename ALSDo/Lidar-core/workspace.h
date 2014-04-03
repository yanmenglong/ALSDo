#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <cstdio>
#include <string> 
#include <vector>
using namespace std;

#include "global.h"
#include "lasio.h"

class GridIndex;
class OBAFilter;
class OBAClassifier;
class VegetationExtractor;

class LIDARCORE_API WorkSpace 
{
public:
	WorkSpace();
	~WorkSpace();

	//Create a new project, the result directory must exist
	void newProj(const char *lasPath, const double &cellSize, const char *resultDir);
	//Resample the z value for demonstration
	unsigned char* resampleZ();
	//Get result path and las file path (end with '/')
	const char* getLasPath();
	const char* getResFolder();
	//Get the grid index
	GridIndex* getGridIndex();
	//Get processors
	OBAFilter* getOBAFilter();
	OBAClassifier *getOBAClassifier();
	VegetationExtractor* getVegetationExtractor();

protected:
	void dispose();

private:
	//result folder and las file path
	string _resFolder;
	string _lasPath;
	// grid index 
	GridIndex *_gridIndex;
	// resample data 	
	unsigned char *_zValue;

	/* processor */	
	OBAFilter *_obaFilter;
	OBAClassifier *_obaClassifier;
	VegetationExtractor *_vegetationExtractor;
};

#endif