#ifndef ENTITYPARAMTHRESHOLDDLG_H
#define ENTITYPARAMTHRESHOLDDLG_H

#include <QDialog>
#include "ui_entityparamthresholddlg.h"

class EntityParamThresholdDlg : public QDialog
{
	Q_OBJECT

public:
	EntityParamThresholdDlg(QWidget *parent = 0, const QString &paramTag = tr(""), const double &value = 0.0);
	~EntityParamThresholdDlg();

	// Get given parameter maximum and minimum value
	inline double getValueMin()
	{
		return this->ui.valueMin->toPlainText().toDouble();		
	}
	inline double getValueMax()
	{
		return this->ui.valueMax->toPlainText().toDouble();	
	}

private:
	Ui::EntityParamThresholdDlg ui;

	QString _paramTag;
};

#endif 
