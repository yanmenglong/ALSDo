#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <map>
#include <hash_map>
using namespace std;
#ifdef WIN32
	using namespace stdext;
#else
	using namespace __gun_cxx;
#endif

#include "global.h"

class GridIndex;

const string areaTag("area");
const string meanTag("mean");
const string varianceTag("variance");
const string cellZDiffTag("cellZDiff");
const string boundingZDiffTag("boundingZDiff");
const string boundingHTag("boundingH");
const string boundingHVariTag("boundingHVari");
const string perimeterTag("perimeter");
const string mbrLengthTag("mbrLength");
const string mbrWidthTag("mbrWidth");
const string mbrPerimeterTag("mbrPerimeter");
const string perimeterAndMbrPerimeterRateTag("perimeterAndMbrPerimeterRate");
const string mbrAreaTag("mbrArea");
const string areaAndMbrAreaRateTag("areaAndMbrAreaRate");
const string compactnessTag("compactness");
const string topologyTag("topology");

/***********************************************************
Entity class£ºabstraction of segmentation results
***********************************************************/
class LIDARCORE_API Entity 
{
public:
	Entity(int id, PointType type);
	virtual ~Entity();
	
	//Set the start point
	void setSeedPoint(const PT &pt);
	//Add point
	void addPoint(const PT &pt, const double &v);
	//Add points
	void addPoints(const vector<PT> &pts);
	void addCurrQueue(const PT &pt);
	void clearCurrQueue();
	
	void findBounding(const int &width, const int &height, int* ids);
	void sortBounding(const int &width, const int &height, bool *bdFlag, bool *visitFlag);
	
	void findTop(int width, int height, int* ids);
	void addTop(int id);
	void removeTop(int id);

	// Statistic the information of the object, inclcuding area, mean, variance, mbr length, mbr width, mbr area, compactness, topology
	void statistic(double cellSize, int sizeTh, GridIndex *gridIndex);
	void removeInnerPts();

	int& getID();
	double& getMean();	
	void setType(PointType type); 
	PointType& getType();
	int& getPtNum();
	vector<PT>& getAllPts();
	vector<PT>& getBounding();
	vector<PT>& getCurrentQueue();
	PT& getCenterPos();

	map<int, int>& getTopKeys();
	map<string, double>& getParameters();
	PT& getSeedPt();

private:
	/* Get the 4 neighborhood points of given point pt */
	void get4Neighbor(PT &pt, PT nhbor[4]);
	/* Get the 8 neighborhood points of given point pt */
	void get8Neighbor(PT &pt, PT nhbor[8]);

private:
	int _id;
	int _ptNum;
	double _mean;
	PT _centerPt;

	vector<PT> _pts;
	vector<PT> _bound;
	vector<PT> _currQueue;
	map<int, int> _topkeys;
	map<string, double> _parameters;
	PointType _type;
	PT _seedPt;
};

#endif