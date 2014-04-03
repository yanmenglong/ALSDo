#include "pointview.h"
#include "mainui.h"

#include <QMessageBox>
#include <vector>
using namespace std;

/**
*
*/
PointView::PointView(QWidget *parent)
	: QWidget(parent)
{
	this->_image = NULL;
	this->_entitys = NULL;
	this->_mouseTrack = MouseTrack::m_Identify;

	this->setMouseTracking(true);
}

/**
*
*/
PointView::~PointView()
{
	if(this->_image != NULL)
	{
		delete this->_image;
		this->_image = NULL;
	}
}

/**
* Return size of current client
*/
QSize PointView::sizeHint() const
{
	if(this->_image != NULL)
	{
		return this->_image->size();
	}
	else
	{
		return QSize(10, 10);
	}
}

/**
* Set image data
*/
void PointView::setImageData(const int &dataW, const int &dataH, unsigned char* data)
{
	if(this->_image != NULL)
	{
		delete this->_image;
		this->_image = NULL;
	}

	this->_image = new QImage(dataW, dataH, QImage::Format_RGB666);	
	int h = 0, w = 0, pos = 0;
	QRgb value;
	
	for(h = 0; h < dataH; ++h)
	{
		for(w = 0; w < dataW; ++w)
		{
			pos = h * dataW + w;
			value = qRgb(data[pos], data[pos], data[pos]);
			this->_image->setPixel(w, h, value);		
		}
	}
	this->resize(this->_image->size());
}

/**
* Set the mouse track type
*/
void PointView::setMouseTrack(MouseTrack mouseTrack)
{
	this->_mouseTrack = mouseTrack;
	if(this->_mouseTrack == MouseTrack::m_Extract)
	{
		this->grabKeyboard();
		this->_boundIDs.clear();
		this->update();
	}
	else if(this->_mouseTrack == MouseTrack::m_Identify)
	{
		this->releaseKeyboard();
	}	
}

/**
* Show object boundings with given ids
*/
void PointView::showObjectBoundings(hash_map<int, Entity*> *entitys, vector<int> &IDs)
{	
	Entity *entity = NULL;
	vector<int>::iterator idIter;

	// First clear the points, rectangles and TINs for better demonstration
	this->_points.clear();
	this->_rects.clear();
	this->_tinPts.clear();

	this->_entitys = entitys;
	if(this->_mouseTrack == MouseTrack::m_Identify)
	{		
		// If the current mouse track type is identifying objects, clear the previous bouding IDs
		this->_boundIDs.clear();
		for(idIter = IDs.begin(); idIter != IDs.end(); ++idIter)
		{
			this->_boundIDs.push_back(*idIter);
		}
	}
	else if(this->_mouseTrack == MouseTrack::m_Extract)
	{
		if(QMessageBox::Yes != QMessageBox::question(this, tr("query"), tr("accept as building ?"),
			QMessageBox::Yes, QMessageBox::No))
		{
			return;
		}
		
		// In fact, only one id each time
		for(idIter = IDs.begin(); idIter != IDs.end(); ++idIter)
		{
			this->_boundIDs.push_back(*idIter);
			entity = (*this->_entitys)[*idIter];
			entity->setType(PointType::p_building);
		}	
	} 
	
	// Update the view
	this->update();	
}

/**
* Add the points and draw the points
*/
void PointView::addPoints(vector<PT> &points, const bool &clearPrePts)
{
	vector<PT>::iterator ptIter;

	if(clearPrePts)
	{
		this->_points.clear();
	}
	for(ptIter = points.begin(); ptIter != points.end();  ++ptIter)
	{
		this->_points.push_back(*ptIter);
	}
	this->update();
}

/**
* Add the rectangles and draw the rectangles
*/
void PointView::addRectangles(vector<QRect> &rects, const bool &clearPreRects)
{
	vector<QRect>::iterator rectIter;
	
	if(clearPreRects)
	{
		this->_rects.clear();
	}
	for(rectIter = rects.begin(); rectIter != rects.end(); ++rectIter)
	{
		this->_rects.push_back(*rectIter);
	}
	this->update();
}

/**
* Add the TIN points and draw the TINs
*/
void PointView::addTINPts(vector<PT> &points, const bool &clearPreTINPts)
{
	vector<PT>::iterator ptIter;

	if(clearPreTINPts)
	{
		this->_tinPts.clear();
	}
	for(ptIter = points.begin(); ptIter != points.end();  ++ptIter)
	{
		this->_tinPts.push_back(*ptIter);
	}
	this->update();	
}

/**
* Save the current screen image
*/
QImage PointView::getCurrentScreen()
{
	QImage image = this->_image->copy(this->_image->rect());
	QPainter painter(&image);
	this->drawObjects(painter);
	return image;
}

/**
* Dispose the current information
*/
void PointView::Dispose()
{
	// Dispose the image 
	if(this->_image != NULL)
	{
		delete this->_image;
		this->_image = NULL;
	}

	// Dispose these drawing objects
	this->_boundIDs.clear();
	this->_points.clear();
	this->_rects.clear();
	this->_tinPts.clear();
	this->_entitys = NULL;
	
	// Rest mouse operation tag
	this->_mouseTrack = MouseTrack::m_Identify;	
}

/**
* Update the whole region
*/
void PointView::updateRegion(hash_map<int, Entity*> *entitys)
{
	this->_entitys = entitys;
	this->update();
}

/**
* Draw image and elements in this event
*/
void PointView::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QPoint point(0, 0);
	if(this->_image == NULL)
	{
		return;		
	}
	painter.drawImage(point, this->_image[0]);
	this->drawObjects(painter);
}

/**
* Mouse press event
*/
void PointView::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) 
	{
		int x = event->x();
		int y = event->y();
		if(x < 0 || x >= this->_image->width() || y < 0 || y >= this->_image->height()) {
			return;
		}

		emit this->identifyObject(x, y);
	}
}

/**
* Mouse move event
*/
void PointView::mouseMoveEvent(QMouseEvent *event)
{
	 int mx = event->x(), my = event->y();
	 emit this->showMousePos(mx, my);
}

/**
* Key press event
*/
void PointView::keyPressEvent(QKeyEvent *event)
{
	int id = 0;

	if(event->key() == Qt::Key::Key_C && this->_mouseTrack == MouseTrack::m_Extract && this->_boundIDs.size() > 0)
	{
		if(QMessageBox::Yes != QMessageBox::question(this, tr("query"), tr("remove the last building ?"),
			QMessageBox::Yes, QMessageBox::No))
		{
			return;
		}

		id = this->_boundIDs.back();
		this->_boundIDs.pop_back();
		this->update();

		// Emit this signal to mainui to set the type of related object
		emit this->cancelBuilding(id);
	}
}

/**
* Draw objects with given painter
*/
void PointView::drawObjects(QPainter &painter)
{
	int offset = 3, ii = 0, count = 0, id = 0;
	PT pt1, pt2, pt3;
	vector<int>::iterator idIter;
	vector<PT> bd, pts;
	vector<PT>::iterator ptIter;
	hash_map<int, Entity*>::iterator entityIter;
	Entity *entity = NULL;

	// Draw objects with different colors according to their types
	if(this->_entitys != NULL)
	{
		for(entityIter = this->_entitys->begin(); entityIter != this->_entitys->end(); ++entityIter)
		{
			entity = entityIter->second;
			if(entity->getType() == PointType::p_created)
			{
				painter.setPen(Qt::gray);
			}
			else if(entity->getType() == PointType::p_ground)
			{
				painter.setPen(Qt::blue);
			}
			else if(entity->getType() == PointType::p_building)
			{
				painter.setPen(Qt::red);
			}
			else if(entity->getType() == PointType::p_high_vegetation)
			{
				painter.setPen(Qt::green);
			}
			else 
			{
				// Other type, just ignore it
				continue;
			}

			pts = entity->getAllPts();
			for(ptIter = pts.begin(); ptIter != pts.end(); ++ptIter)
			{
				painter.drawPoint(ptIter->x, ptIter->y);
			}
			/*
			if(entity->getType() == PointType::p_building)
			{
				painter.setPen(Qt::black);
				painter.setFont(QFont("Times", 12, 12, false));
				pts = entity->getBounding();
				for(ptIter = pts.begin(); ptIter != pts.end(); ++ptIter)
				{
					painter.drawPoint(ptIter->x, ptIter->y);
				}
			}
			*/
		}
	}

	painter.setPen(Qt::red);
	painter.setFont(QFont("Times", 9, 9, false));
	// Draw points	
	for(vector<PT>::iterator it = this->_points.begin(); it != this->_points.end(); ++it)
	{
		painter.drawLine(it->x-offset, it->y, it->x+offset, it->y);
		painter.drawLine(it->x, it->y-offset, it->x, it->y+offset);
	}

	// Draw rectangles
	for(vector<QRect>::iterator it = this->_rects.begin(); it != this->_rects.end(); ++it)
	{
		painter.drawRect(*it);
	}

	// Draw triangles
	ii = 0;
	count = this->_tinPts.size();
	while(ii < count)
	{
		pt1 = this->_tinPts[ii];
		pt2 = this->_tinPts[ii + 1];
		pt3 = this->_tinPts[ii + 2];
		painter.drawLine(pt1.x, pt1.y, pt2.x, pt2.y);
		painter.drawLine(pt2.x, pt2.y, pt3.x, pt3.y);
		painter.drawLine(pt3.x, pt3.y, pt1.x, pt1.y);

		ii = ii + 3;
	}

	// Draw boundings	
	for(vector<int>::iterator it = this->_boundIDs.begin(); it != this->_boundIDs.end(); ++it)
	{
		id = *it;
		Entity *entity = (*this->_entitys)[id];
		if(entity == NULL)
		{
			continue;
		}

		bd = entity->getBounding();
		for(vector<PT>::iterator it1 = bd.begin(); it1 != bd.end(); ++it1)
		{
			painter.drawPoint(it1->x, it1->y);
		}
	}
}