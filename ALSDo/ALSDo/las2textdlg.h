#ifndef LAS2TEXT_H
#define LAS2TEXT_H

#include <QDialog>
#include "ui_las2textdlg.h"

class Las2TextDlg : public QDialog
{
	Q_OBJECT

public:
	Las2TextDlg(QWidget *parent = 0);
	~Las2TextDlg();

private:
	Ui::las2textDlgClass ui;

private slots:
	void clickLasOpenBtn();
	void clickTextOpenBtn();
	void clickStartBtn();
};

#endif // LAS2TEXT_H
