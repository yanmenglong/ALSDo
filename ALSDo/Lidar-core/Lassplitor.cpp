#include "lassplitor.h"
#include "lasio.h"

LasSplitor::LasSplitor()
{
	this->_region.minX = 999999999.0;
	this->_region.minY = 999999999.0;
	this->_region.maxX = -999999999.0;
	this->_region.maxY = -999999999.0;
	this->_ptNum = 0;
	this->_area = 0.0;
}

LasSplitor::~LasSplitor()
{
	this->_oriMBRs.clear();
	this->_resMBRs.clear();
	this->_blocks.clear();

	for(vector<LasIO*>::iterator it=this->_lasIOs.begin(); it!=this->_lasIOs.end(); ++it)
	{
		LasIO *lasIO = *it;
		if(lasIO != NULL)
		{
			delete lasIO;
			lasIO = NULL;
		}
	}
	this->_lasIOs.clear();

	for(vector<Block>::iterator it=this->_blocks.begin(); it!=this->_blocks.end(); ++it)
	{
		Block block = *it;
		block.dispose();
	}
}

void LasSplitor::loadFiles(vector<string> paths)
{
	for(vector<string>::iterator it=paths.begin(); it!=paths.end(); ++it)
	{
		string path = *it;
		LasIO *lasIO = NULL;
		lasIO = new LasIO(path);
		LASHeader *header = lasIO->open();
		this->_lasIOs.push_back(lasIO);

		MBR mbr;
		mbr.minX = header->xMin;
		mbr.minY = header->yMin;
		mbr.maxX = header->xMax;
		mbr.maxY = header->yMax;
		this->_oriMBRs.push_back(mbr);

		this->_ptNum += header->records_number;
		this->_area += (mbr.maxX-mbr.minX)*(mbr.maxY-mbr.minY);

		this->_region.minX = mbr.minX < this->_region.minX ? mbr.minX : this->_region.minX;
		this->_region.minY = mbr.minY < this->_region.minY ? mbr.minY : this->_region.minY;
		this->_region.maxX = mbr.maxX > this->_region.maxX ? mbr.maxX : this->_region.maxX;
		this->_region.maxY = mbr.maxY > this->_region.maxY ? mbr.maxY : this->_region.maxY; 
	}
}

void LasSplitor::splitFiles(string resFolder)
{
	double density = this->_ptNum/this->_area;
	double factor = density/4.0;
	double RegionH = 1000.0/factor + 1.0;
	double RegionW = 1000.0/factor + 1.0;

	if(resFolder.at(resFolder.length()-1) != '/')
	{
		resFolder = resFolder + string("/");
	}

	int partN = 0;
	char intTag [4];
	for(double gY=this->_region.minY; gY<=this->_region.maxY; gY+=RegionH)
	{
		for(double gX=this->_region.minX; gX<=this->_region.maxX; gX+=RegionW)
		{
			partN++;
			memset(intTag, 0, sizeof(char)*4);
			sprintf(intTag, "%d", partN);
			string path = resFolder + string(intTag) + ".las";

			Block block;
			block.mbr.minX = gX;
			block.mbr.maxX = gX + RegionW;
			block.mbr.minY = gY;
			block.mbr.maxY = gY + RegionH;
			memset(block.neigh, 0, sizeof(int)*4);
			block.lasIO = NULL;
			block.lasIO = new LasIO(path);
			block.lasIO->open(true);
			this->_blocks.push_back(block);
		}
	}

	//split the las files
	for(vector<LasIO*>::iterator it1=this->_lasIOs.begin(); it1!=this->_lasIOs.end(); ++it1)
	{
		LasIO *lasIO = *it1;
		LASPoint* pts = lasIO->readAllPts();
		LASPoint* pt = NULL;
		int ptNum = lasIO->lasHeader()->records_number;		
		for(int ii=0; ii<ptNum; ++ii)
		{
			pt = &pts[ii];
			for(vector<Block>::iterator it2=this->_blocks.begin(); it2!=this->_blocks.end(); ++it2)
			{
				Block block = *it2;
				if(pt->x>=block.mbr.minX && pt->x<=block.mbr.maxX && pt->y>=block.mbr.minY && pt->y<=block.mbr.maxY)
				{
					block.lasIO->writePt(*pt);
					break;
				}
			}
		}

		if(pts != NULL)
		{
			delete []pts;
			pts = NULL;
		}
	}

	//free memory
	for(vector<LasIO*>::iterator it=this->_lasIOs.begin(); it!=this->_lasIOs.end(); ++it)
	{
		LasIO *lasIO = *it;
		if(lasIO != NULL)
		{
			delete lasIO;
			lasIO = NULL;
		}
	}
	this->_lasIOs.clear();

	//close the file
	for(vector<Block>::iterator it=this->_blocks.begin(); it!=this->_blocks.end(); ++it)
	{
		Block block = *it;
		block.lasIO->close();
		//remove(block.lasIO->lasPath().c_str());
	}

	//if nuber of points less than 1,000,000, it will be merged	
	printf("density : %lf\n", density);
}

MBR& LasSplitor::getRegion()
{
	return this->_region;
}

vector<MBR>& LasSplitor::getOriMBRs()
{
	return this->_oriMBRs;
}

vector<MBR>& LasSplitor::getResMBRs()
{
	return this->_resMBRs;
}
