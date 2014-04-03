#ifndef NEWPROJECTDLG_H
#define NEWPROJECTDLG_H

#include <QDialog>

#include "ui_newprojectdlg.h"
#include "lasio.h"

class NewProjectDlg : public QDialog
{
Q_OBJECT

public:
	NewProjectDlg(QWidget *parent = 0);
	~NewProjectDlg();

	inline QString getLasPath()
	{
		return this->ui.lasPath->toPlainText();
	}
	inline QString getResultDir()
	{
		return this->ui.resultDir->toPlainText();
	}
	inline double cellSize()
	{
		return this->_cellSize;
	}
	inline double ptDensity()
	{
		return this->_ptDensity;
	}

private:
	Ui::NewProjectDlgClass ui;
	LasIO *_lasIO;
	double _cellSize;
	double _ptDensity;

private slots:
	void clickLasOpenBtn();
	void clickProjectOpenBtn();
	void tabIndexChange(int index);
	void gridSizeValueChanged();
};

#endif // NEWPROJECTDLG_H
