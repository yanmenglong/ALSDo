#include <QtGui/QKeyEvent>
#include <QMessageBox>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>

#include "glrenderwidget.h"
#include "lasio.h"

GLRenderWidget::GLRenderWidget(QWidget * parent)
	:QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
	this->setFocusPolicy(Qt::ClickFocus);
	this->_gw = new osgViewer::GraphicsWindowEmbedded(0, 0, width(), height());	
	this->init();
}

GLRenderWidget::~GLRenderWidget()
{
}

void GLRenderWidget::loadData(const QString lasPath)
{
	QByteArray ba = lasPath.toLatin1();
	const char *path = ba.data();
	LasIO *lasIO = new LasIO(string(path));
	LASHeader *header = lasIO->open();
	LASPoint* laspts = NULL, *laspt = NULL;
	int ii = 0;

	if(header == NULL) 
	{
		QMessageBox::critical(this, tr("error"), tr("open las error!"), QMessageBox::Ok);
		return; 
	}	
	laspts = lasIO->readAllPts();

	// 创建几何对象 
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry; 
	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
	for(ii = 0; ii < header->records_number; ++ii)
	{
		laspt = &laspts[ii];
		v->push_back(osg::Vec3(laspt->x, laspt->y, laspt->z)); 
	}
	geom->setVertexArray(v.get());	 
	
	// 创建颜色数组 
	osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array;
	c->push_back(osg::Vec4(1.f, 1.f, 1.f, 1.f)); 
	geom->setColorArray(c.get()); 
	geom->setColorBinding(osg::Geometry::BIND_OVERALL); 

	// 法线
	osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array; 
	n->push_back(osg::Vec3(0.f, 0.f, 1.f ));
	geom->setNormalArray(n.get()); 
	geom->setNormalBinding(osg::Geometry::BIND_OVERALL); 

	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, v->size())); 

	// 向 Geode 类添加几何体（Drawable） 
	osg::ref_ptr<osg::Geode> geode = new osg::Geode; 
	geode->addDrawable(geom.get()); 

	this->setCameraManipulator(new osgGA::TrackballManipulator);
    this->setSceneData(geode.get());

	if(lasIO != NULL)
	{
		delete lasIO;
		lasIO = NULL;
	}
	if(laspts != NULL)
	{
		delete []laspts;
		laspts = NULL;
	}	
}
	
osgViewer::GraphicsWindow* GLRenderWidget::getGraphicsWindow()
{
	return this->_gw.get();
}

void GLRenderWidget::init()
{
	this->getCamera()->setViewport(new osg::Viewport(0, 0, width(), height()));
	this->getCamera()->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(width())/static_cast<double>(height()), 1.0f, 10000.0f);
	this->getCamera()->setGraphicsContext(this->getGraphicsWindow());
	this->setThreadingModel(osgViewer::Viewer::SingleThreaded);

	this->connect(&this->_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
	this->_timer.start(10);
}

void GLRenderWidget::resizeGL(int width, int height )
{
	this->_gw->getEventQueue()->windowResize(0, 0, width, height);
	this->_gw->resized(0, 0, width, height);
}

void GLRenderWidget::paintGL()
{		
	this->frame();
}

void GLRenderWidget::keyPressEvent(QKeyEvent* event )
{
	this->_gw->getEventQueue()->keyPress((osgGA::GUIEventAdapter::KeySymbol) *(event->text().toAscii().data()));
}

void GLRenderWidget::keyReleaseEvent(QKeyEvent* event )
{
	this->_gw->getEventQueue()->keyRelease((osgGA::GUIEventAdapter::KeySymbol) *(event->text().toAscii().data()));
}

void GLRenderWidget::mousePressEvent(QMouseEvent* event )
{
	int button = 0;
	switch(event->button())
	{
	case(Qt::LeftButton): 
		button = 1; 
		break;
	case(Qt::MidButton): 
		button = 2; 
		break;
	case(Qt::RightButton): 
		button = 3; 
		break;
	case(Qt::NoButton): 
		button = 0; 
		break;
	default: 
		button = 0; 
		break;
	}
	this->_gw->getEventQueue()->mouseButtonPress(event->x(), event->y(), button);
}

void GLRenderWidget::mouseReleaseEvent(QMouseEvent* event )
{
	int button = 0;
	switch(event->button())
	{
	case(Qt::LeftButton): 
		button = 1;
		break;
	case(Qt::MidButton): 
		button = 2; 
		break;
	case(Qt::RightButton): 
		button = 3; 
		break;
	case(Qt::NoButton): 
		button = 0; 
		break;
	default:
		button = 0; 
		break;
	}
	this->_gw->getEventQueue()->mouseButtonRelease(event->x(), event->y(), button);
}

void GLRenderWidget::mouseMoveEvent(QMouseEvent* event )
{
	this->_gw->getEventQueue()->mouseMotion(event->x(), event->y());
}
