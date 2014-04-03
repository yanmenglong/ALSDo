#include "text2lasdlg.h"
#include "utils.h"
#include "lasio.h"

#include <QMessageBox>
#include <cstring>
using namespace std;

Text2LasDlg::Text2LasDlg(QWidget *parent)
	: QDialog(parent)
{
	this->ui.setupUi(this);

	this->connect(this->ui.textOpenBtn, SIGNAL(clicked()), this, SLOT(clickTextOpenBtn()));
	this->connect(this->ui.lasOpenBtn, SIGNAL(clicked()), this, SLOT(clickLasOpenBtn()));	
	this->connect(this->ui.startBtn, SIGNAL(clicked()), this, SLOT(clickStartBtn()));
	this->connect(this->ui.quitBtn, SIGNAL(clicked()), this, SLOT(close()));
}

Text2LasDlg::~Text2LasDlg()
{

}

void Text2LasDlg::clickTextOpenBtn()
{
	QString fileName = GetOpenFilePath(this, tr("Open text file"), tr("Text file (*.xyz)"));
	if (!fileName.isNull()) 
	{
		this->ui.textPath->setPlainText(fileName);
    }
}

void Text2LasDlg::clickLasOpenBtn()
{
	QString fileName = GetSaveFilePath(this, tr("Set output file path"), tr("LAS file (*.las)"));
    if (!fileName.isNull()) 
	{
		this->ui.lasPath->setPlainText(fileName);		
    }	
}

void Text2LasDlg::clickStartBtn()
{
	string textPath = this->ui.textPath->toPlainText().toStdString();
	string lasPath = this->ui.lasPath->toPlainText().toStdString();

	LasIO::text2las(textPath.c_str(), lasPath.c_str());		
	QMessageBox::information(this, tr("information"), tr("convert text to las file finished!"), QMessageBox::Ok);
}
