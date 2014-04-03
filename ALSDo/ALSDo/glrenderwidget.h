#ifndef ADAPTERWIDGET_H_
#define ADAPTERWIDGET_H_

#include <osgViewer/Viewer>
#include <QtCore/QTimer>
#include <QtCore/QString>
#include <QtOpenGL/QGLWidget>

class GLRenderWidget : public osgViewer::Viewer, public QGLWidget
{
public:
	GLRenderWidget(QWidget *parent = 0);
	virtual ~GLRenderWidget();
	void loadData(const QString lasPath);

	osgViewer::GraphicsWindow* getGraphicsWindow();

protected:
	virtual void init();
	virtual void resizeGL(int width, int height);
	virtual void paintGL();
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);

	osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> _gw;
	QTimer _timer;
};
#endif