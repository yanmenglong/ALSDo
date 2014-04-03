#ifndef AUTORUNTHREAD_H
#define AUTORUNTHREAD_H

#include <QThread>

class AutoRunThread : public QThread
{
	Q_OBJECT

public:
	AutoRunThread(QObject *parent);
	~AutoRunThread();

	// File path
	QString lasPath;
	QString resFolder;
	double cellSize;

	// Seed generation
	double maxObjSize;
	bool useOBASeedGenerator;
	double seedHeightTh;
	double seedSlopeTh;
	double seedCellSize;

	// Filtering
	double hIteration;
	double angleIteration;
	double filteringHeightTh;
	double filteringSlopeTh;
	double dtmFactor;
	double objectHT;

signals:
     void stepInformation(int precent, QString text);

 protected:
     void run();		
};

#endif 
