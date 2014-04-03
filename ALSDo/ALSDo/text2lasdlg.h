#ifndef TEXT2LASDLG_H
#define TEXT2LASDLG_H

#include <QDialog>
#include "ui_text2lasdlg.h"

class Text2LasDlg : public QDialog
{
	Q_OBJECT

public:
	Text2LasDlg(QWidget *parent = 0);
	~Text2LasDlg();

private:
	Ui::text2lasdlgClass ui;

private slots:
	void clickTextOpenBtn();
	void clickLasOpenBtn();
	void clickStartBtn();
};

#endif // TEXT2LASDLG_H
