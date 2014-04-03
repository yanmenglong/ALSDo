#ifndef POINTINFODLG_H
#define POINTINFODLG_H

#include <QDialog>
#include "ui_pointinfodlg.h"

#include "lasio.h"

class PointInfoDlg : public QDialog
{
Q_OBJECT

public:
	PointInfoDlg(LASHeader *lasHeader, QWidget *parent = 0);
	~PointInfoDlg();

private:
	Ui::PointInfoDlgClass ui;
};

#endif // POINTINFODLG_H
