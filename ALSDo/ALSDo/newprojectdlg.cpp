#include "newprojectdlg.h"
#include "utils.h"
#include "global.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <cmath>
#include <string>
using namespace std;



NewProjectDlg::NewProjectDlg(QWidget *parent)
	: QDialog(parent)
{
	this->_lasIO = NULL;
	this->_cellSize = 0.0;
	this->_ptDensity = 0.0;

	this->ui.setupUi(this);
	this->ui.tabWidget->setCurrentIndex(0);
	this->connect(this->ui.lasOpenBtn, SIGNAL(clicked()), this, SLOT(clickLasOpenBtn()));
	this->connect(this->ui.projectOpenBtn, SIGNAL(clicked()), this, SLOT(clickProjectOpenBtn()));
	this->connect(this->ui.okBtn, SIGNAL(clicked()), this, SLOT(accept()));
	this->connect(this->ui.cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));
	this->connect(this->ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabIndexChange(int)));
	this->connect(this->ui.gridSize, SIGNAL(textChanged()), this, SLOT(gridSizeValueChanged()));
}

NewProjectDlg::~NewProjectDlg()
{
	if(this->_lasIO != NULL)
	{
		delete this->_lasIO;
		this->_lasIO = NULL;
	}
}

void NewProjectDlg::clickLasOpenBtn()
{
	QString fileName = GetOpenFilePath(this, QObject::tr("Open LAS File"), tr("LAS file (*.las)"));
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
			if(this->_lasIO != NULL)
			{
				delete this->_lasIO;
				this->_lasIO = NULL;
			}

			string lasPath = this->ui.lasPath->toPlainText().toStdString();
			this->_lasIO = new LasIO(lasPath);
			this->_lasIO->open();
			calcPtDensity(this->_lasIO->lasHeader(), this->_cellSize, this->_ptDensity);			
		}
    }
}

void NewProjectDlg::clickProjectOpenBtn() 
{
	QString fileName = GetSaveFilePath(this, tr("new LAS project path"), tr("LAS project file (*.*)"));
    if (!fileName.isNull()) 
	{
		this->ui.resultDir->setPlainText(fileName);
    }
}

void NewProjectDlg::tabIndexChange(int index)
{
	if(index == 1)
	{
		if(this->ui.lasPath->toPlainText().isEmpty())
		{
			QMessageBox::information(this,tr("information"), tr("please open input las path first!"), QMessageBox::Ok);
			this->ui.tabWidget->setCurrentIndex(0);
		}

		QString value;
		value.setNum(this->_cellSize);
		this->ui.gridSize->setPlainText(value);
		value.setNum(this->_ptDensity);
		this->ui.pointDensity->setPlainText(value);
	}
}

void NewProjectDlg::gridSizeValueChanged()
{
	if(this->_lasIO == NULL)
	{
		return;
	}
	LASHeader *lasHeader = this->_lasIO->lasHeader();

	QString value = this->ui.gridSize->toPlainText();
	this->_cellSize = value.toDouble();
	this->_ptDensity = lasHeader->records_number / (((lasHeader->xMax - lasHeader->xMin)/this->_cellSize) * ((lasHeader->yMax - lasHeader->yMin)/this->_cellSize));

	
	if(this->_ptDensity < MIN_PT_DENSITY)
	{
		value = tr("point density is %1, and it is too small!").arg(this->_ptDensity);
		QMessageBox::information(this, tr("information"), value, QMessageBox::Ok);
		//calcPtDensity(this->_lasheader, this->_cellSize, this->_ptDensity);			
	}

	this->disconnect(this->ui.gridSize, SIGNAL(textChanged()), this, SLOT(gridSizeValueChanged()));
	value.setNum(this->_cellSize);
	this->ui.gridSize->setPlainText(value);
	value.setNum(this->_ptDensity);
	this->ui.pointDensity->setPlainText(value);
	this->connect(this->ui.gridSize, SIGNAL(textChanged()), this, SLOT(gridSizeValueChanged()));	
}

