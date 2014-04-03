#include "autorunthread.h"

#include "global.h"
#include "util.h"
#include "tinfilter.h"

#include <string>
using namespace std;

AutoRunThread::AutoRunThread(QObject *parent)
: QThread(parent)
{
}

AutoRunThread::~AutoRunThread()
{
}

void AutoRunThread::run()
{
	string lasFilePath, resFolder;
	WorkSpace *workSpace = NULL;

	lasFilePath = this->lasPath.toStdString();
	resFolder = this->resFolder.toStdString();
	workSpace = new WorkSpace();
	workSpace->newProj(lasFilePath.c_str(), this->cellSize, resFolder.c_str());
	resFolder = workSpace->getResFolder();

	// Seed generation
	if(this->useOBASeedGenerator && this->maxObjSize >= 20.0)
	{
		emit stepInformation(0, tr("start identifying complexity of the scene ..."));
		::calcComplexByObj(workSpace, this->maxObjSize, this->seedHeightTh, this->seedSlopeTh);
		emit stepInformation(20, tr("identifying complexity of the scene finished ..."));
	}

	// Filtering
	emit stepInformation(20, tr("start filter points ..."));
	TINFilter *filter = new TINFilter(workSpace, maxObjSize, this->seedCellSize, this->angleIteration, this->hIteration);
	filter->filterGround(this->filteringHeightTh, this->filteringSlopeTh);
	emit stepInformation(70, tr("filtering points finished ..."));
	
	// Dem interpolation
	emit stepInformation(70, tr("start dem interpolation ..."));
	las2Raster(workSpace, this->dtmFactor);
	emit stepInformation(90, tr("dem interpolation finished..."));
	
	// Object points extraction
	emit stepInformation(90, tr("start get non-groud points ..."));
	extractObjectPoints(workSpace, this->dtmFactor, this->objectHT);
	emit stepInformation(100, tr("process finished successfully!"));

	if (workSpace != NULL)
	{
		delete workSpace;
		workSpace = NULL;
	}
}