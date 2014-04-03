#include "utils.h"
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QIODevice>
#include <QTextStream>

#include "global.h"

#ifdef __cplusplus 
extern "C" { 
#endif

QString GetOpenFilePath(QWidget *parent, QString title, QString filter)
{
	QString path("./path.history");
	QString folder("./");	
	if(QFile::exists(path))
	{
		QFile file(path);
		if(file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QTextStream in(&file);
			folder = in.readLine();
		}
	}

	QString fileName = QFileDialog::getOpenFileName(parent, title, folder, filter);
	if(!fileName.isEmpty())
	{
		QFileInfo info(fileName);
		QFile file(path);
		if(file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QTextStream out(&file);
			out<<info.absoluteDir().absolutePath();
		}
	}	
	return fileName;
}

QString GetSaveFilePath(QWidget *parent, QString title, QString filter)
{
	QString path("./path.history");
	QString folder("./");	
	if(QFile::exists(path))
	{
		QFile file(path);
		if(file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QTextStream in(&file);
			folder = in.readLine();
		}
	}
	QString fileName = QFileDialog::getSaveFileName(parent, title, folder, filter);
	if(!fileName.isEmpty())
	{
		QFileInfo info(fileName);
		QFile file(path);
		if(file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QTextStream out(&file);
			out<<info.absoluteDir().absolutePath();
		}
	}
	return fileName;
}

void calcPtDensity(LASHeader *lasHeader, double &cellSize, double &ptDensity)
{
	double xR = lasHeader->xMax - lasHeader->xMin;
	double yR = lasHeader->yMax - lasHeader->yMin;

	cellSize = 1.0;
	ptDensity = lasHeader->records_number / (xR * yR);
	while(ptDensity < MIN_PT_DENSITY)
	{
		cellSize += 1.0;
		ptDensity = lasHeader->records_number / ((xR/cellSize) * (yR/cellSize));
	}
}

#ifdef __cplusplus 
} 
#endif