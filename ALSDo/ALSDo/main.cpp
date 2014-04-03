#include "mainui.h"
#include <QtGui/QApplication>
#include <QtGui/QStyleFactory>
#include <QtCore/QTextCodec>
#include <QtGui/QFont>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QIODevice>
#include <QTextStream>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	//QTextCodec::setCodecForCStrings(QTextCodec::codecForName("gbk"));
	//QApplication::setFont(QFont("Times", 11, QFont::Light));	
	//QApplication::setStyle(QStyleFactory::create("cleanlooks"));

	MainUI w;
	w.showMaximized();
	return a.exec();
}
