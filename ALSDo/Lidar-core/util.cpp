#include "util.h"
#include "tin.h"
#include "lasio.h"
#include "workspace.h"
#include "gridindex.h"
#include "obasegment.h"
#include "obaclassifier.h"

#include "gdal_priv.h"
#include "ogrsf_frmts.h"

#include <cmath>
#include <io.h>

//判断点与三角形关系的另一做法
//S1 = ( y - y1 ) ( x2 - x1 ) - ( y2 - y1 ) ( x - x1 )
//S2 = ( y - y2 ) ( x3 - x2 ) - ( y3 - y2 ) ( x - x2 )
//S3 = ( y - y3 ) ( x1 - x3 ) - ( y1 - y3 ) ( x - x3 )
/*
static bool onedge(double *pa, double *pb, double *des)
{
	if(pa[0] == des[0] && pb[0] == des[0])
	{
		return true;
	}
	if(pa[0] == des[0] && pb[0] != des[0])
	{
		return false;
	}
	if(pa[0] != des[0] && pb[0] == des[0])
	{
		return false;
	}

	double k1 = (des[1] - pa[1])/(des[0] - pa[0]);
	double k2 = (des[1] - pb[1])/(des[0] - pb[0]);
	if(k1 == k2)
	{
		return true;
	}
	return false;
}

static bool ontriangle(double *pa, double *pb, double *pc, double *des)
{
	return onedge(pa, pb, des) || onedge(pb, pc, des) || onedge(pa, pc, des);
}
*/

static bool intriangle(double *pa, double *pb, double *pc, double *des) 
{
	double S1 = 0.0, S2 = 0.0, S3 = 0.0;
	S1 = (des[1] - pa[1])*(pb[0] - pa[0]) - (pb[1] - pa[1])*(des[0] - pa[0]);
	S2 = (des[1] - pb[1])*(pc[0] - pb[0]) - (pc[1] - pb[1])*(des[0] - pb[0]);
	S3 = (des[1] - pc[1])*(pa[0] - pc[0]) - (pa[1] - pc[1])*(des[0] - pc[0]);

	if(S1 < 0 || S2 < 0 || S3 < 0)
	{
		return false;
	}
	return true;
}

static void raster_triangle(triangle* t, const double &dtmFactor, const int &rW, const int &rH, float *rasterZ)
{
	double xl = min(min(t->V[0][0], t->V[1][0]), t->V[2][0]);
	double xh = max(max(t->V[0][0], t->V[1][0]), t->V[2][0]);
	double yl = min(min(t->V[0][1], t->V[1][1]), t->V[2][1]);
	double yh = max(max(t->V[0][1], t->V[1][1]), t->V[2][1]);
	double x = 0.0, y = 0.0, desp[2], dis_a = 0.0, dis_b = 0.0, dis_c;
	double ac = 0.0, ab = 0.0, bc = 0.0;
	int ix = 0, iy = 0;
	
	for(y = yl; y <= yh; y += dtmFactor)
	{
		for(x = xl; x <= xh; x+= dtmFactor) 
		{
			desp[0] = x;
			desp[1] = y;
			ix = int(x/dtmFactor);
			iy = int(y/dtmFactor);

			/*
			if(z[iy*rW + ix] == NAN)
			{
				z[iy*rW + ix] = TAG_Z;
			}
			*/
			if(desp[0] == t->V[0][0] && desp[1] == t->V[0][1]) 
			{
				rasterZ[(rH-1-iy)*rW + ix] = t->V[0][2];
				continue;
			}
			if(desp[0] == t->V[1][0] && desp[1] == t->V[1][1]) 
			{
				rasterZ[(rH-1-iy)*rW + ix] = t->V[1][2];
				continue;
			}
			if(desp[0] == t->V[2][0] && desp[1] == t->V[2][1]) 
			{
				rasterZ[(rH-1-iy)*rW + ix] = t->V[2][2];
				continue;
			}

			if(intriangle(t->V[0], t->V[1], t->V[2], desp))
			{
				dis_a = pow(t->V[0][0]-desp[0], 2.0) + pow(t->V[0][1]-desp[1], 2.0) + 0.001;
				dis_b = pow(t->V[1][0]-desp[0], 2.0) + pow(t->V[1][1]-desp[1], 2.0) + 0.001;
				dis_c = pow(t->V[2][0]-desp[0], 2.0) + pow(t->V[2][1]-desp[1], 2.0) + 0.001;
				bc = dis_b * dis_c;
				ac = dis_a * dis_c;
				ab = dis_a * dis_b;
				rasterZ[(rH-1-iy)*rW + ix] = (bc*t->V[0][2] + ac*t->V[1][2] + ab*t->V[2][2])/(bc + ac + ab);	
			}
		}
	}
}

static void statistic_raster(float* const rasterZ, const int &count, double &zMin, double &zMax, double &zMean, double &zDev)
{
	int ii = 0, validN = 0;
	zMin = MAXV;
	zMax = MINV;	
	zMean = 0.0;
	zDev = 0.0;

	for(ii = 0; ii < count; ++ii)
	{
		if(rasterZ[ii] == NAN)
		{
			continue;
		}

		zMin = min(zMin, (double)rasterZ[ii]);
		zMax = max(zMax, (double)rasterZ[ii]);		
		zMean += rasterZ[ii];
		validN++;
	}
	zMean = zMean/validN;

	zDev = 0;
	for(ii = 0; ii < count; ++ii)
	{
		if(rasterZ[ii] == NAN)
		{
			continue;
		}

		zDev += pow(rasterZ[ii] - zMean, 2.0);
	}
	zDev = sqrt(zDev/validN);
}

/**
* Identify the complexity of scene by object based analysis
*/
EXPORT void calcComplexByObj(WorkSpace *workSpace, const double &maxObjSize, const double &seedHeightTh, const double &seedSlopeTh)
{
	int gridH = 0, gridW = 0, regionSize = 0, regionH = 0, regionW = 0, *regionAreaSum = NULL, ih = 0, iw = 0, pos = 0;
	GridIndex *gridIndex = workSpace->getGridIndex();
	
	gridH = gridIndex->getGridHeight();
	gridW = gridIndex->getGridWidth();
	regionSize = maxObjSize / gridIndex->getCellSize();
	regionH = int(gridH/regionSize)+1;
	regionW = int(gridW/regionSize)+1;
	
	regionAreaSum = new int[regionH * regionW];
	memset(regionAreaSum, 0, regionH*regionW*sizeof(int));

	OBASegment *obaSegment = new OBASegment(workSpace);
	obaSegment->segmentRegion(seedHeightTh, seedSlopeTh);
	obaSegment->findBounding();	
	obaSegment->findTop();
	obaSegment->removeHole(5);

	hash_map<int, Entity*>* objs = obaSegment->getEntitys();
	Entity *obj = NULL;
	PT pt;

	for(hash_map<int, Entity*>::iterator it = objs->begin(); it != objs->end(); ++it)
	{
		obj = it->second;
		if(obj->getPtNum() < 10)
		{
			pt = obj->getCenterPos();
			ih = int(pt.y/regionSize);
			iw = int(pt.x/regionSize);
			pos = ih*regionW+iw;
			regionAreaSum[pos] += obj->getPtNum();
		}		
	}

	std::string specialRectPath = std::string(workSpace->getResFolder()) + "special_region.rect";
	FILE *fp = fopen(specialRectPath.c_str(), "w+");
	fprintf(fp, "%lf\n", maxObjSize);

	double areaTh = 0.6 * regionSize * regionSize; 
	for(ih = 0; ih < regionH; ++ih)
	{
		for(iw = 0; iw < regionW; ++iw)
		{
			pos = ih*regionW + iw;
			if(regionAreaSum[pos] >= areaTh)
			{
				fprintf(fp, "%d %d\n", iw, ih);	
			}
			fflush(fp);
		}
	}
	fclose(fp);	

	if(regionAreaSum != NULL)
	{
		delete []regionAreaSum;
		regionAreaSum = NULL;
	}
	if(obaSegment != NULL)
	{
		delete obaSegment;
		obaSegment = NULL;
	}
}

/**
* Identify the complexity of scene by local edge statistic
*/
EXPORT void calcComplexByLocalEdge(WorkSpace *workSpace, const double &hT, const double &maxObjSize)
{
	int gridW = 0, gridH = 0, regionSize = 0, regionW = 0, regionH = 0, *regionEdgeSum = NULL, hh = 0, ww = 0, ih = 0, iw = 0, pos = 0;
	GridIndex *gridIndex = NULL;
	bool *localEdge = NULL;

	gridIndex = workSpace->getGridIndex();
	gridW = gridIndex->getGridWidth();
	gridH = gridIndex->getGridHeight();
	regionSize = maxObjSize / gridIndex->getCellSize();

	localEdge = new bool[gridW*gridH];
	memset(localEdge, 0, gridW*gridH*sizeof(bool));	
	
	regionW = int(gridW/regionSize)+1;
	regionH = int(gridH/regionSize)+1;	
	regionEdgeSum = new int[regionW*regionH];
	memset(regionEdgeSum, 0, regionW*regionH*sizeof(int));

	// Get local edge
	getLocalEdge(workSpace, 1.0, localEdge);

	// Statistic the edge point
	for(hh = 0; hh < gridH; ++hh)
	{
		for(ww = 0; ww < gridW; ++ww)
		{
			if(!localEdge[hh*gridW+ww])
			{
				continue;
			}

			ih = int(hh/regionSize);
			iw = int(ww/regionSize);
			pos = ih*regionW+iw;
			regionEdgeSum[pos]++;
		}
	}

	// Output the special region
	std::string specialRectPath = std::string(workSpace->getResFolder()) + "special_region.rect";
	FILE *fp = fopen(specialRectPath.c_str(), "w+");
	fprintf(fp, "%lf\n", maxObjSize);

	int edgeTh = 0.2 * regionSize * regionSize; 
	for(ih = 0; ih < regionH; ++ih)
	{
		for(iw = 0; iw < regionW; ++iw)
		{
			pos = ih*regionW + iw;
			if(regionEdgeSum[pos] >= edgeTh)
			{
				fprintf(fp, "%d %d\n", iw, ih);	
			}
			fflush(fp);
		}
	}
	fclose(fp);	

	if(localEdge != NULL)
	{
		delete []localEdge;
		localEdge = NULL;
	}
	if(regionEdgeSum != NULL)
	{
		delete []regionEdgeSum;
		regionEdgeSum = NULL;
	}
}

/**
* Filter the left points by grid ground tag
*
* workSpace: the project work space
* hT: the height threshold
* angleT: the angle threshold
* groundTag: the tag identify whether the cell is ground cell
*/
EXPORT void filterPtsByGrid(WorkSpace *workSpace, const double &filteringHeightTh, const double &filteringSlopeTh, const bool *groundTag)
{
	GridIndex *gridIndex = NULL;
	LasIO *oriLASIO = NULL, *groundLASIO = NULL, *nongroundLASIO = NULL;
	LASHeader *oriLASHeader = NULL, *lasHeader = NULL;
	LASPoint *lasPts = NULL, *lasPt = NULL;
	int ptN = 0, ii = 0, gridW = 0, ix = 0, iy = 0, pos = 0;
	double cellSize = 0.0, hDiff = 0.0, aDiff = 0.0;
	PT3D * pt3d1 = NULL, pt3d2;
	std::string resFolder(workSpace->getResFolder());

	gridIndex = workSpace->getGridIndex();
	gridW = gridIndex->getGridWidth();
	oriLASHeader = gridIndex->getLASHeader();
	cellSize = gridIndex->getCellSize();

	oriLASIO = new LasIO(string(workSpace->getLasPath()));
	lasHeader = oriLASIO->open();
	if(lasHeader == NULL)
	{
		return;
	}

	lasPts = oriLASIO->readAllPts();
	ptN = lasHeader->records_number;
	// Filtering points by groundTag
	for(ii = 0; ii < ptN; ++ii)
	{
		// First reset point
		lasPt = &lasPts[ii];
		lasPt->class_id = PointType::p_unclassified;
		pt3d2.x = lasPt->x - oriLASHeader->xMin;
		pt3d2.y = lasPt->y - oriLASHeader->yMin;
		pt3d2.z = lasPt->z;
		ix = int(pt3d2.x/cellSize);
		iy = int(pt3d2.y/cellSize);
		pos = iy * gridW + ix;

		// The related pos is ground cell
		if(groundTag[pos]) 
		{
			pt3d1 = gridIndex->getMinPT3D(ix, iy);
			getHeightDiffandAngleDiff(*pt3d1, pt3d2, hDiff, aDiff);
			if(hDiff > filteringHeightTh || aDiff > filteringSlopeTh)
			{
				lasPt->class_id = PointType::p_nonground;
			}
			else 
			{
				lasPt->class_id = PointType::p_ground;
			}

		} 
		else 
		{
			lasPt->class_id = PointType::p_nonground;
		}
	} 

	groundLASIO = new LasIO(resFolder + "ground.las");
	groundLASIO->open(true);
	groundLASIO->writePts(lasPts, ptN, PointType::p_ground);
	groundLASIO->close();

	nongroundLASIO = new LasIO(resFolder + "tmp.las");
	nongroundLASIO->open(true);
	nongroundLASIO->writePts(lasPts, ptN, PointType::p_nonground);
	nongroundLASIO->close();

	// Dispose garbage
	if(oriLASIO != NULL)
	{
		delete oriLASIO;
		oriLASIO = NULL;
	}
	if(groundLASIO != NULL)
	{
		delete groundLASIO;
		groundLASIO = NULL;
	}
	if(nongroundLASIO != NULL)
	{
		delete nongroundLASIO;
		nongroundLASIO = NULL;
	}
	if(lasPts != NULL)
	{
		delete []lasPts;
		lasPts = NULL;
	}
}

/**
* Extract object points with given tolenrance
*/
EXPORT int extractObjectPoints(WorkSpace *workSpace, const double &dtmFactor, const double &objHT)
{
	string resultFolder, inLASPath, dtmPath, outLASPath;
	LasIO *inLASIO = NULL, *outLASIO = NULL;
	LASHeader *lasHeader = NULL;	
	LASPoint *lasPts = NULL, *lasPt = NULL;
	int lasPtN = 0, ii = 0, x = 0, y = 0, rW = 0, rH = 0, pos = 0;
	GDALDataset *inDS = NULL;
	float *rZ = NULL, rasterZ = 0.0;
	double geoTf[6];

	resultFolder = workSpace->getResFolder();
	inLASPath = resultFolder + "tmp.las";
	dtmPath = resultFolder + "ground.img";
	outLASPath = resultFolder + "nonground.las";

	inLASIO = new LasIO(inLASPath);
	lasHeader = inLASIO->open();
	lasPts = inLASIO->readAllPts();
	inLASIO->close();
	lasPtN = lasHeader->records_number;	

	GDALAllRegister();
	inDS = (GDALDataset *)GDALOpen(dtmPath.c_str(), GA_ReadOnly);
	inDS->GetGeoTransform(geoTf);
	GDALRasterBand *poBand = inDS->GetRasterBand(1);
	rW = poBand->GetXSize();
	rH = poBand->GetYSize();
	rZ = new float[rW * rH];
	poBand->RasterIO(GF_Read, 0, 0, rW, rH, rZ, rW, rH, GDT_Float32, 0, 0); 	
	GDALClose(inDS);

	for(ii = 0; ii < lasPtN; ++ii)
	{
		lasPt = &lasPts[ii];
		x = int((lasPt->x - geoTf[0])/geoTf[1] + 0.5);
		y = int((lasPt->y - geoTf[3])/geoTf[5] + 0.5);
		if(x < 0 || x >= rW || y < 0 || y >= rH)
		{
			continue;
		}

		pos = y * rW + x;
		rasterZ = rZ[pos];
		if(abs(lasPt->z - rasterZ) >= objHT)
		{
			lasPt->class_id = PointType::p_nonground;
		}
		else 
		{
			lasPt->class_id = PointType::p_unclassified;
		}
	}

	outLASIO = new LasIO(outLASPath);
	outLASIO->open(true);
	outLASIO->writePts(lasPts, lasPtN, PointType::p_nonground);	
	outLASIO->close();
	
	if (inLASIO != NULL)
	{
		delete inLASIO;
		inLASIO = NULL;
	}
	if (outLASIO != NULL)
	{
		delete outLASIO;
		outLASIO = NULL;
	}
	if (lasPts != NULL)
	{
		delete []lasPts;
		lasPts = NULL;
	}
	if(rZ != NULL)
	{
		delete []rZ;
		rZ = NULL;
	}
	return 0;	
}


/**
* Extract building LAS points
*/
EXPORT int exportBuildingLASPoints(WorkSpace *workSpace)
{
	GridIndex *gridIndex = NULL;
	OBAClassifier *obaClassifier = NULL;	
	int gridW = 0, gridH = 0, w = 0, h = 0, ii = 0, lasPtN = 0, ix = 0, iy = 0, pos = 0;
	double cellSize = 0.0;
	bool *type = NULL;
	hash_map<int, Entity*> *objs = NULL;
	hash_map<int, Entity*>::iterator iter;
	Entity* entity = NULL;
	vector<PT> pts;
	PT pt;
	LasIO *inLasIO = NULL, *outLasIO = NULL;
	LASHeader *lasHeader = NULL;
	LASPoint *lasPts = NULL, *lasPt = NULL;
	string buildingPath;
	
	gridIndex = workSpace->getGridIndex();
	obaClassifier = workSpace->getOBAClassifier();
	gridW = gridIndex->getGridWidth();
	gridH = gridIndex->getGridHeight();
	cellSize = gridIndex->getCellSize();
	type = new bool[gridW * gridH];
	memset(type, 0, gridW * gridH * sizeof(bool));

	objs = obaClassifier->getEntitys();
	for(iter = objs->begin(); iter != objs->end(); ++iter) 
	{
		entity = iter->second;
		if(entity->getType() != PointType::p_building)
		{
			continue;
		}

		pts = entity->getAllPts();
		for(ii = 0; ii < pts.size(); ++ii) 
		{
			pt = pts[ii];
			pos = pt.y * gridW + pt.x;
			type[pos] = true;
		}		
	}

	// Read the original points
	inLasIO = new LasIO(workSpace->getLasPath());
	lasHeader = inLasIO->open();
	lasPts = inLasIO->readAllPts();
	lasPtN = lasHeader->records_number;
	for(ii = 0; ii < lasPtN; ++ii)
	{
		lasPt = &lasPts[ii];
		ix = (lasPt->x-lasHeader->xMin)/cellSize;
		iy = (lasPt->y-lasHeader->yMin)/cellSize;
		pos = iy * gridW + ix;
		if(!type[pos])
		{
			continue;
		}

		lasPt->class_id = PointType::p_building;
	}

	buildingPath = std::string(workSpace->getResFolder()) + "building.las";
	outLasIO = new LasIO(buildingPath);
	outLasIO->open(true);
	outLasIO->writePts(lasPts, lasPtN, PointType::p_building);	
	outLasIO->close();

	/*
	string typePath = folder + "type.id";
	FILE *fp = fopen(typePath.c_str(), "w+");
	for (h = 0; h < gridH; ++h)
	{
		for (w = 0; w < gridW; ++w)
		{
			pos = h * gridW + w;
			fprintf(fp, "%d ", type[pos]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
	fflush(fp);
	*/

	if(type != NULL)
	{
		delete []type;
		type = NULL;
	}
	if(inLasIO != NULL)
	{
		delete inLasIO;
		inLasIO = NULL;
	}
	if(outLasIO != NULL)
	{
		delete outLasIO;
		outLasIO = NULL;
	}	
	if(lasPts != NULL)
	{
		delete []lasPts;
		lasPts = NULL;
	}	
	return 0;
}

/**
* Extract building shapefile boundary polygons
*/
EXPORT int exportBuildingPolygons(WorkSpace *workSpace)
{
	GridIndex *gridIndex = NULL;
	LASHeader *lasHeader = NULL;
	OBAClassifier *obaClassifier = NULL;	
	string buildingPath1, buildingPath2;
	OGRSFDriver *driver = NULL;
	OGRDataSource *ds1, *ds2 = NULL;
	OGRLayer *layer1, *layer2 = NULL;
	hash_map<int, Entity*> *objs = NULL;
	hash_map<int, Entity*>::iterator objIter;
	Entity* entity = NULL;
	vector<PT> pts;
	vector<PT>::iterator ptIter;
	PT3D *pt3d;

	gridIndex = workSpace->getGridIndex();
	lasHeader = gridIndex->getLASHeader();
	obaClassifier = workSpace->getOBAClassifier();
	buildingPath1 = std::string(workSpace->getResFolder()) + "building_point.shp";
	buildingPath2 = std::string(workSpace->getResFolder()) + "building_polygon.shp";

	OGRRegisterAll();
    driver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName("ESRI Shapefile");
	if((_access(buildingPath1.c_str(), 0)) != -1)
	{
		if(driver->DeleteDataSource(buildingPath1.c_str()) != OGRERR_NONE)
		{
			return -1;
		}
	}
	if((_access(buildingPath2.c_str(), 0)) != -1)
	{
		if(driver->DeleteDataSource(buildingPath2.c_str()) != OGRERR_NONE)
		{
			return -1;
		}
	}

	ds1 = driver->CreateDataSource(buildingPath1.c_str(), NULL );	
    layer1 = ds1->CreateLayer("building", NULL, wkbPoint, NULL); 
	ds2 = driver->CreateDataSource(buildingPath2.c_str(), NULL );	
    layer2 = ds2->CreateLayer("building", NULL, wkbPolygon, NULL); 
   
	OGRFieldDefn idField("buildingID", OFTInteger);
    if(layer1->CreateField(&idField) != OGRERR_NONE)
    {
       OGRDataSource::DestroyDataSource(ds1);
	   return -1;
    }
	if(layer2->CreateField(&idField) != OGRERR_NONE)
    {
       OGRDataSource::DestroyDataSource(ds2);
	   return -1;
    }

	obaClassifier->sortBounding();
	objs = obaClassifier->getEntitys();
	for(objIter = objs->begin(); objIter != objs->end(); ++objIter) 
	{
		entity = objIter->second;
		if(entity->getType() != PointType::p_building)
		{
			continue;
		}

		pts = entity->getBounding();	
		OGRLinearRing ring;
		for(ptIter = pts.begin(); ptIter != pts.end(); ++ptIter)
		{
			pt3d = gridIndex->getMaxPT3D(ptIter->x, ptIter->y);
			if(pt3d == NULL)
			{
				continue;
			}
			ring.addPoint(pt3d->x+lasHeader->xMin, pt3d->y+lasHeader->yMin, pt3d->z);

			OGRPoint point(pt3d->x+lasHeader->xMin, pt3d->y+lasHeader->yMin, pt3d->z);
			OGRFeature feature1(layer1->GetLayerDefn());
			feature1.SetField("buildingID", entity->getID());
			feature1.SetGeometry(&point); 
			if(layer1->CreateFeature(&feature1) != OGRERR_NONE)
			{
				OGRDataSource::DestroyDataSource(ds1);
				return -1;
			}
		}	

		OGRPolygon polygon;
		polygon.addRing(&ring);
		OGRFeature feature2(layer2->GetLayerDefn());
		feature2.SetField("buildingID", entity->getID());
		feature2.SetGeometry(&polygon); 
		if(layer2->CreateFeature(&feature2) != OGRERR_NONE)
		{
			OGRDataSource::DestroyDataSource(ds2);
			return -1;
		}
	}

	OGRDataSource::DestroyDataSource(ds1);	
	OGRDataSource::DestroyDataSource(ds2);	
	return 0;
}

/**
* Extract building raster
*/
EXPORT int exportBuildingRaster(WorkSpace *workSpace)
{
	GridIndex *gridIndex = NULL;
	LASHeader *lasHeader = NULL;
	int gridW = 0, gridH = 0, count = 0, ii = 0, x = 0, y = 0;
	float *rasterZ = 0;
	OBAClassifier *obaClassifier = NULL;	
	string buildingPath;
	hash_map<int, Entity*> *objs = NULL;
	hash_map<int, Entity*>::iterator objIter;
	Entity* entity = NULL;
	vector<PT> pts;
	vector<PT>::iterator ptIter;
	PT3D *pt3d;
	double zMax = MINV, zMin = MAXV, zMean = 0.0, zDev = 0.0;

	gridIndex = workSpace->getGridIndex();
	lasHeader = gridIndex->getLASHeader();
	gridW = gridIndex->getGridWidth();
	gridH = gridIndex->getGridHeight();
	obaClassifier = workSpace->getOBAClassifier();
	buildingPath = std::string(workSpace->getResFolder()) + "building.img";

	count = gridW * gridH;
	rasterZ = new float[count];
	for(ii = 0; ii < count; ++ii)
	{
		rasterZ[ii] = NAN;
	}

	GDALAllRegister();
	double geoTf[6] = { lasHeader->xMin, 1.0, 0.0, lasHeader->yMax, 0.0, -1.0};
    GDALDriver *driver = GetGDALDriverManager()->GetDriverByName("hfa");
	if((_access(buildingPath.c_str(), 0)) != -1)
	{
		if(driver->Delete(buildingPath.c_str()) != CPLErr::CE_None)
		{
			return -1;
		}
	}

	objs = obaClassifier->getEntitys();
	for(objIter = objs->begin(); objIter != objs->end(); ++objIter) 
	{
		entity = objIter->second;
		if(entity->getType() != PointType::p_building)
		{
			continue;
		}

		pts = entity->getAllPts();	
		for(ptIter = pts.begin(); ptIter != pts.end(); ++ptIter)
		{
			pt3d = gridIndex->getMaxPT3D(ptIter->x, ptIter->y);
			if(pt3d == NULL)
			{
				continue;
			}
			x = int(ptIter->x);
			y = int(ptIter->y);
			rasterZ[y*gridW+x] = pt3d->z;
		}		
	}	

	statistic_raster(rasterZ, count, zMin, zMax, zMean, zDev);
	
	GDALDataset *outDS = driver->Create(buildingPath.c_str(), gridW, gridH, 1, GDT_Float32, NULL);
	outDS->SetGeoTransform(geoTf);
	GDALRasterBand *poBand = outDS->GetRasterBand(1);
	poBand->RasterIO(GF_Write, 0, 0, gridW, gridH, rasterZ, gridW, gridH, GDT_Float32, 0, 0);
	poBand->SetStatistics(zMin, zMax, zMean, zDev);
	poBand->SetNoDataValue(NAN);
	GDALClose(outDS);

	if(rasterZ != NULL)
	{
		delete []rasterZ;
		rasterZ = NULL;
	}

	return 0;
}

/**
*
*/
EXPORT void clipRegion(const char *lasPath, const char *outLasPath, const double &xmin, const double &ymin, const double &xmax, const double &ymax)
{
	LasIO *in_lasIO = NULL, *out_lasIO = NULL;
	in_lasIO = new LasIO(string(lasPath));
	LASHeader *header = in_lasIO->open();
	LASPoint *pts = in_lasIO->readAllPts();
	in_lasIO->close();

	LASPoint *laspt = NULL;
	int ii = 0, num = header->records_number, count = 0;
	for(ii = 0; ii < num; ++ii)
	{
		laspt = &pts[ii];
		if(laspt->x < xmin || laspt->x >xmax || laspt->y < ymin || laspt->y > ymax)
		{
			laspt->class_id = PointType::p_created;
			continue;
		}

		count++;
		laspt->class_id = PointType::p_unclassified;
	}

	out_lasIO = new LasIO(string(outLasPath));
	out_lasIO->open(true);
	out_lasIO->writePts(pts, count, PointType::p_unclassified);
	out_lasIO->close();

	if(pts != NULL)
	{
		delete []pts;
		pts = NULL;
	}
	if(in_lasIO != NULL)
	{
		delete in_lasIO;
		in_lasIO = NULL;
	}
	if(out_lasIO != NULL)
	{
		delete out_lasIO;
		out_lasIO = NULL;
	}
}

/** 
* Merge the given datasets
*/
EXPORT void mergePoints(std::vector<const char *> las_paths_l, const char *out_las_path)
{
	LasIO *in_lasIO = NULL, *out_lasIO = NULL;
	LASHeader *header = NULL;
	LASPoint *pts = NULL;
	int ii = 0, num = header->records_number;

	out_lasIO = new LasIO(string(out_las_path));
	out_lasIO->open(true);

	for (std::vector<const char *>::iterator it = las_paths_l.begin(); it != las_paths_l.end(); ++it)
	{
		in_lasIO = new LasIO(string(*it));
		header = in_lasIO->open();
		pts = in_lasIO->readAllPts();
		in_lasIO->close();

		num = header->records_number;
		for(ii = 0; ii < num; ++ii)
		{
			pts[ii].class_id = PointType::p_unclassified;
		}		
		out_lasIO->writePts(pts, num, PointType::p_unclassified);

		if(pts != NULL)
		{
			delete []pts;
			pts = NULL;
		}
		if(in_lasIO != NULL)
		{
			delete in_lasIO;
			in_lasIO = NULL;
		}		
	}

	out_lasIO->close();
	if(out_lasIO != NULL)
	{
		delete out_lasIO;
		out_lasIO = NULL;
	}
}

/**
*
*/
void getHeightDiffandAngleDiff(PT3D &p1, PT3D &p2, double &hdiff, double &angle)
{
	double dis = 0.0;
	hdiff = abs(p1.z - p2.z);
	dis = sqrt(pow(p1.x - p2.x, 2.0) + pow(p1.y - p2.y, 2.0));
	angle = atan(hdiff / dis);	
}

/**
* Get the local height different matrix
*/
void getLocalEdge(WorkSpace *workSpace, const double &hT, bool *edge)
{
	GridIndex *gridIndex = NULL;
	int gridW = 0, gridH = 0, ix = 0, iy = 0, pos = 0, num = 0, ii = 0;
	PT3D *pt3d1 = NULL, *pt3d2 = NULL;
	PT nhbor[4];
	double sumZ = 0.0;

	gridIndex = workSpace->getGridIndex();
	gridW = gridIndex->getGridWidth();
	gridH = gridIndex->getGridHeight();
	for (iy = 0; iy < gridH; ++iy)
	{	
		for (ix = 0; ix < gridW; ++ix)
		{
			pos = iy * gridW + ix;
			pt3d1 = gridIndex->getMinPT3D(ix, iy);
			if (pt3d1 == NULL)
			{
				continue;
			}

			gridIndex->get4NeighPTs(ix, iy, nhbor);
			
			num = 0;
			sumZ = 0.0;
			for (ii = 0; ii < 4; ++ii)
			{
				/* out of the range,ignore it */
				pt3d2 = gridIndex->getMinPT3D(nhbor[ii].x, nhbor[ii].y);
				if(pt3d2 == NULL) 
				{
					continue;
				}
				sumZ += pt3d2->z;
				num++;
			}
			sumZ = pt3d1->z - sumZ/num;
			edge[pos] = sumZ <= hT ? false : true;
		}
	}

#if LOG_OUT
	string edges_path = string(workSpace->getResFolder()) + string("edges.pt");
	FILE *fp = fopen(edges_path.c_str(), "w+");
	for (iy = 0; iy < gridH; ++iy)
	{	
		for (ix = 0; ix < gridW; ++ix)
		{
			pos = iy * gridW + ix;
			if(edge[pos])
			{
				fprintf(fp, "%d %d\n", ix, iy);
			}
		}
		fflush(fp);
	}
	fclose(fp);	
#endif
}

/**
* It is used to convet las to rasterZ in memory
*/
void las2RasterZ(WorkSpace *workSpace, const double &dtmFactor, const int &rasterW, const int &rasterH, float *rasterZ)
{
	string inLASPath;
	LasIO *lasIO = NULL;
	LASHeader *lasHeader = NULL;
	LASPoint *lasPts = NULL, *lasPt = NULL;
	int lasPtN = 0, ii = 0, jj = 0, minID = -1, h = 0, w = 0, tinN = 0;
	double cX[3], cY[3], minDis = MAXV, tmpDis = 0.0, p[3], *S[3];
	triangle *t = NULL;

	inLASPath = workSpace->getResFolder() + string("ground.las");
	lasIO = new LasIO(inLASPath);
	lasHeader = lasIO->open();
	if(lasHeader->records_number > MAX_LAS_POINTS)
	{
		printf("too much points, memory is not enough!\n");
		if(lasIO != NULL)
		{
			delete lasIO;
			lasIO = NULL;
		}
		return;
	}

	lasPts = lasIO->readAllPts();
	lasPtN = lasHeader->records_number;
	cX[0] = lasHeader->xMin;
	cX[1] = lasHeader->xMax;
	cX[2] = lasHeader->xMax;
	cY[0] = lasHeader->yMin;
	cY[1] = lasHeader->yMin;
	cY[2] = lasHeader->yMax;

	TIN tin(lasPtN);
	for(ii = 0; ii < 3; ++ii)
	{
		minDis = MAXV;
		minID = -1;

		for(jj = 0; jj < lasPtN; ++jj)
		{
			lasPt = &lasPts[jj];
			tmpDis = abs(lasPt->x - cX[ii]) + abs(lasPt->y - cY[ii]);
			if(tmpDis < minDis)
			{
				minDis = tmpDis;
				minID = jj;
			}
		}
		lasPt = &lasPts[minID];
		S[ii] = new double[3];
		S[ii][0] = lasPt->x - lasHeader->xMin;
		S[ii][1] = lasPt->y - lasHeader->yMin;
		S[ii][2] = lasPt->z;
	}
	tin.initialize(S[0], S[1], S[2]);

	for(ii = 0; ii < lasPtN; ++ii)
	{
		p[0] = lasPts[ii].x - lasHeader->xMin;
		p[1] = lasPts[ii].y - lasHeader->yMin;
		p[2] = lasPts[ii].z;
		tin.add(p);
	}

	// Start conveting the TIN triangle to raster triangle	
	for(h = 0; h < rasterH; ++h)
	{
		for(w = 0; w < rasterW; ++w)
		{
			rasterZ[h * rasterW + w] = NAN;
		}
	}
	tinN = tin.tinCount();
	for (ii = 0; ii < tinN; ++ii)
	{
		t = tin.triangleIndex(ii);
		if (t->next < 0) // If not deleted
		{		
			if (t->V[0]) 
			{				
				raster_triangle(t, dtmFactor, rasterW, rasterH, rasterZ);
			}
		}	
	}

	// Delete the lasPts buffer and *S[3] buffer
	if(lasPts != NULL)
	{
		delete []lasPts;
		lasPts = NULL;
	}
	for(int ii = 0; ii < 3; ++ii)
	{
		if(S[ii] != NULL)
		{
			delete S[ii];
			S[ii] = NULL;
		}
	}
	if(lasIO != NULL)
	{
		delete lasIO;
		lasIO = NULL;
	}
}

/**
* Interpolate dem with given resolution
*/
EXPORT void las2Raster(WorkSpace *workSpace, const double &dtmFactor)
{
	string resultFolder, inLASPath, rasterPath;
	LasIO *lasIO = NULL;
	LASHeader *lasHeader = NULL;
	LASPoint *lasPts = NULL, *lasPt = NULL;
	int lasPtN = 0, ii = 0, jj = 0, minID = -1, rasterW = 0, rasterH = 0, h = 0, w = 0, tinN = 0;
	double cX[3], cY[3], minDis = MAXV, tmpDis = 0.0, p[3], *S[3], zMax = MINV, zMin = MAXV, zMean = 0.0, zDev = 0.0, count = 0, validN = 0;
	float *rasterZ = NULL;
	triangle *t = NULL;

	resultFolder = workSpace->getResFolder();
	inLASPath = resultFolder + "ground.las";

	lasIO = new LasIO(inLASPath);
	lasHeader = lasIO->open();
	if(lasHeader->records_number > MAX_LAS_POINTS)
	{
		printf("too much points, memory is not enough!\n");
		if(lasIO != NULL)
		{
			delete lasIO;
			lasIO = NULL;
		}
		return;
	}

	lasPts = lasIO->readAllPts();
	lasPtN = lasHeader->records_number;
	cX[0] = lasHeader->xMin;
	cX[1] = lasHeader->xMax;
	cX[2] = lasHeader->xMax;
	cY[0] = lasHeader->yMin;
	cY[1] = lasHeader->yMin;
	cY[2] = lasHeader->yMax;

	TIN tin(lasPtN);
	for(ii = 0; ii < 3; ++ii)
	{
		minDis = MAXV;
		minID = -1;

		for(jj = 0; jj < lasPtN; ++jj)
		{
			lasPt = &lasPts[jj];
			tmpDis = abs(lasPt->x - cX[ii]) + abs(lasPt->y - cY[ii]);
			if(tmpDis < minDis)
			{
				minDis = tmpDis;
				minID = jj;
			}
		}
		lasPt = &lasPts[minID];
		S[ii] = new double[3];
		S[ii][0] = lasPt->x - lasHeader->xMin;
		S[ii][1] = lasPt->y - lasHeader->yMin;
		S[ii][2] = lasPt->z;
	}
	tin.initialize(S[0], S[1], S[2]);

	for(ii = 0; ii < lasPtN; ++ii)
	{
		p[0] = lasPts[ii].x - lasHeader->xMin;
		p[1] = lasPts[ii].y - lasHeader->yMin;
		p[2] = lasPts[ii].z;
		tin.add(p);
	}

	// Delete the lasPts buffer and *S[3] buffer
	if(lasPts != NULL)
	{
		delete []lasPts;
		lasPts = NULL;
	}

	// Start conveting the TIN triangle to raster triangle	
	rasterW = int((lasHeader->xMax - lasHeader->xMin)/dtmFactor) + 1;
	rasterH = int((lasHeader->yMax - lasHeader->yMin)/dtmFactor) + 1;
	rasterZ = new float[rasterW * rasterH];
	for(h = 0; h < rasterH; ++h)
	{
		for(w = 0; w < rasterW; ++w)
		{
			rasterZ[h * rasterW + w] = NAN;
		}
	}
	tinN = tin.tinCount();
	for (ii = 0; ii < tinN; ++ii)
	{
		t = tin.triangleIndex(ii);
		if (t->next < 0) // If not deleted
		{		
			if (t->V[0]) 
			{				
				raster_triangle(t, dtmFactor, rasterW, rasterH, rasterZ);
			}
		}	
	}

	// Statistic the raster
	count = rasterW * rasterH;
	statistic_raster(rasterZ, count, zMin, zMax, zMean, zDev);

	GDALAllRegister();
	rasterPath = resultFolder + "ground.img";
	double geoTf[6] = { lasHeader->xMin, dtmFactor, 0.0, lasHeader->yMax, 0.0, -dtmFactor};
    GDALDriver *driver = GetGDALDriverManager()->GetDriverByName("hfa");
	GDALDataset *outDS = driver->Create(rasterPath.c_str(), rasterW, rasterH, 1, GDT_Float32, NULL);
	outDS->SetGeoTransform(geoTf);
	GDALRasterBand *poBand = outDS->GetRasterBand(1);
	poBand->RasterIO(GF_Write, 0, 0, rasterW, rasterH, rasterZ, rasterW, rasterH, GDT_Float32, 0, 0);
	poBand->SetStatistics(zMin, zMax, zMean, zDev);
	poBand->SetNoDataValue(NAN);
	GDALClose(outDS);   

	for(int ii = 0; ii < 3; ++ii)
	{
		if(S[ii] != NULL)
		{
			delete S[ii];
			S[ii] = NULL;
		}
	}
	if(lasIO != NULL)
	{
		delete lasIO;
		lasIO = NULL;
	}
	if(rasterZ != NULL)
	{
		delete []rasterZ;
		rasterZ = NULL;
	}
}