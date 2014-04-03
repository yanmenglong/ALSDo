#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;
#include <io.h>

#include "ogrsf_frmts.h"

#include "lasio.h"
#include "lassplitor.h"
#include "util.h"
#include "workspace.h"
#include "obafilter.h"

static void text2las(int argc, const char* argv[])
{
	if (argc != 3)
	{
		printf("wrong parameters!\n");
		return;
	}
	LasIO::text2las(argv[1], argv[2]);
}

static void las2text(int argc, const char* argv[]) 
{
	if (argc != 3)
	{
		printf("wrong parameters!\n");
		return;
	}

	LasIO::las2text(argv[1], argv[2]);
}

static void test_ogr()
{
	//output the tree boundaries
	const char *pszDriverName = "ESRI Shapefile", *shpPath = "f:/test.shp";
	OGRSFDriver *driver = NULL;
	OGRDataSource *ds = NULL;
	OGRLayer *layer = NULL;
	int ii = 0;
	double x = 0, y = 0;

    OGRRegisterAll();
    driver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(pszDriverName);
	if((_access(shpPath, 0)) != -1)
	{
		if(driver->DeleteDataSource(shpPath) != OGRERR_NONE)
		{
			return;
		}
	}

	ds = driver->CreateDataSource(shpPath, NULL );	
    layer = ds->CreateLayer("test", NULL, wkbPolygon, NULL);
 
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

	OGRFeature feature(layer->GetLayerDefn());
	feature.SetField("id", 1);
	feature.SetField("type", "test");
	OGRLinearRing ring;	
	for(ii = 0; ii < 4; ++ii)
	{
		x = rand() * 1.0;
		y = rand() * 1.0;
		ring.addPoint(x, y);
	}
	OGRPolygon polygon;
	polygon.addRing(&ring);
	feature.SetGeometry(&polygon); 
	if(layer->CreateFeature(&feature) != OGRERR_NONE)
	{
		OGRDataSource::DestroyDataSource(ds);
		return;
	}
	OGRDataSource::DestroyDataSource(ds);	
}

static void statistic_LV()
{
	double cellSize = 1.0, maxObjSize = 100, hT = 0.0, dangle = 0.0, angleT = 0.0, variance = 0.0, varianceAver = 0.0, count = 0.0;
	const char *lasPath = "e:/data/lidar/test/clipregion.las", *resultDir = "e:/data/lidar/test/clipregion/";
	FILE *fp = fopen("c:/scale.txt", "w+"); 
	WorkSpace *workspace = new WorkSpace();
	hash_map<int, Entity*> *entitys = NULL;
	Entity *entity = NULL;

	workspace->newProj(lasPath, cellSize, resultDir);
	OBAFilter *obaFilter = workspace->getOBAFilter();	

	fprintf(fp, "angle\t objectN\t varianceAver\n");
	for(dangle = 5.0; dangle <= 60.0; dangle += 5.0) 
	{
		hT = 5.0;
		angleT = PI * dangle/ 180.0;
		obaFilter->segmentRegion(hT, angleT);
		obaFilter->findBounding();	
		obaFilter->findTop();
		obaFilter->removeHole(5);
		//obaFilter->removeEntity(20);
		obaFilter->findBounding();
		obaFilter->statisticEntityInfo(NAN);

		entitys = obaFilter->getEntitys();
		varianceAver = 0.0;
		count = 0.0;
		for(hash_map<int, Entity*>::iterator it = entitys->begin(); it != entitys->end(); ++it)
		{
			entity = it->second;
			if(entity->getType() == PointType::p_created)
			{
				continue;
			}

			variance = entity->getParameters()[varianceTag];
			varianceAver += variance;
			count++;
		}	
		varianceAver = varianceAver / count;

		fprintf(fp, "%lf  %lf  %lf\n", dangle, count, varianceAver);
		fflush(fp);

		printf("%lf is finished!\n", dangle);
	}	
	fclose(fp);
}


#include <math.h>
int main(int argc, const char *argv[])
{
	/*
	clipRegion("H:\\data\\lidar\\plant\\bottom\\48385002.las", 
		"H:\\data\\lidar\\plant\\bottom\\48385002_part1.las", 
		20296,
		375640,
		21288,
		376327);

	clipRegion("H:\\data\\lidar\\plant\\bottom\\48385002.las", 
		"H:\\data\\lidar\\plant\\bottom\\48385002_part2.las", 
		21795,
		375148,
		22494,
		375692);
		*/

	/*
	clipRegion("H:\\data\\lidar\\plant\\first\\48385002\\ground.las", 
		"H:\\data\\lidar\\plant\\first\\48385002\\ground_part1.las", 
		20296,
		375640,
		21288,
		376327);

	clipRegion("H:\\data\\lidar\\plant\\first\\48385002\\ground.las", 
		"H:\\data\\lidar\\plant\\first\\48385002\\ground_part2.las", 
		21795,
		375148,
		22494,
		375692);
		*/

	std::vector<const char *> las_path_l;
	las_path_l.push_back("H:\\yyyyyyy\\yml\\48385002_b_part2.las");
	las_path_l.push_back("H:\\yyyyyyy\\yml\\48385002_part2.las");


	mergePoints(las_path_l, 
		"H:\\yyyyyyy\\yml\\48385002_site2.las"
		);

	return 0;	
}