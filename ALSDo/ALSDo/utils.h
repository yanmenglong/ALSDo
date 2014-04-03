#ifndef TOOLS_H
#define TOOLS_H

#include <QWidget>

#include "lasio.h"

#ifdef __cplusplus 
extern "C" { 
#endif


QString GetOpenFilePath(QWidget *parent, QString title, QString filter);
QString GetSaveFilePath(QWidget *parent, QString title, QString filter);

void calcPtDensity(LASHeader *lasHeader, double &cellSize, double &ptDensity);

#ifdef __cplusplus 
} 
#endif

#endif