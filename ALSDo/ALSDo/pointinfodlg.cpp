#include "pointinfodlg.h"

PointInfoDlg::PointInfoDlg(LASHeader *lasHeader, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	QString content;
	content = QString("%1 m").arg(lasHeader->xMin, 0, 'f', 3);
	this->ui.xMin->setPlainText(content);
	content = QString("%1 m").arg(lasHeader->xMax, 0, 'f', 3);
	this->ui.xMax->setPlainText(content);

	content = QString("%1 m").arg(lasHeader->yMin, 0, 'f', 3);
	this->ui.yMin->setPlainText(content);
	content = QString("%1 m").arg(lasHeader->yMax, 0, 'f', 3);
	this->ui.yMax->setPlainText(content);

	content = QString("%1 m").arg(lasHeader->zMin, 0, 'f', 3);
	this->ui.zMin->setPlainText(content);
	content = QString("%1 m").arg(lasHeader->zMax, 0, 'f', 3);
	this->ui.zMax->setPlainText(content);

	content = QString("%1 pts").arg(lasHeader->records_number);
	this->ui.totalPtN->setPlainText(content);
	content = QString("%1 pts").arg(lasHeader->returns_number[0]);
	this->ui.firstPtN->setPlainText(content);
	content = QString("%1 pts").arg(lasHeader->returns_number[1]);
	this->ui.secondPtN->setPlainText(content);
	content = QString("%1 pts").arg(lasHeader->returns_number[2]);
	this->ui.thirdPtN->setPlainText(content);
	content = QString("%1 pts").arg(lasHeader->returns_number[3]);
	this->ui.fourthPtN->setPlainText(content);
	content = QString("%1 pts").arg(lasHeader->returns_number[4]);
	this->ui.fifthPtN->setPlainText(content);

	double density = lasHeader->records_number / ((lasHeader->xMax - lasHeader->xMin) * (lasHeader->yMax - lasHeader->yMin));
	content = QString("%1 pts/m^2").arg(density, 0, 'f', 3);
	this->ui.ptDensity->setPlainText(content);

	this->ui.xMin->setEnabled(false);
	this->ui.xMax->setEnabled(false);
	this->ui.yMin->setEnabled(false);
	this->ui.yMax->setEnabled(false);
	this->ui.zMin->setEnabled(false);
	this->ui.zMax->setEnabled(false);
	this->ui.totalPtN->setEnabled(false);
	this->ui.firstPtN->setEnabled(false);
	this->ui.secondPtN->setEnabled(false);
	this->ui.thirdPtN->setEnabled(false);
	this->ui.fourthPtN->setEnabled(false);
	this->ui.fifthPtN->setEnabled(false);
	this->ui.ptDensity->setEnabled(false);
}

PointInfoDlg::~PointInfoDlg()
{

}
