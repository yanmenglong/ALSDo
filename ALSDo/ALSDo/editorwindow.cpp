#include <QtGui/QtGui>
#include "editorwindow.h"
#include "glrenderwidget.h"
#include "utils.h"

EditorWindow::EditorWindow(QWidget *parent, Qt::WindowFlags flags)
: QMainWindow(parent, flags)
{
	this->createUI();
	this->setWindowTitle("Editor Window");
	this->resize(QApplication::desktop()->width() - 250, QApplication::desktop()->height() - 350);
}

EditorWindow::~EditorWindow()
{
}

void EditorWindow::createUI()
{
	//action
	QAction* openAction = new QAction(QIcon(tr("./icons/open.png")), tr("&Open"),this);
	this->connect(openAction, SIGNAL(triggered()), this, SLOT(openScene()));
	QAction* exitAction = new QAction(QIcon(tr("./icons/quit.png")), tr("&Exit"), this);
    exitAction->setStatusTip(tr("exit"));
    this->connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

	//menu
	QMenu* fileMenu = this->menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(openAction);
	fileMenu->addAction(exitAction);

	//toolbar
	QToolBar *fileToolBar = this->addToolBar(tr("FileTool"));
	fileToolBar->addAction(openAction);

	this->_glRenderWidget = new GLRenderWidget(this);
	this->setCentralWidget(this->_glRenderWidget);
}

void EditorWindow::openScene()
{
	QString dataPath = GetOpenFilePath(this, tr("Open file ..."), tr("LAS File (*.las)"));
	if (!dataPath.isEmpty())
	{
		this->_glRenderWidget->loadData(dataPath);
	}
}
