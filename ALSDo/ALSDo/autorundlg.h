#ifndef AUTORUNDLG_H
#define AUTORUNDLG_H

#include <QDialog>

#ifdef LIDARCORE_EXPORTS
	#define LIDARCORE_API __declspec(dllexport)
#else
	#define LIDARCORE_API __declspec(dllimport)
#endif
#define LAS_BUFFER 100000

#include "ui_autorundlg.h"
#include "autorunthread.h"
#include "lasio.h"

class AutoRunDlg : public QDialog
{
	Q_OBJECT

public:
	AutoRunDlg(QWidget *parent = 0);
	~AutoRunDlg();

private:
	Ui_AutoRunDlgClass ui;	

	// For gridSize and point density
	LasIO *lasIO;
	// Filtering thread
	AutoRunThread runThread;	

private slots:
	void clickLasOpenBtn();
	void clickProjectOpenBtn();
	void clickRunBtn();
	void cellSizeValueChanged();
	void useOBASeedGeneratorCheckBoxStateChanged();

	void showStepInformation(int percent, QString text);
};

#endif 
