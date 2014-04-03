#include "obafilterparamdlg.h"

OBAFilterParamDlg::OBAFilterParamDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	// Seed generation
	this->ui.seedHeightTh->setPlainText(tr("1.0"));
	this->ui.seedSlopeTh->setPlainText(tr("35.0"));
	this->ui.maxObjSize->setPlainText(tr("60.0"));
	this->ui.seedCellSize->setPlainText(tr("15.0"));

	// OBA segmentation
	this->ui.segHeightTh->setPlainText(tr("1.0"));
	this->ui.segSlopeTh->setPlainText(tr("45.0"));

	// LAS points filtering
	this->ui.filteringHeightTh->setPlainText(tr("0.3"));
	this->ui.filteringSlopeTh->setPlainText(tr("10.0"));
	this->ui.dtmFactor->setPlainText(tr("1.0"));
	this->ui.objectHT->setPlainText(tr("1.0"));

	this->connect(this->ui.okBtn, SIGNAL(clicked()), this, SLOT(accept()));
	this->connect(this->ui.cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));
}

OBAFilterParamDlg::~OBAFilterParamDlg()
{

}
