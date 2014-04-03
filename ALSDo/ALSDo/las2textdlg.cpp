#include "las2textdlg.h"
#include "utils.h"

#include <QMessageBox>
#include <cstring>
using namespace std;

Las2TextDlg::Las2TextDlg(QWidget *parent)
	: QDialog(parent)
{
	this->ui.setupUi(this);

	this->connect(this->ui.lasOpenBtn, SIGNAL(clicked()), this, SLOT(clickLasOpenBtn()));
	this->connect(this->ui.textOpenBtn, SIGNAL(clicked()), this, SLOT(clickTextOpenBtn()));
	this->connect(this->ui.startBtn, SIGNAL(clicked()), this, SLOT(clickStartBtn()));
	this->connect(this->ui.quitBtn, SIGNAL(clicked()), this, SLOT(close()));
}

Las2TextDlg::~Las2TextDlg()
{

}

void Las2TextDlg::clickLasOpenBtn()
{
	QString fileName = GetOpenFilePath(this, QObject::tr("Open LAS File"), tr("LAS file (*.las)"));
	if (!fileName.isNull()) 
	{
		this->ui.lasPath->setPlainText(fileName);
    }
}

void Las2TextDlg::clickTextOpenBtn()
{
	QString fileName = GetSaveFilePath(this, tr("Set output file path"), tr("Text file (*.xyz)"));
    if (!fileName.isNull()) 
	{
		this->ui.textPath->setPlainText(fileName);
    }
}

void Las2TextDlg::clickStartBtn()
{
	string lasPath = this->ui.lasPath->toPlainText().toStdString();
	string textPath = this->ui.textPath->toPlainText().toStdString();	

	LasIO::las2text(lasPath.c_str(), textPath.c_str());
	QMessageBox::information(this, tr("information"), tr("convert las to text file finished!"), QMessageBox::Ok);
}