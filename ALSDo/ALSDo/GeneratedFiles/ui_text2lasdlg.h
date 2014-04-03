/********************************************************************************
** Form generated from reading UI file 'text2lasdlg.ui'
**
** Created: Mon Mar 31 08:22:53 2014
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEXT2LASDLG_H
#define UI_TEXT2LASDLG_H

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

class Ui_text2lasdlgClass
{
public:
    QPushButton *startBtn;
    QLabel *label1;
    QLabel *label2;
    QPushButton *lasOpenBtn;
    QPushButton *quitBtn;
    QPushButton *textOpenBtn;
    QTextEdit *lasPath;
    QTextEdit *textPath;

    void setupUi(QDialog *text2lasdlgClass)
    {
        if (text2lasdlgClass->objectName().isEmpty())
            text2lasdlgClass->setObjectName(QString::fromUtf8("text2lasdlgClass"));
        text2lasdlgClass->resize(591, 206);
        startBtn = new QPushButton(text2lasdlgClass);
        startBtn->setObjectName(QString::fromUtf8("startBtn"));
        startBtn->setGeometry(QRect(140, 152, 75, 31));
        label1 = new QLabel(text2lasdlgClass);
        label1->setObjectName(QString::fromUtf8("label1"));
        label1->setGeometry(QRect(30, 40, 111, 16));
        label2 = new QLabel(text2lasdlgClass);
        label2->setObjectName(QString::fromUtf8("label2"));
        label2->setGeometry(QRect(30, 98, 111, 16));
        lasOpenBtn = new QPushButton(text2lasdlgClass);
        lasOpenBtn->setObjectName(QString::fromUtf8("lasOpenBtn"));
        lasOpenBtn->setGeometry(QRect(480, 90, 75, 31));
        quitBtn = new QPushButton(text2lasdlgClass);
        quitBtn->setObjectName(QString::fromUtf8("quitBtn"));
        quitBtn->setGeometry(QRect(350, 150, 75, 31));
        textOpenBtn = new QPushButton(text2lasdlgClass);
        textOpenBtn->setObjectName(QString::fromUtf8("textOpenBtn"));
        textOpenBtn->setGeometry(QRect(480, 32, 75, 31));
        lasPath = new QTextEdit(text2lasdlgClass);
        lasPath->setObjectName(QString::fromUtf8("lasPath"));
        lasPath->setGeometry(QRect(160, 88, 291, 31));
        textPath = new QTextEdit(text2lasdlgClass);
        textPath->setObjectName(QString::fromUtf8("textPath"));
        textPath->setGeometry(QRect(160, 30, 291, 31));

        retranslateUi(text2lasdlgClass);

        QMetaObject::connectSlotsByName(text2lasdlgClass);
    } // setupUi

    void retranslateUi(QDialog *text2lasdlgClass)
    {
        text2lasdlgClass->setWindowTitle(QApplication::translate("text2lasdlgClass", "text2lasdlg", 0, QApplication::UnicodeUTF8));
        startBtn->setText(QApplication::translate("text2lasdlgClass", "start", 0, QApplication::UnicodeUTF8));
        label1->setText(QApplication::translate("text2lasdlgClass", "input text path", 0, QApplication::UnicodeUTF8));
        label2->setText(QApplication::translate("text2lasdlgClass", "output las path", 0, QApplication::UnicodeUTF8));
        lasOpenBtn->setText(QApplication::translate("text2lasdlgClass", "...", 0, QApplication::UnicodeUTF8));
        quitBtn->setText(QApplication::translate("text2lasdlgClass", "quit", 0, QApplication::UnicodeUTF8));
        textOpenBtn->setText(QApplication::translate("text2lasdlgClass", "...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class text2lasdlgClass: public Ui_text2lasdlgClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEXT2LASDLG_H
