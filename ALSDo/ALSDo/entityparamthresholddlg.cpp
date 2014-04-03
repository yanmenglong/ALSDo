#include "entityparamthresholddlg.h"

EntityParamThresholdDlg::EntityParamThresholdDlg(QWidget *parent, const QString &paramTag, const double &value)
	: QDialog(parent)
{
	QString content;
	ui.setupUi(this);

	this->_paramTag = paramTag;
	content = this->ui.label1->text().append(" (").append(this->_paramTag).append(")");
	this->ui.label1->setText(content);
	content = this->ui.label2->text().append(" (").append(this->_paramTag).append(")");
	this->ui.label2->setText(content);

	content = QString("%1").arg(value, 0, 'f', 3);
	this->ui.valueMin->setPlainText(content);
	content = QString("%1").arg(2*value, 0, 'f', 3);
	this->ui.valueMax->setPlainText(content);

	this->connect(this->ui.okBtn, SIGNAL(clicked()), this, SLOT(accept()));
	this->connect(this->ui.cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));
}

EntityParamThresholdDlg::~EntityParamThresholdDlg()
{
}
