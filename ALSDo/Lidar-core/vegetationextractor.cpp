#include "vegetationextractor.h"
#include "workspace.h"

#include <cmath>
#include <algorithm>
#include <cstdio>
#include "ogrsf_frmts.h"

static void get_hightdiff_angle(PT3D &p1, PT3D &p2, double &hdiff, double &angle)
{
	double dis = 0.0;
	hdiff = abs(p1.z - p2.z);
	dis = sqrt(pow(p1.x - p2.x, 2.0) + pow(p1.y - p2.y, 2.0));
	angle = atan(hdiff / dis);
}

VegetationExtractor::VegetationExtractor(WorkSpace *workSpace)
	: OBASegment(workSpace)
{
	this->_workSpace = workSpace;
}

VegetationExtractor::~VegetationExtractor()
{
	this->dispose();
}

void VegetationExtractor::segmentRegionBySeeds(const double &maxObjSize, const double &hIter, const double &aIter, const char* classid_path) 
{
	int ii = 0, count = 0, pos = 0, entityID = 0, validN = 0, acceptN = 0, prevN = 0;
	short *idM = NULL;
	vector<PT> seeds;
	PT pt;
	Entity* entity = NULL;
	PT3D *pt3d = NULL;
	bool *enter_queue = NULL;
	double hT = 0.0, angleT = 0.0;

	this->dispose();

	count = this->_gridH * this->_gridW;
	memset(idM, 0, count*sizeof(short));	
	FILE *fp = fopen(classid_path, "r");

	for(ii = 0; ii < count; ++ii)
	{
		fscanf(fp, "%d", &idM[ii]);
	}
	fclose(fp);

	/* Find maximum seeds */
	this->findSeeds(maxObjSize, seeds, idM);

	this->_entityIDs = new int[count];
	for(ii = 0; ii < count; ++ii)
	{
		this->_entityIDs[ii] = NAN;
	}
	enter_queue = new bool[count];
	memset(enter_queue, 0, count * sizeof(bool));

	for(ii = 0; ii < count; ++ii)
	{
		if(idM[ii] == 0)
		{
			continue;
		}
		validN++;
	}

	prevN = -1;
	while(acceptN != prevN)
	{
		hT += hIter;
		angleT += aIter;
		prevN = acceptN;

		if(acceptN == 0)
		{
			for (vector<PT>::iterator it = seeds.begin(); it != seeds.end(); ++it)
			{
				pt = *it;
				pos = pt.y * this->_gridW + pt.x;
				if(this->_entityIDs[pos] != NAN) 
				{
					continue;
				}

				entityID++;
				entity = new Entity(entityID, PointType::p_unclassified);
				entity->setSeedPoint(pt);

				this->_queue.push_back(pt);
				enter_queue[pos] = true;

				while(!this->isQueueEmpty()) 
				{
					pt = this->popQueue();
					pos = pt.y * this->_gridW + pt.x;
					pt3d = this->_gridIndex->getMaxPT3D(pt.x, pt.y);
					enter_queue[pos] = false;

					this->_entityIDs[pos] = entityID;
					entity->addPoint(pt, pt3d->z);
					this->enterQueue(entity, pt, hT, angleT, idM, enter_queue);

					acceptN++;
				}
		
				this->_entitys[entityID] = entity;
			}
		}
		else
		{
			for(hash_map<int, Entity*>::iterator it = this->_entitys.begin(); it != this->_entitys.end(); ++it)
			{
				entityID = it->first;
				entity = it->second;				
				seeds = entity->getCurrentQueue();
				for(vector<PT>::iterator it1 = seeds.begin(); it1 != seeds.end(); ++it1)
				{
					pt = *it1;
					pos = pt.y * this->_gridW + pt.x;
					enter_queue[pos] = true;
					this->_queue.push_back(pt);
				}

				entity->clearCurrQueue();

				while(!this->isQueueEmpty()) 
				{
					pt = this->popQueue();
					pos = pt.y * this->_gridW + pt.x;
					pt3d = this->_gridIndex->getMaxPT3D(pt.x, pt.y);
					enter_queue[pos] = false;

					this->_entityIDs[pos] = entityID;
					entity->addPoint(pt, pt3d->z);
					this->enterQueue(entity, pt, hT, angleT, idM, enter_queue);

					acceptN++;
				}

			}
		}
	}

	// Dispose local garbage
	if(enter_queue != NULL) 
	{
		delete []enter_queue;
		enter_queue = NULL;
	}
	if(idM != NULL)
	{
		delete []idM;
		idM = NULL;
	}
}

void VegetationExtractor::exportTrees(const char *topPath, const char *boundaryPath)
{
	GridIndex *gridIndex = NULL;
	int count = 0, e_type = 0;
	PT3D *pt3d = NULL;	
	Entity *entity = NULL;
	PT pt;
	FILE *fp = NULL;
	
	gridIndex = this->_workSpace->getGridIndex();
	fp = fopen(topPath, "w+");
	for(hash_map<int, Entity*>::iterator it = this->_entitys.begin(); it != this->_entitys.end(); ++it) 
	{
		count++;
		entity = it->second;
		pt = entity->getSeedPt();
		e_type = entity->getType();
		pt3d = gridIndex->getMaxPT3D(pt.x, pt.y);

		fprintf(fp, "%d %lf %lf %lf %d\n", count, pt3d->x, pt3d->y, pt3d->z, e_type);
	}
	fclose(fp);

	
	//this->sortBounding();

	//output the tree boundaries
	const char *pszDriverName = "ESRI Shapefile";
    OGRRegisterAll();
    OGRSFDriver *driver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(pszDriverName);
	OGRDataSource *ds = driver->CreateDataSource(boundaryPath, NULL );
    OGRLayer *layer = ds->CreateLayer("tree", NULL, wkbPolygon, NULL);
 
    OGRFieldDefn idField("id", OFTInteger);
    if(layer->CreateField(&idField) != OGRERR_NONE)
    {
       OGRDataSource::DestroyDataSource(ds);
	   return;
    }
	OGRFieldDefn typeField("type", OFTString);
	typeField.SetWidth(16);
    if(layer->CreateField(&typeField) != OGRERR_NONE)
    {
       OGRDataSource::DestroyDataSource(ds);
	   return;
    }

	vector<PT> bounding;
	int ii = 0, size = 0;

	for(hash_map<int, Entity*>::iterator it = this->_entitys.begin(); it != this->_entitys.end(); ++it) 
	{
		Entity *entity = it->second;
		bounding = entity->getBounding();

		OGRFeature feature(layer->GetLayerDefn());
		feature.SetField("id", count);
		if(entity->getType() == PointType::p_tree_conifer)
		{
			feature.SetField("type", "conifer");
		}
		else if(entity->getType() == PointType::p_tree_deciduous)
		{
			feature.SetField("type", "deciduous");
		}
		else
		{
			feature.SetField("type", "unknown");
		}

		OGRLinearRing ring;	
		size = bounding.size();
		for(ii = 0; ii < size; ++ii)
		{
			pt = bounding[ii];
			pt3d = gridIndex->getMaxPT3D(pt.x, pt.y);
			ring.addPoint(pt3d->x, pt3d->y);
		}

		OGRPolygon polygon;
		polygon.addRing(&ring);
		feature.SetGeometry(&polygon); 

		if(layer->CreateFeature(&feature) != OGRERR_NONE)
        {
			OGRDataSource::DestroyDataSource(ds);
			return;
        }

		count++;
	}

	OGRDataSource::DestroyDataSource(ds);	
}

void VegetationExtractor::findSeeds(const double &maxObjSize, std::vector<PT> &seeds, const short *idM)
{
	GridIndex *gridIndex = NULL;
	int gridW = 0, gridH = 0, regionWN = 0, regionHN = 0, h = 0, w = 0, pos = 0, xx = 0, yy = 0;
	double rSize = 0.0, *ZMax = NULL;
	PT3D *pt3d = NULL;
	PT *pt = NULL;

	seeds.clear();
	gridIndex = this->_workSpace->getGridIndex();
	gridW = gridIndex->getGridWidth();
	gridH = gridIndex->getGridHeight();
	rSize = maxObjSize / gridIndex->getCellSize();

	regionWN = int(gridW / rSize) + 1;
	regionHN = int(gridH / rSize) + 1;
	ZMax = new double[regionHN * regionWN];
	pt = new PT[regionHN * regionWN];

	for(h = 0; h < regionHN; ++h)
	{
		for(w = 0; w < regionWN; ++w) 
		{
			pos = h * regionWN + w;
			ZMax[pos] = MINV;
			pt[pos].x = -1;
			pt[pos].y = -1;
		}
	}	

	for(h = 0; h < gridH; ++h) 
	{
		for(w = 0; w < gridW; ++w) 
		{
			pos = h * gridW + w;
			pt3d = gridIndex->getMaxPT3D(w, h);
			if(pt3d == NULL)
			{
				continue;
			}
			if(idM[pos] != PointType::p_high_vegetation)
			{
				continue;
			}

			xx = int(pt3d->x/ rSize);
			yy = int(pt3d->y/ rSize);
			pos = yy * regionWN + xx;		
			if (pt3d->z > ZMax[pos])
			{
				ZMax[pos] = pt3d->z;
				pt[pos].x = w;
				pt[pos].y = h;
			}
		}
	}

	for(h = 0; h < regionHN; ++h)
	{
		for(w = 0; w < regionWN; ++w) 
		{
			pos = h * regionWN + w;
			if(pt[pos].x == -1)
			{
				continue;
			}
			
			seeds.push_back(pt[pos]);
		}
	}

#if LOG_OUT
	string seedPath = string(this->_workSpace->getResFolder()) + "tree_seeds.pt"; 
	FILE *fp = fopen(seedPath.c_str(), "w+");
	for (vector<PT>::iterator it = seeds.begin(); it != seeds.end(); ++it)
	{
		fprintf(fp, "%d %d\n", it->x, it->y);
	}
	fflush(fp);
	fclose(fp);
#endif

	if (ZMax != NULL)
	{
		delete []ZMax;
		ZMax = NULL;
	}
	if (pt != NULL)
	{
		delete []pt;
		pt = NULL;
	}
}

void VegetationExtractor::enterQueue(Entity *entity, PT &pt, const double &hT, const double &angleT, const short *idM, bool *flags)
{
	GridIndex *gridIndex = NULL;
	PT nhbor[4];
	int gridW = 0, id1 = 0, id2 = 0, pos = 0, ii = 0;
	double hdiff = 0.0, angle = 0.0;
	PT3D *pt3d1 = NULL, *pt3d2 = NULL;

	gridIndex = this->_workSpace->getGridIndex();
	gridW = gridIndex->getGridWidth();
	pt3d1 = gridIndex->getMaxPT3D(pt.x, pt.y);
	for(ii = 0; ii < 4; ++ii) 
	{
		pt3d2 = gridIndex->getMaxPT3D(nhbor[ii].x, nhbor[ii].y);
		if(pt3d2 == NULL) 
		{
			continue;
		}

		pos = nhbor[ii].y * gridW + nhbor[ii].x;
		if(flags[pos]) 
		{
			continue;
		}
		if(this->_entityIDs[pos] != NAN)
		{
			continue;		
		}

		get_hightdiff_angle(*pt3d1, *pt3d2, hdiff, angle);
		if(hdiff <= hT && angle <= angleT)
		{
			this->_queue.push_back(nhbor[ii]);
			flags[pos] = true;
		}
		else
		{
			//it maybe can used later...
			entity->addCurrQueue(nhbor[ii]);
			flags[pos] = true;
		}
	}	
}

void VegetationExtractor::dispose()
{
	OBASegment::dispose();
}

