/********************************************************************************
** Form generated from reading UI file 'las2textdlg.ui'
**
** Created: Mon Mar 31 08:22:53 2014
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LAS2TEXTDLG_H
#define UI_LAS2TEXTDLG_H

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

class Ui_las2textDlgClass
{
public:
    QPushButton *textOpenBtn;
    QPushButton *lasOpenBtn;
    QTextEdit *textPath;
    QLabel *label1;
    QTextEdit *lasPath;
    QLabel *label2;
    QPushButton *startBtn;
    QPushButton *quitBtn;

    void setupUi(QDialog *las2textDlgClass)
    {
        if (las2textDlgClass->objectName().isEmpty())
            las2textDlgClass->setObjectName(QString::fromUtf8("las2textDlgClass"));
        las2textDlgClass->resize(567, 190);
        textOpenBtn = new QPushButton(las2textDlgClass);
        textOpenBtn->setObjectName(QString::fromUtf8("textOpenBtn"));
        textOpenBtn->setGeometry(QRect(470, 82, 75, 31));
        lasOpenBtn = new QPushButton(las2textDlgClass);
        lasOpenBtn->setObjectName(QString::fromUtf8("lasOpenBtn"));
        lasOpenBtn->setGeometry(QRect(470, 22, 75, 31));
        textPath = new QTextEdit(las2textDlgClass);
        textPath->setObjectName(QString::fromUtf8("textPath"));
        textPath->setGeometry(QRect(150, 80, 291, 31));
        label1 = new QLabel(las2textDlgClass);
        label1->setObjectName(QString::fromUtf8("label1"));
        label1->setGeometry(QRect(20, 90, 111, 16));
        lasPath = new QTextEdit(las2textDlgClass);
        lasPath->setObjectName(QString::fromUtf8("lasPath"));
        lasPath->setGeometry(QRect(150, 20, 291, 31));
        label2 = new QLabel(las2textDlgClass);
        label2->setObjectName(QString::fromUtf8("label2"));
        label2->setGeometry(QRect(20, 30, 111, 16));
        startBtn = new QPushButton(las2textDlgClass);
        startBtn->setObjectName(QString::fromUtf8("startBtn"));
        startBtn->setGeometry(QRect(140, 142, 75, 31));
        quitBtn = new QPushButton(las2textDlgClass);
        quitBtn->setObjectName(QString::fromUtf8("quitBtn"));
        quitBtn->setGeometry(QRect(350, 140, 75, 31));

        retranslateUi(las2textDlgClass);

        QMetaObject::connectSlotsByName(las2textDlgClass);
    } // setupUi

    void retranslateUi(QDialog *las2textDlgClass)
    {
        las2textDlgClass->setWindowTitle(QApplication::translate("las2textDlgClass", "las2textdlg", 0, QApplication::UnicodeUTF8));
        textOpenBtn->setText(QApplication::translate("las2textDlgClass", "...", 0, QApplication::UnicodeUTF8));
        lasOpenBtn->setText(QApplication::translate("las2textDlgClass", "...", 0, QApplication::UnicodeUTF8));
        label1->setText(QApplication::translate("las2textDlgClass", "output text path", 0, QApplication::UnicodeUTF8));
        label2->setText(QApplication::translate("las2textDlgClass", "input las path", 0, QApplication::UnicodeUTF8));
        startBtn->setText(QApplication::translate("las2textDlgClass", "start", 0, QApplication::UnicodeUTF8));
        quitBtn->setText(QApplication::translate("las2textDlgClass", "quit", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class las2textDlgClass: public Ui_las2textDlgClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LAS2TEXTDLG_H
