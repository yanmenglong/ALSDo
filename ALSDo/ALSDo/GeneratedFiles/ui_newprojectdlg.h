/********************************************************************************
** Form generated from reading UI file 'newprojectdlg.ui'
**
** Created: Mon Mar 31 08:22:53 2014
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWPROJECTDLG_H
#define UI_NEWPROJECTDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NewProjectDlgClass
{
public:
    QTabWidget *tabWidget;
    QWidget *tab;
    QToolButton *lasOpenBtn;
    QLabel *label1;
    QLabel *label2;
    QTextEdit *lasPath;
    QTextEdit *resultDir;
    QToolButton *projectOpenBtn;
    QWidget *tab_2;
    QLabel *label3;
    QTextEdit *gridSize;
    QTextEdit *pointDensity;
    QLabel *label4;
    QPushButton *okBtn;
    QPushButton *cancelBtn;

    void setupUi(QDialog *NewProjectDlgClass)
    {
        if (NewProjectDlgClass->objectName().isEmpty())
            NewProjectDlgClass->setObjectName(QString::fromUtf8("NewProjectDlgClass"));
        NewProjectDlgClass->resize(649, 276);
        tabWidget = new QTabWidget(NewProjectDlgClass);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setEnabled(true);
        tabWidget->setGeometry(QRect(10, 0, 631, 221));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        lasOpenBtn = new QToolButton(tab);
        lasOpenBtn->setObjectName(QString::fromUtf8("lasOpenBtn"));
        lasOpenBtn->setGeometry(QRect(556, 40, 51, 31));
        label1 = new QLabel(tab);
        label1->setObjectName(QString::fromUtf8("label1"));
        label1->setGeometry(QRect(20, 50, 91, 16));
        QFont font;
        font.setFamily(QString::fromUtf8("Times New Roman"));
        font.setPointSize(12);
        label1->setFont(font);
        label2 = new QLabel(tab);
        label2->setObjectName(QString::fromUtf8("label2"));
        label2->setGeometry(QRect(20, 110, 91, 16));
        label2->setFont(font);
        lasPath = new QTextEdit(tab);
        lasPath->setObjectName(QString::fromUtf8("lasPath"));
        lasPath->setGeometry(QRect(140, 40, 391, 31));
        resultDir = new QTextEdit(tab);
        resultDir->setObjectName(QString::fromUtf8("resultDir"));
        resultDir->setGeometry(QRect(140, 110, 391, 31));
        projectOpenBtn = new QToolButton(tab);
        projectOpenBtn->setObjectName(QString::fromUtf8("projectOpenBtn"));
        projectOpenBtn->setGeometry(QRect(560, 110, 51, 31));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        label3 = new QLabel(tab_2);
        label3->setObjectName(QString::fromUtf8("label3"));
        label3->setEnabled(true);
        label3->setGeometry(QRect(20, 20, 141, 21));
        label3->setFont(font);
        gridSize = new QTextEdit(tab_2);
        gridSize->setObjectName(QString::fromUtf8("gridSize"));
        gridSize->setGeometry(QRect(190, 10, 141, 31));
        pointDensity = new QTextEdit(tab_2);
        pointDensity->setObjectName(QString::fromUtf8("pointDensity"));
        pointDensity->setEnabled(false);
        pointDensity->setGeometry(QRect(190, 70, 141, 31));
        label4 = new QLabel(tab_2);
        label4->setObjectName(QString::fromUtf8("label4"));
        label4->setEnabled(true);
        label4->setGeometry(QRect(20, 70, 151, 21));
        label4->setFont(font);
        tabWidget->addTab(tab_2, QString());
        okBtn = new QPushButton(NewProjectDlgClass);
        okBtn->setObjectName(QString::fromUtf8("okBtn"));
        okBtn->setGeometry(QRect(180, 230, 75, 31));
        cancelBtn = new QPushButton(NewProjectDlgClass);
        cancelBtn->setObjectName(QString::fromUtf8("cancelBtn"));
        cancelBtn->setGeometry(QRect(370, 230, 75, 31));

        retranslateUi(NewProjectDlgClass);
        QObject::connect(okBtn, SIGNAL(clicked()), NewProjectDlgClass, SLOT(accept()));
        QObject::connect(cancelBtn, SIGNAL(clicked()), NewProjectDlgClass, SLOT(reject()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(NewProjectDlgClass);
    } // setupUi

    void retranslateUi(QDialog *NewProjectDlgClass)
    {
        NewProjectDlgClass->setWindowTitle(QApplication::translate("NewProjectDlgClass", "NewProjectDlg", 0, QApplication::UnicodeUTF8));
        lasOpenBtn->setText(QApplication::translate("NewProjectDlgClass", "...", 0, QApplication::UnicodeUTF8));
        label1->setText(QApplication::translate("NewProjectDlgClass", "las path", 0, QApplication::UnicodeUTF8));
        label2->setText(QApplication::translate("NewProjectDlgClass", "result folder", 0, QApplication::UnicodeUTF8));
        projectOpenBtn->setText(QApplication::translate("NewProjectDlgClass", "...", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("NewProjectDlgClass", "path", 0, QApplication::UnicodeUTF8));
        label3->setText(QApplication::translate("NewProjectDlgClass", "grid size for resample", 0, QApplication::UnicodeUTF8));
        label4->setText(QApplication::translate("NewProjectDlgClass", "point desity of resample", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("NewProjectDlgClass", "advanced setting", 0, QApplication::UnicodeUTF8));
        okBtn->setText(QApplication::translate("NewProjectDlgClass", "OK", 0, QApplication::UnicodeUTF8));
        cancelBtn->setText(QApplication::translate("NewProjectDlgClass", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class NewProjectDlgClass: public Ui_NewProjectDlgClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWPROJECTDLG_H
