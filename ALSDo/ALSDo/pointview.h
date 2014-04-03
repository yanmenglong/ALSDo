#ifndef POINTVIEW_H
#define POINTVIEW_H

#include <QKeyEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QSize>
#include <QWidget>
#include <QPainter>

#include <vector>
using namespace std;

#include "global.h"
#include "entity.h"

enum MouseTrack
{
	m_Identify = 1,
	m_Extract
};

class PointView : public QWidget
{
Q_OBJECT

public:
	PointView(QWidget *parent = 0);
	~PointView();
	
	// Return size of current client
	QSize sizeHint() const;
	// Set image data
	void setImageData(const int &dataW, const int &dataH, unsigned char* data);
	// Set the mouse track type
	void setMouseTrack(MouseTrack mouseTrack);
	// Show object boundings with given ids
	void showObjectBoundings(hash_map<int, Entity*> *entitys, vector<int> &IDs);
	// Add the points and draw the points
	void addPoints(vector<PT> &points, const bool &clearPrePts = true);
	// Add the rectangles and draw the rectangles
	void addRectangles(vector<QRect> &rects, const bool &clearPreRects = true);
	// Add the TIN points and draw the TINs
	void addTINPts(vector<PT> &points, const bool &clearPreTINPts = true);	
	// Save the current screen image
	QImage getCurrentScreen();
	// Dispose the current information
	void Dispose();

private slots:
	// Update the whole region
	void updateRegion(hash_map<int, Entity*> *entitys);

signals:
	// Emit this signal to show the mouse position, and the mainui class will process this signal
	void showMousePos(const int&mx, const int &my);
	// Emit this signal to identity an object, and the mainui class will process this signal
	void identifyObject(const int&mx, const int&my);
	// Emit this signal to cancel a building, and the mainui class will process this signal
	void cancelBuilding(const int &ID);

protected: // Virtual functions
	// Draw image and elements in this event
	virtual void paintEvent(QPaintEvent *event);
	// Mouse press event
	virtual void mousePressEvent(QMouseEvent *event);
	// Mouse move event
	virtual void mouseMoveEvent(QMouseEvent *event);
	// Key press event
	virtual void keyPressEvent(QKeyEvent *event);	

private:
	// Draw objects with given painter
	void drawObjects(QPainter &painter);

private:
	// The image to show
	QImage *_image;
	// The mouse operation tag
	MouseTrack _mouseTrack;	
	// Store the id of objects whose boundaries should be drawn
	vector<int> _boundIDs;

	// Structure to store the points
	vector<PT> _points;
	// Structure to store the rectangles
	vector<QRect> _rects;
	// Structure to store the TIN points
	vector<PT> _tinPts;
	// The current objects pointer getting from mainui 
	hash_map<int, Entity*> *_entitys;
};

#endif // POINTVIEW_H
