/********************************************************************************
** Form generated from reading UI file 'entityparamthresholddlg.ui'
**
** Created: Mon Mar 31 08:22:53 2014
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENTITYPARAMTHRESHOLDDLG_H
#define UI_ENTITYPARAMTHRESHOLDDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_EntityParamThresholdDlg
{
public:
    QPushButton *okBtn;
    QPushButton *cancelBtn;
    QLabel *label1;
    QTextEdit *valueMin;
    QTextEdit *valueMax;
    QLabel *label2;

    void setupUi(QDialog *EntityParamThresholdDlg)
    {
        if (EntityParamThresholdDlg->objectName().isEmpty())
            EntityParamThresholdDlg->setObjectName(QString::fromUtf8("EntityParamThresholdDlg"));
        EntityParamThresholdDlg->resize(475, 229);
        okBtn = new QPushButton(EntityParamThresholdDlg);
        okBtn->setObjectName(QString::fromUtf8("okBtn"));
        okBtn->setGeometry(QRect(80, 160, 111, 31));
        cancelBtn = new QPushButton(EntityParamThresholdDlg);
        cancelBtn->setObjectName(QString::fromUtf8("cancelBtn"));
        cancelBtn->setGeometry(QRect(260, 160, 111, 31));
        label1 = new QLabel(EntityParamThresholdDlg);
        label1->setObjectName(QString::fromUtf8("label1"));
        label1->setEnabled(true);
        label1->setGeometry(QRect(30, 50, 241, 21));
        QFont font;
        font.setFamily(QString::fromUtf8("Times New Roman"));
        font.setPointSize(12);
        label1->setFont(font);
        valueMin = new QTextEdit(EntityParamThresholdDlg);
        valueMin->setObjectName(QString::fromUtf8("valueMin"));
        valueMin->setGeometry(QRect(310, 40, 141, 31));
        valueMax = new QTextEdit(EntityParamThresholdDlg);
        valueMax->setObjectName(QString::fromUtf8("valueMax"));
        valueMax->setGeometry(QRect(310, 90, 141, 31));
        label2 = new QLabel(EntityParamThresholdDlg);
        label2->setObjectName(QString::fromUtf8("label2"));
        label2->setEnabled(true);
        label2->setGeometry(QRect(30, 100, 241, 21));
        label2->setFont(font);

        retranslateUi(EntityParamThresholdDlg);

        QMetaObject::connectSlotsByName(EntityParamThresholdDlg);
    } // setupUi

    void retranslateUi(QDialog *EntityParamThresholdDlg)
    {
        EntityParamThresholdDlg->setWindowTitle(QApplication::translate("EntityParamThresholdDlg", "EntityParamThresholdDlg", 0, QApplication::UnicodeUTF8));
        okBtn->setText(QApplication::translate("EntityParamThresholdDlg", "ok", 0, QApplication::UnicodeUTF8));
        cancelBtn->setText(QApplication::translate("EntityParamThresholdDlg", "cancel", 0, QApplication::UnicodeUTF8));
        label1->setText(QApplication::translate("EntityParamThresholdDlg", "value min", 0, QApplication::UnicodeUTF8));
        label2->setText(QApplication::translate("EntityParamThresholdDlg", "value max", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class EntityParamThresholdDlg: public Ui_EntityParamThresholdDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENTITYPARAMTHRESHOLDDLG_H
