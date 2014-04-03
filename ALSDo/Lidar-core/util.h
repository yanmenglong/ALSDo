#ifndef UTIL_H
#define UTIL_H

#include "global.h"
#include "gridindex.h"
#include "workspace.h"

#include <vector>

#ifdef __cplusplus
extern "C" 
{
#endif

#define EXPORT extern "C" _declspec(dllexport)

/**
* Identify the complexity of scene by object based analysis
*/
EXPORT void calcComplexByObj(WorkSpace *workSpace, const double &maxObjSize, const double &seedHeightTh, const double &seedSlopeTh);

/**
* Identify the complexity of scene by local edge statistic
*/
EXPORT void calcComplexByLocalEdge(WorkSpace *workSpace, const double &hT, const double &maxObjSize);

/**
* Filter the left points by grid ground tag
*
* workSpace: the project work space
* hT: the height threshold
* angleT: the angle threshold
* groundTag: the tag identify whether the cell is ground cell
*/
EXPORT void filterPtsByGrid(WorkSpace *workSpace, const double &filteringHeightTh, const double &filteringSlopeTh, const bool *groundTag);

/**
* Extract object points with given tolenrance
*/
EXPORT int extractObjectPoints(WorkSpace *workSpace, const double &dtmFactor, const double &objHT);

/**
* Extract building LAS points
*/
EXPORT int exportBuildingLASPoints(WorkSpace *workSpace);

/**
* Extract building shapefile boundary polygons
*/
EXPORT int exportBuildingPolygons(WorkSpace *workSpace);

/**
* Extract building raster
*/
EXPORT int exportBuildingRaster(WorkSpace *workSpace);

/**
* Clip las file with given region
*/
EXPORT void clipRegion(const char *lasPath, const char *outLasPath, const double &xmin, const double &ymin, const double &xmax, const double &ymax);

/** 
* Merge the given datasets
*/
EXPORT void mergePoints(std::vector<const char *> las_paths_l, const char *out_las_path);

/**
* It is used to calcuate the angle and height difference between two given ponts p1 and p2
*/
void getHeightDiffandAngleDiff(PT3D &p1, PT3D &p2, double &hdiff, double &angle);

/**
* Get the local height different matrix
*/
void getLocalEdge(WorkSpace *workSpace, const double &hT, bool *edge);

/**
* It is used to convet las to rasterZ in memory
*/
void las2RasterZ(WorkSpace *workSpace, const double &dtmFactor, const int &rasterW, const int &rasterH, float *rasterZ);

/**
* Interpolate dem with given resolution
*/
EXPORT void las2Raster(WorkSpace *workSpace, const double &dtmFactor);

#ifdef __cplusplus
}
#endif

#endif