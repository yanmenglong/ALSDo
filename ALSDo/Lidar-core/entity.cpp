#include "entity.h"
#include "GridIndex.h"

#include <list>
#include <algorithm>
#include <cmath>
using namespace std;


/**
* Find the minimum bounding rectangle of the current entity
*/
static void findMBR(double &mbrLength, double &mbrWidth, double &mbrArea, vector<PT> boundings)
{
	double cosV[18] = {1.0, 0.99619, 0.98481, 0.96593, 0.93969, 0.90631, 0.86603, 0.81915, 0.76604, 0.70711, 
                    0.64279, 0.57358, 0.5, 0.42262, 0.34202, 0.25882, 0.17365, 0.087156 };
	double sinV[18] = {0.0, 0.087156, 0.17365, 0.25882, 0.34202, 0.42262, 0.5, 0.57358, 0.64279, 0.70711, 
                    0.76604, 0.81915, 0.86603, 0.90631, 0.93969, 0.96593, 0.98481, 0.99619 };
	int step = 0;
	double minX = 9999999.0, minY = 9999999.0, maxX = -9999999.0, maxY = -9999999.0, XX = 0.0, YY = 0.0, width = 0.0, height = 0.0, 
		area = 0.0, minArea = 9999999.0; 
	std::vector<PT>::iterator it;
	PT pt;

	for(step = 0; step < 18; ++step)
	{
		minX = 9999999.0;
		minY = 9999999.0;
		maxX = -9999999.0;
		maxY = -9999999.0;

		for(it = boundings.begin(); it != boundings.end(); ++it)
		{
			pt = *it;
			XX = pt.x * cosV[step] + pt.y * sinV[step];
            YY = -pt.x * sinV[step] + pt.y * cosV[step];

			minX = min(minX, XX);
			minY = min(minY, YY);
			maxX = max(maxX, XX);
			maxY = max(maxY, YY);
		}

		width = maxX-minX;
		height = maxY-minY;
		area = height*width;

		if(area < minArea) {
			mbrLength = height > width ? height : width;
			mbrWidth = height < width ? height : width;
			mbrArea = area;
		}
	}
}

/**
*
*/
Entity::Entity(int id, PointType type) 
{
	this->_id = id;
	this->_type = type;
	this->_ptNum = 0;
	this->_mean = 0.0;
	this->_centerPt.x = 0;
	this->_centerPt.y = 0;
}

/**
*
*/
Entity::~Entity()
{
	this->_pts.clear();
	this->_bound.clear();
	this->_currQueue.clear();
	this->_topkeys.clear();
	this->_parameters.clear();
}

/**
*
*/
void Entity::setSeedPoint(const PT &pt)
{
	this->_seedPt = pt;
}

/**
*
*/
void Entity::addPoint(const PT &pt, const double &v) 
{
	this->_pts.push_back(pt);
	this->_mean = (this->_mean * this->_ptNum + v)/(this->_ptNum + 1);
	this->_centerPt.x = (this->_ptNum*this->_centerPt.x + pt.x)/(this->_ptNum + 1);
	this->_centerPt.y = (this->_ptNum*this->_centerPt.y + pt.y)/(this->_ptNum + 1);
	this->_ptNum++;
}

/**
*
*/
void Entity::addPoints(const vector<PT> &pts)
{
	int ptN = pts.size(), ii = 0;
	for(ii = 0; ii < ptN; ++ii)
	{
		this->_pts.push_back(pts[ii]);
		this->_centerPt.x = (this->_ptNum*this->_centerPt.x + pts[ii].x)/(this->_ptNum + 1);
		this->_centerPt.y = (this->_ptNum*this->_centerPt.y + pts[ii].y)/(this->_ptNum + 1);
	}
	this->_ptNum += ptN;
}

/**
*
*/
void Entity::addCurrQueue(const PT &pt)
{
	this->_currQueue.push_back(pt);
}

/**
*
*/
void Entity::clearCurrQueue()
{
	this->_currQueue.clear();
}

/**
*
*/
void Entity::findBounding(const int &width, const int &height, int* ids)
{
	PT nhbor[4], pt1, pt2;
	int count = 0, ii = 0, jj = 0;
	
	this->_bound.clear();
	count = this->_pts.size();
	// Find all bounding pixels 
	for(ii = 0; ii < count; ++ii)
	{
		pt1 = this->_pts[ii];
		if(pt1.x == 0 || pt1.x == (width - 1) || pt1.y == 0 || pt1.y == (height - 1)) 
		{
			this->_bound.push_back(pt1);			
		} 
		else 
		{
			this->get4Neighbor(pt1, nhbor);

			for(jj = 0; jj < 4; ++jj) 
			{
				pt2 = nhbor[jj];
				if(pt2.x < 0 || pt2.x >= width || pt2.y < 0 || pt2.y >= height)
				{
					continue;
				}

				if(ids[pt2.y * width + pt2.x] != this->_id) 
				{
					this->_bound.push_back(pt1);
				}
			}
		}
	}
}

/**
* 
*/
void Entity::sortBounding(const int &width, const int &height, bool *bdFlag, bool *visitFlag)
{
	std::list<PT> bound;
	PT currPt, pt, minPt;
	int tmpDis = 0, minDis = 99999999;

	// Put bounding points in list
	for(std::vector<PT>::iterator ptIter = this->_bound.begin(); ptIter != this->_bound.end(); ++ptIter)
	{
		bound.push_back(*ptIter);
	}
	this->_bound.clear();

	// Sort bounding points
	std::list<PT>::iterator ptIter = bound.begin();
	currPt = *ptIter;
	this->_bound.push_back(currPt);
	bound.erase(ptIter);

	while(bound.size() > 0)
	{
		minDis = 99999999;
		for(std::list<PT>::iterator tmpIter = bound.begin(); tmpIter != bound.end(); ++tmpIter)
		{
			pt = *tmpIter;
			tmpDis = (currPt.x-pt.x)*(currPt.x-pt.x) + (currPt.y-pt.y)*(currPt.y-pt.y);
			if(tmpDis < minDis)
			{
				minDis = tmpDis;
				minPt = *tmpIter;
			}
		}

		minDis = sqrt((double)minDis);
		if(minDis < 4)
		{			
			this->_bound.push_back(currPt);
			currPt = minPt;
		}
		// No matter what happened remove the minPt, which could be a currPt
		bound.erase(std::find(bound.begin(), bound.end(), minPt));
	}
}

/**
*
*/
void Entity::findTop(int width, int height, int* ids) 
{
	PT nhbor[4], pt1, pt2;
	int count = 0,ii = 0,jj = 0,id = 0;

	this->_topkeys.clear();
	count = this->_bound.size();	
	for(ii = 0; ii < count; ++ii) 
	{
		pt1 = this->_bound[ii];
		this->get4Neighbor(pt1, nhbor);

		for(jj = 0; jj < 4; ++jj) 
		{
			pt2 = nhbor[jj];

			if(pt2.x < 0 || pt2.x >= width || pt2.y < 0 || pt2.y >= height) 
			{
				continue;
			}

			id = ids[pt2.y * width + pt2.x];
			if(id == NAN)
			{
				continue;
			}

			if(id != this->_id)
			{
				this->addTop(id);
			}
		}
	}
}

/**
*
*/
void Entity::addTop(int id) 
{
	this->_topkeys[id] = id;
}

/**
*
*/
void Entity::removeTop(int id) 
{
	this->_topkeys.erase(id);
}

/**
*
*/
void Entity::statistic(double cellSize, int sizeTh, GridIndex *gridIndex)
{
	int gridW = 0, count = 0;
	PT3D *pt3d1 = NULL, *pt3d2 = NULL;
	double sum = 0.0;

	double area = 0.0;
	// mean is already defined
	double variance = 0.0;
	double cellZDiff = 0.0;
	double boundingZDiff = 0.0;
	double boundingH = 0.0;
	double boundingHVari = 0.0;
	double perimeter = 0.0;
	double mbrLength = 0.0;
	double mbrWidth = 0.0;
	double mbrPerimeter = 0.0;
	double perimeterAndMbrPerimeterRate = 0.0;
	double mbrArea = 0.0;
	double areaAndMbrAreaRate = 0.0;
	double compactness = 0.0;
	// topology is already defined
	std::vector<PT>::iterator ptIter;

	this->_parameters.clear();
	if(this->_type == PointType::p_created || (sizeTh != NAN && this->_pts.size() > sizeTh))
	{
		this->_parameters[areaTag] = area;
		this->_parameters[meanTag] = this->_mean;
		this->_parameters[varianceTag] = variance;
		this->_parameters[cellZDiffTag] = cellZDiff;
		this->_parameters[boundingZDiffTag] = boundingZDiff;
		this->_parameters[boundingHTag] = boundingH;
		this->_parameters[boundingHVariTag] = boundingHVari;
		this->_parameters[perimeterTag] = perimeter;
		this->_parameters[mbrLengthTag] = mbrLength;
		this->_parameters[mbrWidthTag] = mbrWidth;
		this->_parameters[mbrPerimeterTag] = mbrPerimeter; 
		this->_parameters[perimeterAndMbrPerimeterRateTag] = perimeterAndMbrPerimeterRate;
		this->_parameters[mbrAreaTag] = mbrArea;  
		this->_parameters[areaAndMbrAreaRateTag] = areaAndMbrAreaRate; 
		this->_parameters[compactnessTag] = compactness;
		this->_parameters[topologyTag] = this->_topkeys.size();
		return;
	}

	// area
	area = this->_pts.size() * cellSize * cellSize;

	// mean is known, get variance
	count = 0;
	sum = 0.0;
	gridW = gridIndex->getGridWidth();
	for(ptIter = this->_pts.begin(); ptIter != this->_pts.end(); ++ptIter)
	{
		pt3d1 = gridIndex->getMinPT3D(ptIter->x, ptIter->y);
		if(pt3d1 == NULL)
		{
			continue;
		}

		sum += pow(pt3d1->z-this->_mean, 2.0);
		count++;
	}
	if(count != 0)
	{
		variance = sqrt(sum/count);
	}

	// cellZDiff statistic
	cellZDiff = 0.0;
	count = 0;
	for(ptIter = this->_pts.begin(); ptIter != this->_pts.end(); ++ptIter)
	{
		pt3d1 = gridIndex->getMinPT3D(ptIter->x, ptIter->y);
		pt3d2 = gridIndex->getMaxPT3D(ptIter->x, ptIter->y);
		if(pt3d1 == NULL)
		{
			continue;
		}

		cellZDiff += pt3d2->z - pt3d1->z;
		count++;
	}
	cellZDiff = cellZDiff/count;

	// boundingZDiff, boundingH and boundingVari
	boundingZDiff = 0;
	boundingH = 0;
	count = 0;
	for(ptIter = this->_bound.begin(); ptIter != this->_bound.end(); ++ptIter)
	{
		pt3d1 = gridIndex->getMinPT3D(ptIter->x, ptIter->y);
		pt3d2 = gridIndex->getMaxPT3D(ptIter->x, ptIter->y);
		if(pt3d1 == NULL)
		{
			continue;
		}

		boundingZDiff += pt3d2->z - pt3d1->z;
		boundingH += pt3d2->z;
		count++;
	}
	boundingZDiff = boundingZDiff / count;
	boundingH = boundingH / count;

	boundingHVari = 0;
	for(ptIter = this->_bound.begin(); ptIter != this->_bound.end(); ++ptIter)
	{
		pt3d2 = gridIndex->getMaxPT3D(ptIter->x, ptIter->y);
		if(pt3d2 == NULL)
		{
			continue;
		}
		boundingHVari += pow(pt3d2->z - boundingH, 2.0);
	}
	boundingHVari = sqrt(boundingHVari/count);

	// perimeter
	perimeter = this->_bound.size()*cellSize;
	perimeter = (perimeter + PI)/0.9;

	// MBR length, width, area, mbrPerimeter, perimeterAndMbrPerimeterRate, areaAndMbrAreaRate
	findMBR(mbrLength, mbrWidth, mbrArea, this->_bound);
	mbrLength = mbrLength * cellSize;
	mbrWidth = mbrWidth * cellSize;
	mbrPerimeter = 2*(mbrLength + mbrWidth);
	mbrArea = mbrArea * cellSize * cellSize;
	perimeterAndMbrPerimeterRate = perimeter/mbrPerimeter;
	areaAndMbrAreaRate = area/mbrArea;

	// compactness
	compactness = 4*PI*area/(perimeter*perimeter);

	// Put values to parameters map
	this->_parameters[areaTag] = area;
	this->_parameters[meanTag] = this->_mean;
	this->_parameters[varianceTag] = variance;
	this->_parameters[cellZDiffTag] = cellZDiff;
	this->_parameters[boundingZDiffTag] = boundingZDiff;
	this->_parameters[boundingHTag] = boundingH;
	this->_parameters[boundingHVariTag] = boundingHVari;
	this->_parameters[perimeterTag] = perimeter;
	this->_parameters[mbrLengthTag] = mbrLength;
	this->_parameters[mbrWidthTag] = mbrWidth;
	this->_parameters[mbrPerimeterTag] = mbrPerimeter;
	this->_parameters[perimeterAndMbrPerimeterRateTag] = perimeterAndMbrPerimeterRate;
	this->_parameters[mbrAreaTag] = mbrArea;
	this->_parameters[areaAndMbrAreaRateTag] = areaAndMbrAreaRate;
	this->_parameters[compactnessTag] = compactness;
	this->_parameters[topologyTag] = this->_topkeys.size();
}

/**
*
*/
void Entity::removeInnerPts() 
{
	this->_pts.clear();
}

/**
*
*/
int& Entity::getID()
{ 
	return this->_id; 
}

/**
*
*/
double& Entity::getMean()
{ 
	return this->_mean;
}

/**
*
*/
void Entity::setType(PointType type) 
{
	this->_type = type; 
}

/**
*
*/
PointType& Entity::getType()
{
	return this->_type;
}

/**
*
*/
int& Entity::getPtNum()
{ 
	return this->_ptNum;
}

/**
*
*/
vector<PT>& Entity::getAllPts()
{ 
	return this->_pts; 
}

/**
*
*/
vector<PT>& Entity::getBounding()
{ 
	return this->_bound; 
}

/**
*
*/
vector<PT>& Entity::getCurrentQueue()
{
	return this->_currQueue; 
}

/**
*
*/
PT& Entity::getCenterPos()
{
	return this->_centerPt;
}

/**
*
*/
map<int, int>& Entity::getTopKeys()
{ 
	return this->_topkeys; 
}

/**
*
*/
map<string, double>& Entity::getParameters()
{
	return this->_parameters;
}

/**
*
*/
PT& Entity::getSeedPt()
{
	return this->_seedPt; 
}

/**
* Get the 4 neighborhood points of given point pt
*   1                
* 0 pt 2
*   3
*/
void Entity::get4Neighbor(PT &pt,PT nhbor[4])
{
	nhbor[0].x = pt.x-1;
	nhbor[0].y = pt.y;
	nhbor[1].x = pt.x;
	nhbor[1].y = pt.y-1;
	nhbor[2].x = pt.x+1;
	nhbor[2].y = pt.y;
	nhbor[3].x = pt.x;
	nhbor[3].y = pt.y+1;
}

/**
* Get the 8 neighborhood points of given point pt
* 1  2  3               
* 0 pt  4
* 7  6  5
*/
void Entity::get8Neighbor(PT &pt, PT nhbor[8])
{
	nhbor[0].x = pt.x-1;
	nhbor[0].y = pt.y;
	nhbor[1].x = pt.x-1;
	nhbor[1].y = pt.y-1;
	nhbor[2].x = pt.x;
	nhbor[2].y = pt.y-1;
	nhbor[3].x = pt.x+1;
	nhbor[3].y = pt.y-1;
	nhbor[4].x = pt.x+1;
	nhbor[4].y = pt.y;
	nhbor[5].x = pt.x+1;
	nhbor[5].y = pt.y+1;
	nhbor[6].x = pt.x;
	nhbor[6].y = pt.y+1;
	nhbor[7].x = pt.x-1;
	nhbor[7].y = pt.y+1;
}

