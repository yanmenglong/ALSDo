#include "workspace.h"
#include "ogrsf_frmts.h"

#include "gridindex.h"
#include "obafilter.h"
#include "obaclassifier.h"
#include "vegetationextractor.h"

#include <cmath>
#include <algorithm>

/***************************************************************************************
* This function is used to normalize the z value to [0, 255] for bitmap demonstration
* width: matrix width
* height: maxtrix height
* zV: original z value
* z: normalized z value
***************************************************************************************/
static void z_enhance(const int &width, const int &height, const double *zV, unsigned char* z) 
{
	int w = 0, h = 0, pos = 0;
	double zmin = 999999999.0, zmax = -999999999, k = 0.0, b = 0.0;
	for(h = 0; h < height; ++h)
	{
		for(w = 0; w < width; ++w)
		{
			pos = h * width + w;
			if(zV[pos] == 0)
			{
				continue;
			}
			zmin = zV[pos] < zmin ? zV[pos] : zmin;
			zmax = zV[pos] > zmax ? zV[pos] : zmax;
		}
	}
	k = 255.0/(zmax - zmin);
	b = -k * zmin;

	for(h = 0; h < height; ++h)
	{
		for(w = 0; w < width; ++w)
		{
			pos = h * width + w;
			if(zV[pos] == 0)
			{
				continue;
			}

			z[pos] = (unsigned char)(k * zV[pos] + b);
		}
	}
}

/**
*
*/
WorkSpace::WorkSpace()
{
	this->_gridIndex = NULL;
	this->_zValue = NULL;
		
	this->_obaFilter = NULL;
	this->_obaClassifier = NULL;
	this->_vegetationExtractor = NULL;
}

/**
*
*/
WorkSpace::~WorkSpace()
{
	this->dispose();
}

/**
*Create a new project
*/
void WorkSpace::newProj(const char *lasPath, const double &cellSize, const char *resultDir)
{
	int ii = 0, N = 0;
	//First set the value of paths
	this->_lasPath = string(lasPath);
	this->_resFolder = string(resultDir);
	if(this->_resFolder.at(this->_resFolder.length()-1) != '/')
	{
		this->_resFolder = this->_resFolder + string("/");
	}

	//dispose objects if it is not NULL
	this->dispose();
	this->_gridIndex = new GridIndex(this, cellSize);
}

/**
*
*/
unsigned char* WorkSpace::resampleZ()
{
	int iy = 0, ix = 0, gridH = 0, gridW = 0, pos = 0;
	PT3D *pt3d = NULL;
	double *zV = NULL;

	gridH = this->_gridIndex->getGridHeight();
	gridW = this->_gridIndex->getGridWidth();

	if(this->_zValue != NULL)
	{
		return this->_zValue;
	}
	this->_zValue = new unsigned char[gridH * gridW];
	memset(this->_zValue, 0, gridH * gridW * sizeof(unsigned char));

	zV = new double[gridH * gridW];
	memset(zV, 0, gridH * gridW * sizeof(double));
	for(iy = 0; iy < gridH; ++iy) 
	{
		for(ix = 0; ix < gridW; ++ix) 
		{
			pos = iy * gridW + ix;
			pt3d = this->_gridIndex->getMinPT3D(ix, iy);
			if(pt3d == NULL)
			{
				continue;
			}
			zV[pos] = pt3d->z;
		}
	}
	z_enhance(gridW, gridH, zV, this->_zValue);

	delete[] zV;
	zV = NULL;
	return this->_zValue;
}

/**
*
*/
const char* WorkSpace::getResFolder() 
{ 
	return this->_resFolder.c_str(); 
}

/**
*
*/
const char* WorkSpace::getLasPath() 
{ 
	return this->_lasPath.c_str();
}

/**
*
*/
GridIndex* WorkSpace::getGridIndex()
{ 
	return this->_gridIndex;
}

/**
*
*/
OBAFilter* WorkSpace::getOBAFilter()
{
	if(this->_obaFilter == NULL)
	{
		this->_obaFilter = new OBAFilter(this);
	}
	return this->_obaFilter; 
}

/**
*
*/
OBAClassifier *WorkSpace::getOBAClassifier()
{
	if(this->_obaClassifier == NULL)
	{
		this->_obaClassifier = new OBAClassifier(this);
	}
	return this->_obaClassifier; 
}

/**
*
*/
VegetationExtractor* WorkSpace::getVegetationExtractor()
{
	if(this->_vegetationExtractor == NULL)
	{
		this->_vegetationExtractor = new VegetationExtractor(this);
	}
	return this->_vegetationExtractor;
}

/**
*
*/
void WorkSpace::dispose()
{
	if(this->_gridIndex != NULL) 
	{
		delete _gridIndex;
		this->_gridIndex = NULL;
	}
	if(this->_zValue != NULL) 
	{
		delete []this->_zValue;
		this->_zValue = NULL;
	}
	if(this->_obaFilter != NULL) 
	{
		delete this->_obaFilter;
		this->_obaFilter = NULL;
	}
	if(this->_obaClassifier != NULL) 
	{
		delete this->_obaClassifier;
		this->_obaClassifier = NULL;
	}	
	if(this->_vegetationExtractor != NULL) 
	{
		delete this->_vegetationExtractor;
		this->_vegetationExtractor = NULL;
	}
}