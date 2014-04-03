#ifndef MAINWINDOWFRM_H_
#define MAINWINDOWFRM_H_

#include <QtGui/QMainWindow>

class QAction;
class QMenu;

class GLRenderWidget;

class EditorWindow : public QMainWindow
{
	Q_OBJECT
public:
	EditorWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~EditorWindow();

private slots:
	void openScene();

private:
	void createUI();

	GLRenderWidget* _glRenderWidget;
};
#endif