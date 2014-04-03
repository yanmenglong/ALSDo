/********************************************************************************
** Form generated from reading UI file 'pointinfodlg.ui'
**
** Created: Mon Mar 31 08:22:53 2014
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POINTINFODLG_H
#define UI_POINTINFODLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_PointInfoDlgClass
{
public:
    QLabel *xMinLabel;
    QPlainTextEdit *xMin;
    QPlainTextEdit *xMax;
    QLabel *xMaxLabel;
    QPlainTextEdit *yMin;
    QLabel *yMaxLabel;
    QLabel *yMinLabel;
    QPlainTextEdit *yMax;
    QPlainTextEdit *zMax;
    QLabel *zMinLabel;
    QLabel *zMaxLabel;
    QPlainTextEdit *zMin;
    QLabel *totalPtNLabel;
    QPlainTextEdit *totalPtN;
    QPlainTextEdit *firstPtN;
    QLabel *firstPtNLabel;
    QPlainTextEdit *secondPtN;
    QLabel *secondPtNLabel;
    QPlainTextEdit *thirdPtN;
    QLabel *thirdPtNLabel;
    QPlainTextEdit *fourthPtN;
    QLabel *fourthPtNLabel;
    QPlainTextEdit *fifthPtN;
    QLabel *fifthPtNLabel;
    QPlainTextEdit *ptDensity;
    QLabel *ptDensityLabel;
    QPushButton *quitBtn;

    void setupUi(QDialog *PointInfoDlgClass)
    {
        if (PointInfoDlgClass->objectName().isEmpty())
            PointInfoDlgClass->setObjectName(QString::fromUtf8("PointInfoDlgClass"));
        PointInfoDlgClass->resize(455, 589);
        xMinLabel = new QLabel(PointInfoDlgClass);
        xMinLabel->setObjectName(QString::fromUtf8("xMinLabel"));
        xMinLabel->setGeometry(QRect(30, 20, 54, 12));
        xMin = new QPlainTextEdit(PointInfoDlgClass);
        xMin->setObjectName(QString::fromUtf8("xMin"));
        xMin->setGeometry(QRect(80, 10, 131, 31));
        xMax = new QPlainTextEdit(PointInfoDlgClass);
        xMax->setObjectName(QString::fromUtf8("xMax"));
        xMax->setGeometry(QRect(290, 10, 131, 31));
        xMaxLabel = new QLabel(PointInfoDlgClass);
        xMaxLabel->setObjectName(QString::fromUtf8("xMaxLabel"));
        xMaxLabel->setGeometry(QRect(240, 20, 54, 12));
        yMin = new QPlainTextEdit(PointInfoDlgClass);
        yMin->setObjectName(QString::fromUtf8("yMin"));
        yMin->setGeometry(QRect(80, 60, 131, 31));
        yMaxLabel = new QLabel(PointInfoDlgClass);
        yMaxLabel->setObjectName(QString::fromUtf8("yMaxLabel"));
        yMaxLabel->setGeometry(QRect(240, 70, 54, 12));
        yMinLabel = new QLabel(PointInfoDlgClass);
        yMinLabel->setObjectName(QString::fromUtf8("yMinLabel"));
        yMinLabel->setGeometry(QRect(30, 70, 54, 12));
        yMax = new QPlainTextEdit(PointInfoDlgClass);
        yMax->setObjectName(QString::fromUtf8("yMax"));
        yMax->setGeometry(QRect(290, 60, 131, 31));
        zMax = new QPlainTextEdit(PointInfoDlgClass);
        zMax->setObjectName(QString::fromUtf8("zMax"));
        zMax->setGeometry(QRect(290, 110, 131, 31));
        zMinLabel = new QLabel(PointInfoDlgClass);
        zMinLabel->setObjectName(QString::fromUtf8("zMinLabel"));
        zMinLabel->setGeometry(QRect(30, 120, 54, 12));
        zMaxLabel = new QLabel(PointInfoDlgClass);
        zMaxLabel->setObjectName(QString::fromUtf8("zMaxLabel"));
        zMaxLabel->setGeometry(QRect(240, 120, 54, 12));
        zMin = new QPlainTextEdit(PointInfoDlgClass);
        zMin->setObjectName(QString::fromUtf8("zMin"));
        zMin->setGeometry(QRect(80, 110, 131, 31));
        totalPtNLabel = new QLabel(PointInfoDlgClass);
        totalPtNLabel->setObjectName(QString::fromUtf8("totalPtNLabel"));
        totalPtNLabel->setGeometry(QRect(30, 180, 81, 16));
        totalPtN = new QPlainTextEdit(PointInfoDlgClass);
        totalPtN->setObjectName(QString::fromUtf8("totalPtN"));
        totalPtN->setGeometry(QRect(140, 170, 131, 31));
        firstPtN = new QPlainTextEdit(PointInfoDlgClass);
        firstPtN->setObjectName(QString::fromUtf8("firstPtN"));
        firstPtN->setGeometry(QRect(140, 220, 131, 31));
        firstPtNLabel = new QLabel(PointInfoDlgClass);
        firstPtNLabel->setObjectName(QString::fromUtf8("firstPtNLabel"));
        firstPtNLabel->setGeometry(QRect(30, 230, 81, 16));
        secondPtN = new QPlainTextEdit(PointInfoDlgClass);
        secondPtN->setObjectName(QString::fromUtf8("secondPtN"));
        secondPtN->setGeometry(QRect(140, 270, 131, 31));
        secondPtNLabel = new QLabel(PointInfoDlgClass);
        secondPtNLabel->setObjectName(QString::fromUtf8("secondPtNLabel"));
        secondPtNLabel->setGeometry(QRect(30, 280, 81, 16));
        thirdPtN = new QPlainTextEdit(PointInfoDlgClass);
        thirdPtN->setObjectName(QString::fromUtf8("thirdPtN"));
        thirdPtN->setGeometry(QRect(140, 320, 131, 31));
        thirdPtNLabel = new QLabel(PointInfoDlgClass);
        thirdPtNLabel->setObjectName(QString::fromUtf8("thirdPtNLabel"));
        thirdPtNLabel->setGeometry(QRect(30, 330, 81, 16));
        fourthPtN = new QPlainTextEdit(PointInfoDlgClass);
        fourthPtN->setObjectName(QString::fromUtf8("fourthPtN"));
        fourthPtN->setGeometry(QRect(140, 370, 131, 31));
        fourthPtNLabel = new QLabel(PointInfoDlgClass);
        fourthPtNLabel->setObjectName(QString::fromUtf8("fourthPtNLabel"));
        fourthPtNLabel->setGeometry(QRect(30, 380, 81, 16));
        fifthPtN = new QPlainTextEdit(PointInfoDlgClass);
        fifthPtN->setObjectName(QString::fromUtf8("fifthPtN"));
        fifthPtN->setGeometry(QRect(140, 420, 131, 31));
        fifthPtNLabel = new QLabel(PointInfoDlgClass);
        fifthPtNLabel->setObjectName(QString::fromUtf8("fifthPtNLabel"));
        fifthPtNLabel->setGeometry(QRect(30, 430, 81, 16));
        ptDensity = new QPlainTextEdit(PointInfoDlgClass);
        ptDensity->setObjectName(QString::fromUtf8("ptDensity"));
        ptDensity->setGeometry(QRect(140, 470, 131, 31));
        ptDensityLabel = new QLabel(PointInfoDlgClass);
        ptDensityLabel->setObjectName(QString::fromUtf8("ptDensityLabel"));
        ptDensityLabel->setGeometry(QRect(30, 480, 81, 16));
        quitBtn = new QPushButton(PointInfoDlgClass);
        quitBtn->setObjectName(QString::fromUtf8("quitBtn"));
        quitBtn->setGeometry(QRect(180, 540, 75, 31));

        retranslateUi(PointInfoDlgClass);
        QObject::connect(quitBtn, SIGNAL(clicked()), PointInfoDlgClass, SLOT(close()));

        QMetaObject::connectSlotsByName(PointInfoDlgClass);
    } // setupUi

    void retranslateUi(QDialog *PointInfoDlgClass)
    {
        PointInfoDlgClass->setWindowTitle(QApplication::translate("PointInfoDlgClass", "PointInfoDlg", 0, QApplication::UnicodeUTF8));
        xMinLabel->setText(QApplication::translate("PointInfoDlgClass", "xmin", 0, QApplication::UnicodeUTF8));
        xMaxLabel->setText(QApplication::translate("PointInfoDlgClass", "xmax", 0, QApplication::UnicodeUTF8));
        yMaxLabel->setText(QApplication::translate("PointInfoDlgClass", "ymax", 0, QApplication::UnicodeUTF8));
        yMinLabel->setText(QApplication::translate("PointInfoDlgClass", "ymin", 0, QApplication::UnicodeUTF8));
        zMinLabel->setText(QApplication::translate("PointInfoDlgClass", "zmin", 0, QApplication::UnicodeUTF8));
        zMaxLabel->setText(QApplication::translate("PointInfoDlgClass", "zmax", 0, QApplication::UnicodeUTF8));
        totalPtNLabel->setText(QApplication::translate("PointInfoDlgClass", "total points", 0, QApplication::UnicodeUTF8));
        firstPtNLabel->setText(QApplication::translate("PointInfoDlgClass", "first return", 0, QApplication::UnicodeUTF8));
        secondPtNLabel->setText(QApplication::translate("PointInfoDlgClass", "second return", 0, QApplication::UnicodeUTF8));
        thirdPtNLabel->setText(QApplication::translate("PointInfoDlgClass", "third return", 0, QApplication::UnicodeUTF8));
        fourthPtNLabel->setText(QApplication::translate("PointInfoDlgClass", "fourth return", 0, QApplication::UnicodeUTF8));
        fifthPtNLabel->setText(QApplication::translate("PointInfoDlgClass", "fifth return", 0, QApplication::UnicodeUTF8));
        ptDensityLabel->setText(QApplication::translate("PointInfoDlgClass", "point density", 0, QApplication::UnicodeUTF8));
        quitBtn->setText(QApplication::translate("PointInfoDlgClass", "quit", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PointInfoDlgClass: public Ui_PointInfoDlgClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POINTINFODLG_H
