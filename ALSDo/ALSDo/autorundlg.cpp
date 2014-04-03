#include "autorundlg.h"
#include "utils.h"
#include "workspace.h"


#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <cmath>
#include <string>
using namespace std;

AutoRunDlg::AutoRunDlg(QWidget *parent)
	: QDialog(parent),
	runThread(this)
{
	this->lasIO = NULL;

	this->ui.setupUi(this);

	// Cell size
	this->ui.cellSize->setPlainText(tr("1.0"));

	// Seed generation
	this->ui.maxObjSize->setPlainText(tr("60.0"));
	this->ui.useOBASeedGeneratorCheckBox->setChecked(true);
	this->ui.seedHeightTh->setEnabled(true);
	this->ui.seedSlopeTh->setEnabled(true);
	this->ui.seedCellSize->setEnabled(true);
	this->ui.seedHeightTh->setPlainText(tr("1.0"));
	this->ui.seedSlopeTh->setPlainText(tr("35.0"));
	this->ui.seedCellSize->setPlainText(tr("20.0"));

	// Filtering
	this->ui.hIteration->setPlainText(tr("1.5"));
	this->ui.angleIteration->setPlainText(tr("6.0"));
	this->ui.filteringHeightTh->setPlainText(tr("0.3"));
	this->ui.filteringSlopeTh->setPlainText(tr("10.0"));
	this->ui.dtmFactor->setPlainText(tr("1.0"));
	this->ui.objectHT->setPlainText(tr("1.0"));

	// Action
	this->ui.tabWidget->setCurrentIndex(0);
	this->ui.progressBar->setVisible(false);
	this->connect(this->ui.lasOpenBtn, SIGNAL(clicked()), this, SLOT(clickLasOpenBtn()));
	this->connect(this->ui.projectOpenBtn, SIGNAL(clicked()), this, SLOT(clickProjectOpenBtn()));
	this->connect(this->ui.runBtn, SIGNAL(clicked()), this, SLOT(clickRunBtn()));
	this->connect(this->ui.quitBtn, SIGNAL(clicked()), this, SLOT(close()));
	this->connect(this->ui.cellSize, SIGNAL(textChanged()), this, SLOT(cellSizeValueChanged()));
	this->connect(this->ui.useOBASeedGeneratorCheckBox, SIGNAL(clicked()), this, SLOT(useOBASeedGeneratorCheckBoxStateChanged()));

	// Autorun thread
	this->connect(&this->runThread, SIGNAL(stepInformation(int, QString)), this, SLOT(showStepInformation(int, QString)));
}

AutoRunDlg::~AutoRunDlg()
{
	if(this->lasIO != NULL)
	{
		delete this->lasIO;
		this->lasIO = NULL;
	}
}

void AutoRunDlg::clickLasOpenBtn()
{
	QString fileName, content;
	LASHeader *lasHeader = NULL;
	double cellSize = 0.0, ptDensity = 0.0;
	string lasPath;

	fileName = GetOpenFilePath(this, QObject::tr("Open LAS File"), tr("LAS file (*.las)"));
	if (!fileName.isNull()) 
	{
		QFileInfo fileInfo(fileName);
		if(!fileInfo.exists()) 
		{
			QMessageBox::critical(this, tr("error"), tr("las file does not exist!"), QMessageBox::Ok);
			this->ui.lasPath->setPlainText("");
			return;
		}
		else 
		{
			this->ui.lasPath->setPlainText(fileName);

			//calculate suitable size of resample grid
			if(this->lasIO != NULL)
			{
				delete this->lasIO;
				this->lasIO = NULL;
			}

			lasPath = this->ui.lasPath->toPlainText().toStdString();
			this->lasIO = new LasIO(lasPath);
			lasHeader = this->lasIO->open();
			calcPtDensity(lasHeader, cellSize, ptDensity);

			content = QString("%1").arg(cellSize, 0, 'f', 3);
			this->ui.cellSize->setPlainText(content);
			content = QString("%1").arg(ptDensity, 0, 'f', 3);
			this->ui.pointDensity->setText(content);
		}
    }
}

void AutoRunDlg::clickProjectOpenBtn() 
{
	QString fileName = GetSaveFilePath(this, tr("new LAS project path"), tr("LAS project file (*.*)"));
    if (!fileName.isNull()) 
	{
		this->ui.resultFolder->setPlainText(fileName);
    }
}

void AutoRunDlg::clickRunBtn()
{
	QString lasPath, resFolder;
	double cellSize = 0.0, maxObjSize = 0.0, seedHeightTh = 0.0, seedSlopeTh = 0.0, seedCellSize = 0.0, hIteration = 0.0, 
		angleIteration = 0.0, filteringHeightTh = 0.0, filteringSlopeTh = 0.0, dtmFactor = 0.0, objectHT = 0.0;
	bool useOBASeedGenerator = false;

	lasPath = this->ui.lasPath->toPlainText();
	resFolder = this->ui.resultFolder->toPlainText();
	if(lasPath.isEmpty() || resFolder.isEmpty())
	{
		QMessageBox::critical(this, tr("error"), tr("file path is null!"), QMessageBox::Ok);
		return;
	}
	QDir dir;
	if(!dir.exists(resFolder)) 
	{
		dir.mkdir(resFolder);
	}

	// Unable ui
	this->ui.runBtn->setEnabled(false);
	this->ui.quitBtn->setEnabled(false);
	this->ui.progressBar->setVisible(true);

	cellSize = this->ui.cellSize->toPlainText().toDouble();
	maxObjSize = this->ui.maxObjSize->toPlainText().toDouble();
	useOBASeedGenerator = this->ui.useOBASeedGeneratorCheckBox->isChecked();
	seedHeightTh = this->ui.seedHeightTh->toPlainText().toDouble();
	seedSlopeTh = this->ui.seedSlopeTh->toPlainText().toDouble()*PI/180.0;
	seedCellSize = this->ui.seedCellSize->toPlainText().toDouble();
	hIteration = this->ui.hIteration->toPlainText().toDouble();
	angleIteration = this->ui.angleIteration->toPlainText().toDouble()*PI/180.0;
	filteringHeightTh = this->ui.filteringHeightTh->toPlainText().toDouble();
	filteringSlopeTh = this->ui.filteringSlopeTh->toPlainText().toDouble()*PI/180.0;
	dtmFactor = this->ui.dtmFactor->toPlainText().toDouble();
	objectHT = this->ui.objectHT->toPlainText().toDouble();

	this->runThread.lasPath = lasPath;
	this->runThread.resFolder = resFolder;
	this->runThread.cellSize = cellSize;
	this->runThread.maxObjSize = maxObjSize;
	this->runThread.useOBASeedGenerator = useOBASeedGenerator;
	this->runThread.seedHeightTh = seedHeightTh;
	this->runThread.seedSlopeTh = seedSlopeTh;
	this->runThread.seedCellSize = seedCellSize;
	this->runThread.hIteration = hIteration;
	this->runThread.angleIteration = angleIteration;
	this->runThread.filteringHeightTh = filteringHeightTh;
	this->runThread.filteringSlopeTh = filteringSlopeTh;
	this->runThread.dtmFactor = dtmFactor;
	this->runThread.objectHT = objectHT;

	this->runThread.start();
}

void AutoRunDlg::cellSizeValueChanged()
{
	LASHeader *lasHeader = NULL;
	double cellSize = 0.0, ptDensity = 0.0;
	QString content;

	if(this->lasIO == NULL)
	{
		return;
	}

	lasHeader = this->lasIO->lasHeader();
	cellSize = this->ui.cellSize->toPlainText().toDouble();
	ptDensity = lasHeader->records_number / (((lasHeader->xMax - lasHeader->xMin)/cellSize) * ((lasHeader->yMax - lasHeader->yMin)/cellSize));

	if(ptDensity < MIN_PT_DENSITY)
	{
		content = tr("point density is %1, and it is too small!").arg(ptDensity);
		QMessageBox::information(this, tr("information"), content, QMessageBox::Ok);
		//calcPtDensity(lasHeader, cellSize, ptDensity);			
	}

	this->disconnect(this->ui.cellSize, SIGNAL(textChanged()), this, SLOT(cellSizeValueChanged()));
	content = QString("%1").arg(cellSize, 0, 'f', 3);
	this->ui.cellSize->setPlainText(content);
	content = QString("%1").arg(ptDensity, 0, 'f', 3);
	this->ui.pointDensity->setText(content);	
	this->connect(this->ui.cellSize, SIGNAL(textChanged()), this, SLOT(cellSizeValueChanged()));
}

void AutoRunDlg::useOBASeedGeneratorCheckBoxStateChanged()
{
	if(this->ui.useOBASeedGeneratorCheckBox->isChecked())
	{
		this->ui.seedHeightTh->setEnabled(true);
		this->ui.seedSlopeTh->setEnabled(true);
		this->ui.seedCellSize->setEnabled(true);
	}
	else
	{
		this->ui.seedHeightTh->setEnabled(false);
		this->ui.seedSlopeTh->setEnabled(false);
		this->ui.seedCellSize->setEnabled(false);
	}
}

void AutoRunDlg::showStepInformation(int percent, QString text)
{
	this->ui.progressBar->setValue(percent);
	this->ui.infoLabel->setText(text);
	if(percent == 100)
	{
		this->ui.runBtn->setEnabled(true);
		this->ui.quitBtn->setEnabled(true);
	}
}