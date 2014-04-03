#ifndef OBAFILTERPARAMDLG_H
#define OBAFILTERPARAMDLG_H

#include <QDialog>
#include "ui_obafilterparamdlg.h"

class OBAFilterParamDlg : public QDialog
{
	Q_OBJECT

public:
	OBAFilterParamDlg(QWidget *parent = 0);
	~OBAFilterParamDlg();

	// Seed generation
	inline double getSeedHeightTh()
	{
		return this->ui.seedHeightTh->toPlainText().toDouble();		
	}
	inline double getSeedSlopeTh()
	{
		return this->ui.seedSlopeTh->toPlainText().toDouble();	
	}
	inline double getMaxObjSize()
	{
		return this->ui.maxObjSize->toPlainText().toDouble();		
	}
	inline double getSeedCellSize()
	{
		return this->ui.seedCellSize->toPlainText().toDouble();	
	}
	// OBA segmentation
	inline double getSegHeightTh()
	{
		return this->ui.segHeightTh->toPlainText().toDouble();		
	}
	inline double getSegSlopeTh()
	{
		return this->ui.segSlopeTh->toPlainText().toDouble();	
	}
	// LAS points filtering
	inline double getFilteringHeightTh()
	{
		return this->ui.filteringHeightTh->toPlainText().toDouble();		
	}
	inline double getFilteringSlopeTh()
	{
		return this->ui.filteringSlopeTh->toPlainText().toDouble();	
	}	
	inline double getDTMFactor()
	{
		return this->ui.dtmFactor->toPlainText().toDouble();	
	}
	inline double getObjectHT()
	{
		return this->ui.objectHT->toPlainText().toDouble();	
	}

private:
	Ui::OBAFilterParamDlg ui;
};

#endif // OBAFILTERPARAMDLG_H
