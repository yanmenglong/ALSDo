/********************************************************************************
** Form generated from reading UI file 'autorundlg.ui'
**
** Created: Mon Mar 31 08:22:53 2014
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUTORUNDLG_H
#define UI_AUTORUNDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AutoRunDlgClass
{
public:
    QPushButton *runBtn;
    QPushButton *quitBtn;
    QTabWidget *tabWidget;
    QWidget *pathTab;
    QToolButton *projectOpenBtn;
    QLabel *label1_1;
    QLabel *label1_2;
    QToolButton *lasOpenBtn;
    QTextEdit *lasPath;
    QTextEdit *resultFolder;
    QLabel *label1_4;
    QTextEdit *pointDensity;
    QTextEdit *cellSize;
    QLabel *label1_3;
    QWidget *filteringTab;
    QGroupBox *groupBox2_1;
    QTextEdit *seedSlopeTh;
    QTextEdit *seedHeightTh;
    QLabel *label2_3;
    QLabel *label2_2;
    QLabel *label2_4;
    QTextEdit *seedCellSize;
    QCheckBox *useOBASeedGeneratorCheckBox;
    QTextEdit *maxObjSize;
    QLabel *label2_1;
    QWidget *tab;
    QLabel *label3_3;
    QTextEdit *hIteration;
    QLabel *label3_2;
    QTextEdit *angleIteration;
    QTextEdit *filteringSlopeTh;
    QLabel *label3_4;
    QLabel *label3_1;
    QTextEdit *filteringHeightTh;
    QLabel *label3_5;
    QLabel *label3_6;
    QTextEdit *dtmFactor;
    QTextEdit *objectHT;
    QWidget *extractionTab;
    QWidget *tab_2;
    QProgressBar *progressBar;
    QLabel *infoLabel;

    void setupUi(QDialog *AutoRunDlgClass)
    {
        if (AutoRunDlgClass->objectName().isEmpty())
            AutoRunDlgClass->setObjectName(QString::fromUtf8("AutoRunDlgClass"));
        AutoRunDlgClass->resize(713, 414);
        runBtn = new QPushButton(AutoRunDlgClass);
        runBtn->setObjectName(QString::fromUtf8("runBtn"));
        runBtn->setGeometry(QRect(170, 370, 111, 31));
        quitBtn = new QPushButton(AutoRunDlgClass);
        quitBtn->setObjectName(QString::fromUtf8("quitBtn"));
        quitBtn->setGeometry(QRect(380, 370, 111, 31));
        tabWidget = new QTabWidget(AutoRunDlgClass);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(10, 10, 691, 281));
        tabWidget->setElideMode(Qt::ElideLeft);
        tabWidget->setDocumentMode(false);
        tabWidget->setTabsClosable(false);
        tabWidget->setMovable(false);
        pathTab = new QWidget();
        pathTab->setObjectName(QString::fromUtf8("pathTab"));
        projectOpenBtn = new QToolButton(pathTab);
        projectOpenBtn->setObjectName(QString::fromUtf8("projectOpenBtn"));
        projectOpenBtn->setGeometry(QRect(610, 100, 51, 31));
        label1_1 = new QLabel(pathTab);
        label1_1->setObjectName(QString::fromUtf8("label1_1"));
        label1_1->setGeometry(QRect(10, 40, 91, 16));
        QFont font;
        font.setFamily(QString::fromUtf8("Times New Roman"));
        font.setPointSize(12);
        label1_1->setFont(font);
        label1_2 = new QLabel(pathTab);
        label1_2->setObjectName(QString::fromUtf8("label1_2"));
        label1_2->setGeometry(QRect(10, 110, 91, 16));
        label1_2->setFont(font);
        lasOpenBtn = new QToolButton(pathTab);
        lasOpenBtn->setObjectName(QString::fromUtf8("lasOpenBtn"));
        lasOpenBtn->setGeometry(QRect(610, 30, 51, 31));
        lasPath = new QTextEdit(pathTab);
        lasPath->setObjectName(QString::fromUtf8("lasPath"));
        lasPath->setGeometry(QRect(130, 30, 441, 31));
        resultFolder = new QTextEdit(pathTab);
        resultFolder->setObjectName(QString::fromUtf8("resultFolder"));
        resultFolder->setGeometry(QRect(130, 100, 441, 31));
        label1_4 = new QLabel(pathTab);
        label1_4->setObjectName(QString::fromUtf8("label1_4"));
        label1_4->setEnabled(true);
        label1_4->setGeometry(QRect(340, 170, 151, 21));
        label1_4->setFont(font);
        pointDensity = new QTextEdit(pathTab);
        pointDensity->setObjectName(QString::fromUtf8("pointDensity"));
        pointDensity->setEnabled(false);
        pointDensity->setGeometry(QRect(510, 170, 141, 31));
        cellSize = new QTextEdit(pathTab);
        cellSize->setObjectName(QString::fromUtf8("cellSize"));
        cellSize->setGeometry(QRect(170, 170, 141, 31));
        label1_3 = new QLabel(pathTab);
        label1_3->setObjectName(QString::fromUtf8("label1_3"));
        label1_3->setEnabled(true);
        label1_3->setGeometry(QRect(10, 170, 141, 21));
        label1_3->setFont(font);
        tabWidget->addTab(pathTab, QString());
        filteringTab = new QWidget();
        filteringTab->setObjectName(QString::fromUtf8("filteringTab"));
        groupBox2_1 = new QGroupBox(filteringTab);
        groupBox2_1->setObjectName(QString::fromUtf8("groupBox2_1"));
        groupBox2_1->setGeometry(QRect(20, 70, 611, 171));
        seedSlopeTh = new QTextEdit(groupBox2_1);
        seedSlopeTh->setObjectName(QString::fromUtf8("seedSlopeTh"));
        seedSlopeTh->setGeometry(QRect(340, 80, 171, 31));
        seedHeightTh = new QTextEdit(groupBox2_1);
        seedHeightTh->setObjectName(QString::fromUtf8("seedHeightTh"));
        seedHeightTh->setGeometry(QRect(340, 30, 171, 31));
        label2_3 = new QLabel(groupBox2_1);
        label2_3->setObjectName(QString::fromUtf8("label2_3"));
        label2_3->setGeometry(QRect(180, 90, 121, 16));
        label2_3->setFont(font);
        label2_2 = new QLabel(groupBox2_1);
        label2_2->setObjectName(QString::fromUtf8("label2_2"));
        label2_2->setGeometry(QRect(180, 40, 121, 16));
        label2_2->setFont(font);
        label2_4 = new QLabel(groupBox2_1);
        label2_4->setObjectName(QString::fromUtf8("label2_4"));
        label2_4->setGeometry(QRect(180, 140, 121, 16));
        label2_4->setFont(font);
        seedCellSize = new QTextEdit(groupBox2_1);
        seedCellSize->setObjectName(QString::fromUtf8("seedCellSize"));
        seedCellSize->setGeometry(QRect(340, 130, 171, 31));
        useOBASeedGeneratorCheckBox = new QCheckBox(groupBox2_1);
        useOBASeedGeneratorCheckBox->setObjectName(QString::fromUtf8("useOBASeedGeneratorCheckBox"));
        useOBASeedGeneratorCheckBox->setGeometry(QRect(10, 20, 111, 41));
        maxObjSize = new QTextEdit(filteringTab);
        maxObjSize->setObjectName(QString::fromUtf8("maxObjSize"));
        maxObjSize->setGeometry(QRect(200, 20, 181, 31));
        label2_1 = new QLabel(filteringTab);
        label2_1->setObjectName(QString::fromUtf8("label2_1"));
        label2_1->setGeometry(QRect(50, 30, 121, 16));
        label2_1->setFont(font);
        tabWidget->addTab(filteringTab, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        label3_3 = new QLabel(tab);
        label3_3->setObjectName(QString::fromUtf8("label3_3"));
        label3_3->setEnabled(true);
        label3_3->setGeometry(QRect(10, 100, 141, 21));
        label3_3->setFont(font);
        hIteration = new QTextEdit(tab);
        hIteration->setObjectName(QString::fromUtf8("hIteration"));
        hIteration->setGeometry(QRect(180, 30, 141, 31));
        label3_2 = new QLabel(tab);
        label3_2->setObjectName(QString::fromUtf8("label3_2"));
        label3_2->setEnabled(true);
        label3_2->setGeometry(QRect(350, 40, 141, 21));
        label3_2->setFont(font);
        angleIteration = new QTextEdit(tab);
        angleIteration->setObjectName(QString::fromUtf8("angleIteration"));
        angleIteration->setGeometry(QRect(520, 30, 141, 31));
        filteringSlopeTh = new QTextEdit(tab);
        filteringSlopeTh->setObjectName(QString::fromUtf8("filteringSlopeTh"));
        filteringSlopeTh->setGeometry(QRect(520, 90, 141, 31));
        label3_4 = new QLabel(tab);
        label3_4->setObjectName(QString::fromUtf8("label3_4"));
        label3_4->setEnabled(true);
        label3_4->setGeometry(QRect(350, 100, 141, 21));
        label3_4->setFont(font);
        label3_1 = new QLabel(tab);
        label3_1->setObjectName(QString::fromUtf8("label3_1"));
        label3_1->setEnabled(true);
        label3_1->setGeometry(QRect(10, 40, 141, 21));
        label3_1->setFont(font);
        filteringHeightTh = new QTextEdit(tab);
        filteringHeightTh->setObjectName(QString::fromUtf8("filteringHeightTh"));
        filteringHeightTh->setGeometry(QRect(180, 90, 141, 31));
        label3_5 = new QLabel(tab);
        label3_5->setObjectName(QString::fromUtf8("label3_5"));
        label3_5->setEnabled(true);
        label3_5->setGeometry(QRect(10, 160, 141, 21));
        label3_5->setFont(font);
        label3_6 = new QLabel(tab);
        label3_6->setObjectName(QString::fromUtf8("label3_6"));
        label3_6->setEnabled(true);
        label3_6->setGeometry(QRect(350, 160, 141, 21));
        label3_6->setFont(font);
        dtmFactor = new QTextEdit(tab);
        dtmFactor->setObjectName(QString::fromUtf8("dtmFactor"));
        dtmFactor->setGeometry(QRect(180, 160, 141, 31));
        objectHT = new QTextEdit(tab);
        objectHT->setObjectName(QString::fromUtf8("objectHT"));
        objectHT->setGeometry(QRect(520, 160, 141, 31));
        tabWidget->addTab(tab, QString());
        extractionTab = new QWidget();
        extractionTab->setObjectName(QString::fromUtf8("extractionTab"));
        tabWidget->addTab(extractionTab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tabWidget->addTab(tab_2, QString());
        progressBar = new QProgressBar(AutoRunDlgClass);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(310, 310, 391, 23));
        progressBar->setValue(24);
        infoLabel = new QLabel(AutoRunDlgClass);
        infoLabel->setObjectName(QString::fromUtf8("infoLabel"));
        infoLabel->setGeometry(QRect(20, 310, 261, 16));

        retranslateUi(AutoRunDlgClass);

        tabWidget->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(AutoRunDlgClass);
    } // setupUi

    void retranslateUi(QDialog *AutoRunDlgClass)
    {
        AutoRunDlgClass->setWindowTitle(QApplication::translate("AutoRunDlgClass", "AutoRunDlg", 0, QApplication::UnicodeUTF8));
        runBtn->setText(QApplication::translate("AutoRunDlgClass", "run", 0, QApplication::UnicodeUTF8));
        quitBtn->setText(QApplication::translate("AutoRunDlgClass", "quit", 0, QApplication::UnicodeUTF8));
        projectOpenBtn->setText(QApplication::translate("AutoRunDlgClass", "...", 0, QApplication::UnicodeUTF8));
        label1_1->setText(QApplication::translate("AutoRunDlgClass", "las path", 0, QApplication::UnicodeUTF8));
        label1_2->setText(QApplication::translate("AutoRunDlgClass", "result folder", 0, QApplication::UnicodeUTF8));
        lasOpenBtn->setText(QApplication::translate("AutoRunDlgClass", "...", 0, QApplication::UnicodeUTF8));
        label1_4->setText(QApplication::translate("AutoRunDlgClass", "point desity of resample", 0, QApplication::UnicodeUTF8));
        label1_3->setText(QApplication::translate("AutoRunDlgClass", "cell size of grid index", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(pathTab), QApplication::translate("AutoRunDlgClass", "path", 0, QApplication::UnicodeUTF8));
        groupBox2_1->setTitle(QApplication::translate("AutoRunDlgClass", "threshold for scene complex detection and seeds generation", 0, QApplication::UnicodeUTF8));
        label2_3->setText(QApplication::translate("AutoRunDlgClass", "slope threshold", 0, QApplication::UnicodeUTF8));
        label2_2->setText(QApplication::translate("AutoRunDlgClass", "height threshold", 0, QApplication::UnicodeUTF8));
        label2_4->setText(QApplication::translate("AutoRunDlgClass", "small seed cell size", 0, QApplication::UnicodeUTF8));
        useOBASeedGeneratorCheckBox->setText(QApplication::translate("AutoRunDlgClass", "using advanced\n"
"seeds generator", 0, QApplication::UnicodeUTF8));
        label2_1->setText(QApplication::translate("AutoRunDlgClass", "max object size", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(filteringTab), QApplication::translate("AutoRunDlgClass", "seed generation", 0, QApplication::UnicodeUTF8));
        label3_3->setText(QApplication::translate("AutoRunDlgClass", "local height threshold", 0, QApplication::UnicodeUTF8));
        label3_2->setText(QApplication::translate("AutoRunDlgClass", "angle iteration", 0, QApplication::UnicodeUTF8));
        label3_4->setText(QApplication::translate("AutoRunDlgClass", "local angle threshold", 0, QApplication::UnicodeUTF8));
        label3_1->setText(QApplication::translate("AutoRunDlgClass", "height iteration", 0, QApplication::UnicodeUTF8));
        label3_5->setText(QApplication::translate("AutoRunDlgClass", "dtm resolution factor", 0, QApplication::UnicodeUTF8));
        label3_6->setText(QApplication::translate("AutoRunDlgClass", "object height threshold", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("AutoRunDlgClass", "filtering", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(extractionTab), QApplication::translate("AutoRunDlgClass", "classification", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("AutoRunDlgClass", "building extraction", 0, QApplication::UnicodeUTF8));
        infoLabel->setText(QApplication::translate("AutoRunDlgClass", "information", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AutoRunDlgClass: public Ui_AutoRunDlgClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUTORUNDLG_H
