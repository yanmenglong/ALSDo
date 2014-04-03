#ifndef LASSPLITOR
#define LASSPLITOR

#include "global.h"

class LasIO;

#include <vector>
using namespace std;

typedef struct MBR
{
	double minX;
	double maxX;
	double minY;
	double maxY;
}MBR;

typedef struct Block 
{
	MBR mbr;
	int neigh[4];
	LasIO *lasIO;

	void dispose()
	{
		if(this->lasIO != NULL)
		{
			delete this->lasIO;
			this->lasIO = NULL;
		}
	}
}Block;

class LIDARCORE_API LasSplitor
{
public:
	LasSplitor();
	virtual ~LasSplitor();
	void loadFiles(vector<string> paths);
	void splitFiles(string resFolder);

	MBR& getRegion();
	vector<MBR>& getOriMBRs();
	vector<MBR>& getResMBRs();

private:
	MBR _region;
	int _ptNum;
	double _area;
	vector<MBR> _oriMBRs;
	vector<MBR> _resMBRs;
	vector<Block> _blocks; 
	vector<LasIO*> _lasIOs;
};

#endif