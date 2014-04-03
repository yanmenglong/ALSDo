/********************************************************************************
** Form generated from reading UI file 'obafilterparamdlg.ui'
**
** Created: Mon Mar 31 08:22:53 2014
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OBAFILTERPARAMDLG_H
#define UI_OBAFILTERPARAMDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OBAFilterParamDlg
{
public:
    QPushButton *okBtn;
    QPushButton *cancelBtn;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGroupBox *groupBox1_1;
    QTextEdit *seedSlopeTh;
    QTextEdit *seedHeightTh;
    QLabel *label1_2;
    QLabel *label1_1;
    QGroupBox *groupBox1_2;
    QTextEdit *seedCellSize;
    QTextEdit *maxObjSize;
    QLabel *label1_4;
    QLabel *label1_3;
    QWidget *tab_2;
    QGroupBox *groupBox2_1;
    QTextEdit *segSlopeTh;
    QTextEdit *segHeightTh;
    QLabel *label2_2;
    QLabel *label2_1;
    QWidget *tab_3;
    QGroupBox *groupBox3_1;
    QTextEdit *filteringSlopeTh;
    QLabel *label3_1;
    QTextEdit *filteringHeightTh;
    QLabel *label3_2;
    QLabel *label3_3;
    QTextEdit *dtmFactor;
    QLabel *label3_4;
    QTextEdit *objectHT;

    void setupUi(QWidget *OBAFilterParamDlg)
    {
        if (OBAFilterParamDlg->objectName().isEmpty())
            OBAFilterParamDlg->setObjectName(QString::fromUtf8("OBAFilterParamDlg"));
        OBAFilterParamDlg->resize(625, 398);
        okBtn = new QPushButton(OBAFilterParamDlg);
        okBtn->setObjectName(QString::fromUtf8("okBtn"));
        okBtn->setGeometry(QRect(150, 350, 111, 31));
        cancelBtn = new QPushButton(OBAFilterParamDlg);
        cancelBtn->setObjectName(QString::fromUtf8("cancelBtn"));
        cancelBtn->setGeometry(QRect(330, 350, 111, 31));
        tabWidget = new QTabWidget(OBAFilterParamDlg);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(10, 10, 601, 321));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        groupBox1_1 = new QGroupBox(tab);
        groupBox1_1->setObjectName(QString::fromUtf8("groupBox1_1"));
        groupBox1_1->setGeometry(QRect(10, 10, 571, 121));
        seedSlopeTh = new QTextEdit(groupBox1_1);
        seedSlopeTh->setObjectName(QString::fromUtf8("seedSlopeTh"));
        seedSlopeTh->setGeometry(QRect(180, 70, 201, 31));
        seedHeightTh = new QTextEdit(groupBox1_1);
        seedHeightTh->setObjectName(QString::fromUtf8("seedHeightTh"));
        seedHeightTh->setGeometry(QRect(180, 20, 201, 31));
        label1_2 = new QLabel(groupBox1_1);
        label1_2->setObjectName(QString::fromUtf8("label1_2"));
        label1_2->setGeometry(QRect(20, 80, 121, 16));
        QFont font;
        font.setFamily(QString::fromUtf8("Times New Roman"));
        font.setPointSize(12);
        label1_2->setFont(font);
        label1_1 = new QLabel(groupBox1_1);
        label1_1->setObjectName(QString::fromUtf8("label1_1"));
        label1_1->setGeometry(QRect(20, 30, 121, 16));
        label1_1->setFont(font);
        groupBox1_2 = new QGroupBox(tab);
        groupBox1_2->setObjectName(QString::fromUtf8("groupBox1_2"));
        groupBox1_2->setGeometry(QRect(10, 140, 571, 141));
        seedCellSize = new QTextEdit(groupBox1_2);
        seedCellSize->setObjectName(QString::fromUtf8("seedCellSize"));
        seedCellSize->setGeometry(QRect(180, 80, 201, 31));
        maxObjSize = new QTextEdit(groupBox1_2);
        maxObjSize->setObjectName(QString::fromUtf8("maxObjSize"));
        maxObjSize->setGeometry(QRect(180, 30, 201, 31));
        label1_4 = new QLabel(groupBox1_2);
        label1_4->setObjectName(QString::fromUtf8("label1_4"));
        label1_4->setGeometry(QRect(20, 90, 121, 16));
        label1_4->setFont(font);
        label1_3 = new QLabel(groupBox1_2);
        label1_3->setObjectName(QString::fromUtf8("label1_3"));
        label1_3->setGeometry(QRect(20, 40, 121, 16));
        label1_3->setFont(font);
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        groupBox2_1 = new QGroupBox(tab_2);
        groupBox2_1->setObjectName(QString::fromUtf8("groupBox2_1"));
        groupBox2_1->setGeometry(QRect(10, 20, 571, 121));
        segSlopeTh = new QTextEdit(groupBox2_1);
        segSlopeTh->setObjectName(QString::fromUtf8("segSlopeTh"));
        segSlopeTh->setGeometry(QRect(180, 70, 201, 31));
        segHeightTh = new QTextEdit(groupBox2_1);
        segHeightTh->setObjectName(QString::fromUtf8("segHeightTh"));
        segHeightTh->setGeometry(QRect(180, 20, 201, 31));
        label2_2 = new QLabel(groupBox2_1);
        label2_2->setObjectName(QString::fromUtf8("label2_2"));
        label2_2->setGeometry(QRect(20, 80, 121, 16));
        label2_2->setFont(font);
        label2_1 = new QLabel(groupBox2_1);
        label2_1->setObjectName(QString::fromUtf8("label2_1"));
        label2_1->setGeometry(QRect(20, 30, 121, 16));
        label2_1->setFont(font);
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        groupBox3_1 = new QGroupBox(tab_3);
        groupBox3_1->setObjectName(QString::fromUtf8("groupBox3_1"));
        groupBox3_1->setGeometry(QRect(10, 10, 571, 141));
        filteringSlopeTh = new QTextEdit(groupBox3_1);
        filteringSlopeTh->setObjectName(QString::fromUtf8("filteringSlopeTh"));
        filteringSlopeTh->setGeometry(QRect(180, 90, 201, 31));
        label3_1 = new QLabel(groupBox3_1);
        label3_1->setObjectName(QString::fromUtf8("label3_1"));
        label3_1->setGeometry(QRect(20, 50, 141, 16));
        label3_1->setFont(font);
        filteringHeightTh = new QTextEdit(groupBox3_1);
        filteringHeightTh->setObjectName(QString::fromUtf8("filteringHeightTh"));
        filteringHeightTh->setGeometry(QRect(180, 40, 201, 31));
        label3_2 = new QLabel(groupBox3_1);
        label3_2->setObjectName(QString::fromUtf8("label3_2"));
        label3_2->setGeometry(QRect(20, 100, 141, 16));
        label3_2->setFont(font);
        label3_3 = new QLabel(tab_3);
        label3_3->setObjectName(QString::fromUtf8("label3_3"));
        label3_3->setEnabled(true);
        label3_3->setGeometry(QRect(20, 190, 141, 21));
        label3_3->setFont(font);
        dtmFactor = new QTextEdit(tab_3);
        dtmFactor->setObjectName(QString::fromUtf8("dtmFactor"));
        dtmFactor->setGeometry(QRect(190, 190, 141, 31));
        label3_4 = new QLabel(tab_3);
        label3_4->setObjectName(QString::fromUtf8("label3_4"));
        label3_4->setEnabled(true);
        label3_4->setGeometry(QRect(20, 240, 141, 21));
        label3_4->setFont(font);
        objectHT = new QTextEdit(tab_3);
        objectHT->setObjectName(QString::fromUtf8("objectHT"));
        objectHT->setGeometry(QRect(190, 240, 141, 31));
        tabWidget->addTab(tab_3, QString());

        retranslateUi(OBAFilterParamDlg);

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(OBAFilterParamDlg);
    } // setupUi

    void retranslateUi(QWidget *OBAFilterParamDlg)
    {
        OBAFilterParamDlg->setWindowTitle(QApplication::translate("OBAFilterParamDlg", "OBAFilterParamDlg", 0, QApplication::UnicodeUTF8));
        okBtn->setText(QApplication::translate("OBAFilterParamDlg", "ok", 0, QApplication::UnicodeUTF8));
        cancelBtn->setText(QApplication::translate("OBAFilterParamDlg", "cancel", 0, QApplication::UnicodeUTF8));
        groupBox1_1->setTitle(QApplication::translate("OBAFilterParamDlg", "threshold for scene complex detection", 0, QApplication::UnicodeUTF8));
        label1_2->setText(QApplication::translate("OBAFilterParamDlg", "slope threshold", 0, QApplication::UnicodeUTF8));
        label1_1->setText(QApplication::translate("OBAFilterParamDlg", "height threshold", 0, QApplication::UnicodeUTF8));
        groupBox1_2->setTitle(QApplication::translate("OBAFilterParamDlg", "window size for seeds generation", 0, QApplication::UnicodeUTF8));
        label1_4->setText(QApplication::translate("OBAFilterParamDlg", "small seed cell size", 0, QApplication::UnicodeUTF8));
        label1_3->setText(QApplication::translate("OBAFilterParamDlg", "max object size", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("OBAFilterParamDlg", "oba seed generation", 0, QApplication::UnicodeUTF8));
        groupBox2_1->setTitle(QApplication::translate("OBAFilterParamDlg", "threshold for OBA segmentation", 0, QApplication::UnicodeUTF8));
        label2_2->setText(QApplication::translate("OBAFilterParamDlg", "slope threshold", 0, QApplication::UnicodeUTF8));
        label2_1->setText(QApplication::translate("OBAFilterParamDlg", "height threshold", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("OBAFilterParamDlg", "oba segmentation", 0, QApplication::UnicodeUTF8));
        groupBox3_1->setTitle(QApplication::translate("OBAFilterParamDlg", "threshold for filtering las points", 0, QApplication::UnicodeUTF8));
        label3_1->setText(QApplication::translate("OBAFilterParamDlg", "local height threshold", 0, QApplication::UnicodeUTF8));
        label3_2->setText(QApplication::translate("OBAFilterParamDlg", "local slope threshold", 0, QApplication::UnicodeUTF8));
        label3_3->setText(QApplication::translate("OBAFilterParamDlg", "dtm resolution factor", 0, QApplication::UnicodeUTF8));
        label3_4->setText(QApplication::translate("OBAFilterParamDlg", "object height threshold", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("OBAFilterParamDlg", "filter points", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class OBAFilterParamDlg: public Ui_OBAFilterParamDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OBAFILTERPARAMDLG_H
