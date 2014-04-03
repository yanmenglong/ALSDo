#include "mainui.h"
#include "newprojectdlg.h"
#include "pointinfodlg.h"
#include "utils.h"
#include "pointview.h"
#include "workspace.h"
#include "las2textdlg.h"
#include "text2lasdlg.h"
#include "entityparamthresholddlg.h"
#include "obafilter.h"
#include "gridfilter.h"
#include "obaclassifier.h"
#include "util.h"
#include "autorundlg.h"
#include "editorwindow.h"
#include "treeinfowidget.h"
#include "vegetationextractor.h"
#include "obafilterparamdlg.h"

#include <QAction>
#include <QDir>
#include <QDockWidget>
#include <QFile>
#include <QFileDialog>
#include <QListWidget>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QProcess>
#include <QScrollArea>
#include <QStatusBar>
#include <QToolBar>
#include <QApplication>
#include <QDesktopWidget>

#include <cstdio>
#include <string>
using namespace std;

extern QString token1; 
extern QString token2;

MainUI::MainUI(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	this->_workSpace = NULL;
	this->_editorWindow = NULL;
	this->_step = ProcessStep::none;
	this->createUI();
}

MainUI::~MainUI()
{
	if(this->_workSpace != NULL)
	{
		delete this->_workSpace;
		this->_workSpace = NULL;
	}
}

void MainUI::createUI()
{
	// Point view
	QScrollArea *scrollArea = new QScrollArea(this);
	scrollArea->setBackgroundRole(QPalette::Dark);
	this->_ptView = new PointView(this);
	scrollArea->setWidget(this->_ptView);
    this->setCentralWidget(scrollArea);
	this->connect(this->_ptView, SIGNAL(showMousePos(const int&, const int &)), this, SLOT(showMousePos(const int&, const int &)));
	this->connect(this->_ptView, SIGNAL(identifyObject(const int&, const int &)), this, SLOT(identifyObject(const int&, const int &)));
	this->connect(this->_ptView, SIGNAL(cancelBuilding(const int&)), this, SLOT(cancelBuilding(const int&)));
	this->connect(this, SIGNAL(updatePointView(hash_map<int, Entity*>*)), this->_ptView, SLOT(updateRegion(hash_map<int, Entity*>*)));

	// Dockable items window
	this->_treeInfoWidget = new TreeInfoWidget(this);
	this->connect(this, SIGNAL(updateNodeInfo(QString&, QString&)), this->_treeInfoWidget, SLOT(updateNodeInfo(QString&, QString&)));
	this->connect(this->_treeInfoWidget, SIGNAL(useEntityParam(const QString&, const int&)), this, SLOT(useEntityParam(const QString&, const int&)));
	this->connect(this->_treeInfoWidget, SIGNAL(resetEntitysType()), this, SLOT(resetEntitysType()));		
	this->_dockWidget = new QDockWidget(tr("information view"), this);
	this->_dockWidget->setWidget(this->_treeInfoWidget);
	this->_dockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
	this->addDockWidget(Qt::RightDockWidgetArea, this->_dockWidget);
	this->_dockWidget->close();

	// File action
	QAction *newAction = new QAction(QIcon(tr("./icons/new.png")), tr("&New Project"), this);
	newAction->setShortcuts(QKeySequence::New);
    newAction->setStatusTip(tr("Create a new Project"));
	this->connect(newAction, SIGNAL(triggered()), this, SLOT(newProject()));
	QAction *quitAction = new QAction(QIcon(tr("./icons/quit.png")), tr("&Quit"), this);
    quitAction->setShortcuts(QKeySequence::Quit);
    quitAction->setStatusTip(tr("Quit the application"));
    this->connect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));

	// View action
	QAction *showAllObjsAction = new QAction(QIcon(tr("./icons/boundarys.png")), tr("&Show All Objects"), this);
	showAllObjsAction->setStatusTip(tr("Show all objects"));
	this->connect(showAllObjsAction, SIGNAL(triggered()), this, SLOT(showAllObjects()));
	QAction *saveScreenAction = new QAction(QIcon(tr("./icons/image2disk.png")), tr("&save Current Image"), this);
	saveScreenAction->setStatusTip(tr("save current image"));
	this->connect(saveScreenAction, SIGNAL(triggered()), this, SLOT(saveCurrentScreen()));
	QAction *itemViewAction = new QAction(QIcon(tr("./icons/itemview.png")), tr("&Item View"), this);
    itemViewAction->setStatusTip(tr("Item View"));
    this->connect(itemViewAction, SIGNAL(triggered()), this, SLOT(itemView()));
	QAction *editorViewAction = new QAction(QIcon(tr("./icons/terrain.png")), tr("&editor View"), this);
    editorViewAction->setStatusTip(tr("editor View"));
    this->connect(editorViewAction, SIGNAL(triggered()), this, SLOT(editorView()));

	// Classify action
	QAction *obaFilterAction = new QAction(QIcon(tr("./icons/filter.png")), tr("&Filter Ground"), this);
	obaFilterAction->setStatusTip(tr("Filter by OBA method ..."));
    this->connect(obaFilterAction, SIGNAL(triggered()), this, SLOT(filterPtsByOBAFilter()));	
	QAction *classifyObjsAction = new QAction(QIcon(tr("./icons/classifier.png")), tr("&Classify Objects"), this);
	classifyObjsAction->setStatusTip(tr("Classify nonground points ..."));
	this->connect(classifyObjsAction, SIGNAL(triggered()), this, SLOT(classifyObjs()));		
	QAction *resetAction = new QAction(QIcon(tr("./icons/reset.png")), tr("&Reset Entitys"), this);
    resetAction->setStatusTip(tr("Reset type of entitys"));
	this->connect(resetAction, SIGNAL(triggered()), this, SLOT(resetEntitysType()));	
	QAction *extractBuildingManualAction = new QAction(QIcon(tr("./icons/building-manual.png")), tr("&Extract Building Manual"), this);
	extractBuildingManualAction->setStatusTip(tr("Extract building manual ..."));
	this->connect(extractBuildingManualAction, SIGNAL(triggered()), this, SLOT(extractBuildingManual()));
	QAction *exportBuildingAction = new QAction(QIcon(tr("./icons/building.png")), tr("&Export Building"), this);
	exportBuildingAction->setStatusTip(tr("Export building to disk ..."));
	this->connect(exportBuildingAction, SIGNAL(triggered()), this, SLOT(exportBuilding()));
	QAction *autoRunAction = new QAction(QIcon(tr("./icons/autorun.png")), tr("&Auto Run"), this);
	autoRunAction->setStatusTip(tr("start a autorun filtering project ..."));
	this->connect(autoRunAction, SIGNAL(triggered()), this, SLOT(autoRun()));

	/**
	* LAS tool action
	**/
	QAction *datasetInfoAction = new QAction(QIcon(tr("./icons/info.png")), tr("&Dataset Info"), this);
	datasetInfoAction->setStatusTip(tr("Show the information of dataset ..."));
	this->connect(datasetInfoAction, SIGNAL(triggered()), this, SLOT(datasetInfo()));
	// Import DTM
	QAction *importDTMAction = new QAction(QIcon(tr("./icons/dtm.png")), tr("&Import DTM"), this);
	importDTMAction->setStatusTip(tr("Import DTM ..."));  
	this->connect(importDTMAction, SIGNAL(triggered()), this, SLOT(importDTM()));
	// Import point action
	QAction *importPointAction = new QAction(QIcon(tr("./icons/point.png")), tr("&Import Point"), this);
	importPointAction->setStatusTip(tr("Import point ..."));
	this->connect(importPointAction, SIGNAL(triggered()), this, SLOT(importPoint()));
	// Import rectangle
	QAction *importRectangleAction = new QAction(QIcon(tr("./icons/rectangle.png")), tr("&Import Rectangle"), this);
	importRectangleAction->setStatusTip(tr("Import rectangle ..."));
	this->connect(importRectangleAction, SIGNAL(triggered()), this, SLOT(importRectangle()));
	// Import delaunay
	QAction *importTINAction = new QAction(QIcon(tr("./icons/delaunay.png")), tr("&Import TIN"), this);
	importTINAction->setStatusTip(tr("Import TIN ..."));
	this->connect(importTINAction, SIGNAL(triggered()), this, SLOT(importTIN()));	
	QAction *text2lasAction = new QAction(QIcon(tr("./icons/text2las.png")), tr("&Text to LAS"), this);
	text2lasAction->setStatusTip(tr("Text to LAS ..."));
	this->connect(text2lasAction, SIGNAL(triggered()), this, SLOT(text2las()));
	QAction *las2textAction = new QAction(QIcon(tr("./icons/las2text.png")), tr("&LAS to Text"), this);
	las2textAction->setStatusTip(tr("LAS to text ..."));
	this->connect(las2textAction, SIGNAL(triggered()), this, SLOT(las2text()));

	// Help action
    QAction *aboutAction = new QAction(QIcon(tr("./icons/about.png")), tr("&About"), this);
    aboutAction->setStatusTip(tr("Show the application's About box"));
    this->connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
	
	// File menu
	QMenu *fileMenu = this->menuBar()->addMenu(tr("&File"));
	fileMenu->setStatusTip(tr("File"));
	fileMenu->addAction(newAction);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAction);
	
	// View menu
	QMenu *viewMenu = this->menuBar()->addMenu(tr("&View"));
	viewMenu->addAction(showAllObjsAction);
	viewMenu->addAction(saveScreenAction);
	viewMenu->addSeparator();
	viewMenu->addAction(itemViewAction);
	viewMenu->addAction(editorViewAction);

	// Classify menu
	QMenu *classifyMenu = this->menuBar()->addMenu(tr("&Classify"));
	classifyMenu->addAction(obaFilterAction);
	classifyMenu->addAction(classifyObjsAction);
	classifyMenu->addAction(resetAction);	
	classifyMenu->addAction(extractBuildingManualAction);
	classifyMenu->addAction(exportBuildingAction);
	classifyMenu->addSeparator();
	classifyMenu->addAction(autoRunAction);	

	// Tool menu
	QMenu *toolMenu = this->menuBar()->addMenu(tr("&Tool"));
	toolMenu->addAction(datasetInfoAction);
	toolMenu->addAction(importDTMAction);
	toolMenu->addAction(importPointAction);
	toolMenu->addAction(importRectangleAction);
	toolMenu->addAction(importTINAction);
	toolMenu->addSeparator();
	toolMenu->addAction(text2lasAction);
	toolMenu->addAction(las2textAction);
	
	// Help menu
	QMenu *helpMenu = this->menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
	
	// File toolbar
	QToolBar *fileToolBar = this->addToolBar(tr("File Toolbar"));
	fileToolBar->addAction(newAction);

	// View toolbar
	QToolBar *viewToolBar = this->addToolBar(tr("View Toolbar"));
	viewToolBar->addAction(showAllObjsAction);
	viewToolBar->addAction(saveScreenAction);
	viewToolBar->addAction(itemViewAction);
	viewToolBar->addAction(editorViewAction);	

	// Classify toolbar
	QToolBar *classifyToolBar = this->addToolBar(tr("Classification Toolbar"));
	classifyToolBar->addAction(obaFilterAction);
	classifyToolBar->addAction(classifyObjsAction);	
	classifyToolBar->addAction(resetAction);	
	classifyToolBar->addAction(extractBuildingManualAction);
	classifyToolBar->addAction(exportBuildingAction);
	classifyToolBar->addAction(autoRunAction);	

	// LAS toolbar
	QToolBar *utilToolBar = this->addToolBar(tr("Util Toolbar"));
	utilToolBar->addAction(datasetInfoAction);
	utilToolBar->addAction(importDTMAction);
	utilToolBar->addAction(importPointAction);
	utilToolBar->addAction(importRectangleAction);
	utilToolBar->addAction(importTINAction);
	utilToolBar->addAction(text2lasAction);
	utilToolBar->addAction(las2textAction);	

	// Help toolbar
	QToolBar *helpBar = this->addToolBar(tr("Help Toolbar"));
	helpBar->addAction(aboutAction);

	// Status bar
	this->statusBar()->showMessage(tr("Ready"));
}

/**
* Create a new project
*/
void MainUI::newProject()
{
	string lasPath;
	string resultDir;
	QDir dir;
	int msg = 0;

	NewProjectDlg dlg(this);
	if(dlg.exec() != QDialog::Accepted)
	{
		return;
	}
	lasPath = dlg.getLasPath().toStdString();
	resultDir = dlg.getResultDir().toStdString();
	if(lasPath.length() == 0) 
	{
		QMessageBox::critical(this, tr("error"), tr("las path is null!"), QMessageBox::Ok);
		return;
	}
	if(resultDir.length() == 0) 
	{
		QMessageBox::critical(this, tr("error"), tr("project path is null!"), QMessageBox::Ok);
		return;
	}	
	if(!dir.exists(dlg.getResultDir()))
	{
		dir.mkdir(dlg.getResultDir());
	}
	if(resultDir.at(resultDir.length()-1) != '/')
	{
		resultDir = resultDir + "/";
	}

	if(this->_workSpace != NULL) 
	{
		delete this->_workSpace;
		this->_workSpace = NULL;

		this->_ptView->Dispose();
	}

	this->_workSpace = new WorkSpace();
	this->_workSpace->newProj(lasPath.c_str(), dlg.cellSize(), resultDir.c_str());
	GridIndex *index = this->_workSpace->getGridIndex();
	this->_ptView->setImageData(index->getGridWidth(), index->getGridHeight(), this->_workSpace->resampleZ());
	this->_step = ProcessStep::none;
}

/**
* Quit the system
*/
void MainUI::quit()
{
	this->close();
}

/**
* Show all boundaries of segment objects
*/
void MainUI::showAllObjects()
{
	OBAFilter *obaFilter = NULL;
	OBAClassifier *obaClassifier = NULL;
	VegetationExtractor* vegetationExtractor = NULL;
	hash_map<int, Entity*> *entitys = NULL;
	hash_map<int, Entity*>::iterator entityIter;
	vector<int> IDs;

	if(this->_workSpace == NULL)
	{
		QMessageBox::critical(this, tr("error"), tr("create or open project first!"), QMessageBox::Ok);
		return;
	}
	if(this->_step == ProcessStep::obafilter_filtering)
	{
		obaFilter = this->_workSpace->getOBAFilter();
		entitys = obaFilter->getEntitys();
	}
	if(this->_step == ProcessStep::obaclassifier_classifying)
	{
		obaClassifier = this->_workSpace->getOBAClassifier();
		entitys = obaClassifier->getEntitys();
	}
	if(this->_step == ProcessStep::tree_extraction)
	{
		vegetationExtractor = this->_workSpace->getVegetationExtractor();
		entitys = vegetationExtractor->getEntitys();
	}

	if(entitys == NULL)
	{
		return;
	}
	for(entityIter = entitys->begin(); entityIter != entitys->end(); ++entityIter)
	{
		IDs.push_back(entityIter->first);
	}
	this->_ptView->showObjectBoundings(entitys, IDs);
}

/**
* Save the current screen as image
*/
void MainUI::saveCurrentScreen()
{
	if(this->_workSpace == NULL)
	{
		QMessageBox::critical(this, tr("error"), tr("create or open project first!"), QMessageBox::Ok);
		return;
	}

	QString fileName = GetSaveFilePath(this, tr("png file path"), tr("png file (*.png)"));
    if (!fileName.isNull()) 
	{
		QImage image = this->_ptView->getCurrentScreen();
		image.save(fileName, "PNG");
    }	
}

/**
* Show or hide the item view
*/
void MainUI::itemView()
{
	if(this->_dockWidget->isVisible())
	{
		this->_dockWidget->hide();
	}
	else
	{
		this->_dockWidget->show();
	}
}

/**
* Show or hide the editor view
*/
void MainUI::editorView()
{
	if(this->_editorWindow != NULL)
	{
		delete this->_editorWindow;
		this->_editorWindow = NULL;
	}
	if(QMessageBox::Yes != QMessageBox::question(this, tr("query"), tr("show terrain view ?"), 
		QMessageBox::Yes, QMessageBox::No))
	{
		return;
	}

	this->_editorWindow = new EditorWindow(this);
	this->_editorWindow->showMaximized();
}

/**
* Filter the ground by OBA method
*/
void MainUI::filterPtsByOBAFilter()
{
	
	OBAFilterParamDlg dlg(this);
	OBAFilter *obaFilter = NULL;
	double seedHeightTh = 0.0, seedSlopeTh = 0.0, maxObjSize = 0.0, seedCellSize = 0.0, segHeightTh = 0.0, 
		segSlopeTh = 0.0, filteringHeightTh = 0.0, filteringSlopeTh = 0.0, dtmFactor = 0.0, objectHT = 0.0;
	QString content;

	if(this->_workSpace == NULL) 
	{
		QMessageBox::critical(this, tr("error"), tr("create or open project first!"), QMessageBox::Ok);
		return;
	}
	if(this->_step == ProcessStep::obafilter_filtering)
	{
		if(QMessageBox::Yes != QMessageBox::question(this, tr("query"), tr("another filter ?"), 
			QMessageBox::Yes, QMessageBox::No))
		{
			return;
		}
	}

	if(dlg.exec() != QDialog::Accepted)
	{
		return;
	}

	
	seedHeightTh = dlg.getSeedHeightTh();
	seedSlopeTh = dlg.getSeedSlopeTh()*PI/180.0;
	maxObjSize = dlg.getMaxObjSize();
	seedCellSize = dlg.getSeedCellSize();
	segHeightTh = dlg.getSegHeightTh();
	segSlopeTh = dlg.getSegSlopeTh()*PI/180.0;
	filteringHeightTh = dlg.getFilteringHeightTh();
	filteringSlopeTh = dlg.getFilteringSlopeTh()*PI/180.0;
	dtmFactor = dlg.getDTMFactor();
	objectHT = dlg.getObjectHT();

	obaFilter = this->_workSpace->getOBAFilter();
	obaFilter->segmentRegionBySeeds(seedHeightTh, seedSlopeTh, maxObjSize, seedCellSize, segHeightTh, segSlopeTh);
	//obaFilter->segmentRegion(segHeightTh, segSlopeTh);
	obaFilter->findBounding();	
	obaFilter->findTop();
	obaFilter->removeHole(5);
	//obaFilter->removeEntity(20);
	obaFilter->findBounding();
	obaFilter->statisticEntityInfo(maxObjSize*maxObjSize); // Statistic entity first
	obaFilter->filterRegion(filteringHeightTh, filteringSlopeTh, dtmFactor, objectHT);

	emit updatePointView(obaFilter->getEntitys());
	content.setNum(obaFilter->getObjNum());
	QMessageBox::information(this, tr("information"), tr("Filtering finished! The object number is:") + content, QMessageBox::Ok);
	this->_step = ProcessStep::obafilter_filtering;

	/*
	GridFilter gridFilter(this->_workSpace);
	gridFilter.seedExpand(60.0, 20.0, segHeightTh, segSlopeTh);
	gridFilter.filterPoints(filteringHeightTh, filteringSlopeTh, dtmFactor, objectHT);
	*/
}

/**
* Classify the non-ground points by OBA method
*/
void MainUI::classifyObjs()
{
	OBAClassifier *obaClassifier = NULL;
	double maxObjSize = 0.0, segHeightTh = 0.0, segSlopeTh = 0.0;
	QString content;

	if(this->_workSpace == NULL) 
	{
		QMessageBox::critical(this, tr("error"), tr("create or open project first!"), QMessageBox::Ok);
		return;
	}
	if(this->_step == ProcessStep::obaclassifier_classifying)
	{
		if(QMessageBox::Yes != QMessageBox::question(this, tr("query"), tr("another segmentation for classification ?"), 
			QMessageBox::Yes, QMessageBox::No))
		{
			return;
		}
	}

	maxObjSize = 40;
	segHeightTh = 1.5;
	segSlopeTh = 60*PI/180.0;
	
	obaClassifier = this->_workSpace->getOBAClassifier();
	obaClassifier->segmentRegionBySeeds(maxObjSize, segHeightTh, segSlopeTh);
	//obaFilter->segmentRegion(segHeightTh, segSlopeTh);
	obaClassifier->findBounding();	
	obaClassifier->findTop();
	obaClassifier->removeHole(5);
	obaClassifier->removeEntity(1);
	obaClassifier->findBounding();
	obaClassifier->statisticEntityInfo(NAN); // Statistic entity first
	// Classify entitys by default parameters
	obaClassifier->classifyEntitys();

	emit updatePointView(obaClassifier->getEntitys());
	content.setNum(obaClassifier->getObjNum());
	QMessageBox::information(this, tr("information"), tr("Filtering finished! The object number is:") + content, QMessageBox::Ok);
	this->_step = ProcessStep::obaclassifier_classifying;
}

/**
* Extract building manually
*/
void MainUI::extractBuildingManual()
{
	if(this->_workSpace == NULL)
	{
		QMessageBox::critical(this, tr("error"), tr("create or open project first!"), QMessageBox::Ok);
		return;
	}
	if(this->_step != ProcessStep::obaclassifier_classifying)
	{
		QMessageBox::critical(this, tr("error"), tr("segment first!"), QMessageBox::Ok);
		return;
	}

	this->_ptView->setMouseTrack(MouseTrack::m_Extract);
}

/**
* Export building to disk
*/
void MainUI::exportBuilding()
{
	if(this->_workSpace == NULL)
	{
		QMessageBox::critical(this, tr("error"), tr("create or open project first!"), QMessageBox::Ok);
		return;
	}
	if(this->_step != ProcessStep::obaclassifier_classifying)
	{
		QMessageBox::critical(this, tr("error"), tr("classify objects first!"), QMessageBox::Ok);
		return;
	}

	// Reset the view mouse track type
	this->_ptView->setMouseTrack(MouseTrack::m_Identify);
	
	// Export the building LAS points
	exportBuildingLASPoints(this->_workSpace);
	// Export the building polygons
	exportBuildingPolygons(this->_workSpace);
	// Export the building raster
	exportBuildingRaster(this->_workSpace);
}

/**
* Extract tree automatically
*/
/*
void MainUI::treeExtract()
{
	if(this->_workSpace == NULL)
	{
		QMessageBox::critical(this, tr("error"), tr("create or open project first!"), QMessageBox::Ok);
		return;
	}

	double region_size = 7, hIter = 1.0, aIter = PI * 5.0/ 180.0;
	GridIndex *index= this->_workSpace->getGridIndex();
	int H = index->getGridHeight(), W = index->getGridWidth(), h = 0, w = 0 , ii = 0, size = 0;
	
	size = H*W;
	
	QString fileName = GetOpenFilePath(this, tr("type id file"), tr("id file (*.id)"));
    if (fileName.isNull()) 
	{
		QMessageBox::critical(this, tr("error"), tr("invalid type if file name!"), QMessageBox::Ok);
		return;
	}

	VegetationExtractor* vegetationExtractor = this->_workSpace->getVegetationExtractor();
	vegetationExtractor->segmentRegionBySeeds(region_size, hIter, aIter, fileName.toStdString().c_str());
	vegetationExtractor->findBounding();
	vegetationExtractor->findTop();
	this->_step = ProcessStep::tree_extraction;

	QString str;
	str.setNum(vegetationExtractor->getEntitys()->size());
	QMessageBox::information(this, tr("information"), tr("tree number :") + str, QMessageBox::Ok);
}
*/

/**
* Auto run action
*/
void MainUI::autoRun()
{
	AutoRunDlg dlg(this);
	dlg.exec();
}

/**
* Show dataset information
*/
void MainUI::datasetInfo() 
{
	if(this->_workSpace == NULL) 
	{
		QMessageBox::critical(this, tr("error"), tr("create or open project first!"), QMessageBox::Ok);
		return;
	}

	PointInfoDlg dlg(this->_workSpace->getGridIndex()->getLASHeader(), this);
	dlg.exec();	
}

/**
* Import DTM to the height of objects
*/
void MainUI::importDTM()
{
	QString fileName;
	GridIndex *gridIndex = NULL;
	if(this->_workSpace == NULL)
	{
		QMessageBox::critical(this, tr("error"), tr("create or open project first!"), QMessageBox::Ok);
		return;
	}
	fileName = GetOpenFilePath(this, tr("DTM file"), tr("DTM file (*.img)"));
	if (!fileName.isNull()) 
	{
		gridIndex = this->_workSpace->getGridIndex();
		gridIndex->importDTM(fileName.toStdString());
	}
}

/**
* Import points and draw them 
*/
void MainUI::importPoint()
{
	QString fileName;
	FILE *fp = NULL;
	int x = 0, y = 0;	
	vector<PT> points;

	if(this->_workSpace == NULL)
	{
		QMessageBox::critical(this, tr("error"), tr("create or open project first!"), QMessageBox::Ok);
		return;
	}
	fileName = GetOpenFilePath(this, tr("point file"), tr("txt file (*.pt)"));
	if (!fileName.isNull()) 
	{
		fp = fopen(fileName.toStdString().c_str(), "r");
		while (fscanf(fp, "%d %d\n", &x, &y) != EOF)
		{
			points.push_back(PT(x, y));
		}
		fclose(fp);

		this->_ptView->addPoints(points);		
	}
}

/**
* Import rectangles and draw them 
*/
void MainUI::importRectangle()
{	
	GridIndex *gridIndex = NULL;
	FILE *fp = NULL;
	double maxObjSize = 0.0;
	int x = 0, y = 0, rectW = 0;
	QRect rect;
	vector<QRect> rects;

	if(this->_workSpace == NULL)
	{
		QMessageBox::critical(this, tr("error"), tr("create or open project first!"), QMessageBox::Ok);
		return;
	}
	
	gridIndex = this->_workSpace->getGridIndex();
	QString fileName = GetOpenFilePath(this, tr("rectangle file"), tr("txt file (*.rect)"));
    if (!fileName.isNull()) 
	{
		fp = fopen(fileName.toStdString().c_str(), "r");
		fscanf(fp, "%lf\n", &maxObjSize);
		rectW = maxObjSize / gridIndex->getCellSize();
		
		while (fscanf(fp, "%d %d\n", &x, &y) != EOF)
		{
			rect.setLeft(x*rectW);
			rect.setRight((x+1)*rectW);
			rect.setTop(y*rectW);
			rect.setBottom((y+1)*rectW);
			rects.push_back(rect);
		}
		fclose(fp);

		this->_ptView->addRectangles(rects);
	}
}

/**
* Import TIN points and draw them 
*/
void MainUI::importTIN()
{
	FILE *fp = NULL;
	int x = 0, y = 0;	
	vector<PT> tinPoints;

	if(this->_workSpace == NULL)
	{
		QMessageBox::critical(this, tr("error"), tr("create or open project first!"), QMessageBox::Ok);
		return;
	}
	QString fileName = GetOpenFilePath(this, tr("tin file"), tr("txt file (*.tin)"));
    if (!fileName.isNull()) 
	{
		fp = fopen(fileName.toStdString().c_str(), "r");
		while (fscanf(fp, "%d %d\n", &x, &y) != EOF)
		{
			tinPoints.push_back(PT(x, y));
		}
		fclose(fp);

		this->_ptView->addTINPts(tinPoints);
	}
}

/**
* Text to LAS convertion
*/
void MainUI::text2las()
{
	Text2LasDlg dlg(this);
	dlg.exec();
}

/**
* LAS to text convertion
*/
void MainUI::las2text()
{
	Las2TextDlg dlg(this);
	dlg.exec();
}

/**
* About action
*/
void MainUI::about()
{
	QMessageBox::about(this, tr("About ALS"), tr("Airborne Laser scanning processing System (ALS)\n author: yanmenglong\n email:yanmenglong@gmail.com"));
}

/**
*
*/
void MainUI::showMousePos(const int& mx, const int & my)
{
	GridIndex *gridIndex = NULL;
	int gridW = 0, gridH = 0;
	PT3D *pt3d = NULL;
	LASHeader *lasHeader = NULL;

	if(this->_workSpace == NULL)
	{
		return;
	}

	gridIndex = this->_workSpace->getGridIndex();
	lasHeader = gridIndex->getLASHeader();
	pt3d = gridIndex->getMinPT3D(mx, my);
	if(pt3d == NULL)
	{
		return;
	}
	QString str = QString("%1, %2, %3").arg(lasHeader->xMin+pt3d->x, 0, 'f', 3).arg(lasHeader->yMin+pt3d->y, 0, 'f', 3).arg(pt3d->z, 0, 'f', 3);
	this->statusBar()->showMessage(str);
}

void MainUI::identifyObject(const int& mx, const int& my)
{
	GridIndex *gridIndex = NULL;
	int gridW = 0, gridH = 0, ii = 0, ID = 0;
	LASHeader *lasHeader = NULL;
	PT nhbor4[4], nhbor8[8];
	PT3D *pt3d = NULL;
	vector<PT> points;
	hash_map<int, Entity*> *entitys = NULL;
	Entity *entity = NULL;
	map<string, double> parameters;
	QString header, content, item;
	vector<int> IDs;

	if(this->_workSpace == NULL)
	{
		return;
	}

	gridIndex = this->_workSpace->getGridIndex();
	gridW = gridIndex->getGridWidth();
	gridH = gridIndex->getGridHeight();
	lasHeader = gridIndex->getLASHeader();

	if(mx < 0 || mx >= gridW || my < 0 || my >= gridH)
	{
		return;
	}
	
	if(this->_dockWidget->isVisible() && this->_step == ProcessStep::none) // For neighbourhood index demonstration
	{
		QString header, content, item;
		QStringList strList1, strList2;
		
		strList2<<"points here";
		pt3d = gridIndex->getMinPT3D(mx, my); 
		if(pt3d != NULL)
		{
			strList2<<QString("min-> id:%1 (%2, %3, %4)").arg(pt3d->pos).arg(lasHeader->xMin+pt3d->x, 0, 'f', 3).arg(lasHeader->yMin+pt3d->y, 0, 'f', 3).arg(pt3d->z, 0, 'f', 3);
		} 
		pt3d = gridIndex->getMaxPT3D(mx, my); 
		if(pt3d != NULL)
		{
			strList2<<QString("max-> id:%1 (%2, %3, %4)").arg(pt3d->pos).arg(lasHeader->xMin+pt3d->x, 0, 'f', 3).arg(lasHeader->yMin+pt3d->y, 0, 'f', 3).arg(pt3d->z, 0, 'f', 3);
		} 
		else 
		{
			strList2<<QString("no las point here!");
		}	
		strList1<<strList2.join(token2);
		
		strList2.clear();
		strList2<<"4 neighbour points";
		gridIndex->get4NeighPTs(mx, my, nhbor4);		
		for(ii = 0; ii < 4; ++ii)
		{
			pt3d = gridIndex->getMinPT3D(nhbor4[ii].x, nhbor4[ii].y);
			if(pt3d == NULL)
			{
				continue;
			}
			strList2<<QString("id:%1 (%2, %3, %4)").arg(pt3d->pos).arg(lasHeader->xMin+pt3d->x, 0, 'f', 3).arg(lasHeader->yMin+pt3d->y, 0, 'f', 3).arg(pt3d->z, 0, 'f', 3);
		}
		strList1<<strList2.join(token2);

		strList2.clear();
		strList2<<"8 neighbour points";
		gridIndex->get8NeighPTs(mx, my, nhbor8);		
		for(ii = 0; ii < 8; ++ii)
		{
			pt3d = gridIndex->getMinPT3D(nhbor8[ii].x, nhbor8[ii].y);
			if(pt3d == NULL)
			{
				continue;
			}
			strList2<<QString("id:%1 (%2, %3, %4)").arg(pt3d->pos).arg(lasHeader->xMin+pt3d->x, 0, 'f', 3).arg(lasHeader->yMin+pt3d->y, 0, 'f', 3).arg(pt3d->z, 0, 'f', 3);
		}
		strList1<<strList2.join(token2);

		header = QString("cell(%1, %2)").arg(mx).arg(my);
		content = strList1.join(token1);
		emit updateNodeInfo(header, content);

		points.push_back(PT(mx, my));
		this->_ptView->addPoints(points);
		return;
	}	
	else if(this->_step == ProcessStep::obafilter_filtering) // Show filtering segmentation objects
	{
		OBAFilter *obaFilter = this->_workSpace->getOBAFilter();
		ID = obaFilter->getEntityID(mx, my);
		if(ID == NAN)
		{
			return;
		}

		entitys = obaFilter->getEntitys();
	} 
	else if(this->_step == ProcessStep::obaclassifier_classifying) // Show classification segmentation objects
	{
		OBAClassifier *obaClassifier = this->_workSpace->getOBAClassifier();
		ID = obaClassifier->getEntityID(mx, my);
		if(ID == NAN)
		{
			return;
		}

		entitys = obaClassifier->getEntitys();
	}	
	else if(this->_step == ProcessStep::tree_extraction) // Show tree object
	{
		VegetationExtractor* vegetationExtractor = this->_workSpace->getVegetationExtractor();
		ID = vegetationExtractor->getEntityID(mx, my);
		if(ID == NAN)
		{
			return;
		}

		entitys = vegetationExtractor->getEntitys();
	}

	if(entitys == NULL)
	{
		return;
	}
	
	entity = entitys->at(ID);
	parameters = entity->getParameters();
	
	header = QString("object id: %1").arg(ID);
	QStringList strList;

	strList<<QString("area: %1  (m^2)").arg(parameters[areaTag], 0, 'f', 3);
	strList<<QString("mean: %1  (m)").arg(parameters[meanTag], 0, 'f', 3);
	strList<<QString("variance: %1  (m)").arg(parameters[varianceTag], 0, 'f', 3);
	strList<<QString("cellZDiff: %1  (m)").arg(parameters[cellZDiffTag], 0, 'f', 3);
	strList<<QString("boundingZDiff: %1  (m)").arg(parameters[boundingZDiffTag], 0, 'f', 3);
	strList<<QString("boundingH: %1  (m)").arg(parameters[boundingHTag], 0, 'f', 3);
	strList<<QString("boundingHVari: %1  (m)").arg(parameters[boundingHVariTag], 0, 'f', 3);
	strList<<QString("perimeter: %1  (m)").arg(parameters[perimeterTag], 0, 'f', 3);
	strList<<QString("mbrLength: %1  (m)").arg(parameters[mbrLengthTag], 0, 'f', 3);
	strList<<QString("mbrWidth: %1  (m)").arg(parameters[mbrWidthTag], 0, 'f', 3);
	strList<<QString("mbrPerimeter: %1  (m)").arg(parameters[mbrPerimeterTag], 0, 'f', 3);
	strList<<QString("perimeterAndMbrPerimeterRate: %1").arg(parameters[perimeterAndMbrPerimeterRateTag], 0, 'f', 3);
	strList<<QString("mbrArea: %1  (m^2)").arg(parameters[mbrAreaTag], 0, 'f', 3);
	strList<<QString("areaAndMbrAreaRate: %1").arg(parameters[areaAndMbrAreaRateTag], 0, 'f', 3);
	strList<<QString("compactness: %1").arg(parameters[compactnessTag], 0, 'f', 3);
	strList<<QString("topology: %1").arg(parameters[topologyTag]);

	content = strList.join(token1);
	IDs.push_back(ID);
	this->_ptView->showObjectBoundings(entitys, IDs);

	emit this->updateNodeInfo(header, content);	
}

/**
* Cancel a building with given id
*
* id: the id of entity
*/
void MainUI::cancelBuilding(const int& id)
{
	OBAClassifier *obaClassifier = this->_workSpace->getOBAClassifier();
	Entity *entity = obaClassifier->getEntity(id);
	entity->setType(PointType::p_high_vegetation);
}

/**
* Classifiy the current entitys by given parameter with minimum value and maximum value restriction
*
* paramTag: the parameter tag
* currentID: the id of current entity
*/
void MainUI::useEntityParam(const QString &paramTag, const int &currentID)
{
	OBAClassifier *obaClassifier = NULL;
	hash_map<int, Entity*>* entitys = NULL;
	Entity *entity = NULL;
	std::map<std::string, double> parameters;
	double value = 0.0, valueMin = 0.0, valueMax = 0.0;

	if(this->_step != ProcessStep::obaclassifier_classifying)
	{
		return;
	}

	obaClassifier = this->_workSpace->getOBAClassifier();
	entitys = obaClassifier->getEntitys();
	entity = (*entitys)[currentID];
	parameters = entity->getParameters();
	value = parameters[paramTag.toStdString()];

	EntityParamThresholdDlg dlg(this, paramTag, value);
	if(dlg.exec() != QDialog::Accepted)
	{
		return;
	}

	valueMin = dlg.getValueMin();
	valueMax = dlg.getValueMax();
	obaClassifier->classifyEntitys(paramTag.toStdString(), valueMin, valueMax);

	// Update the point view
	emit this->updatePointView(entitys);
}

void MainUI::resetEntitysType()
{
	OBAClassifier *obaClassifier = NULL;
	hash_map<int, Entity*>* entitys = NULL;

	if(this->_step != ProcessStep::obaclassifier_classifying)
	{
		return;
	}

	obaClassifier = this->_workSpace->getOBAClassifier();
	obaClassifier->resetEntitysType();
	entitys = obaClassifier->getEntitys();	

	// Update the point view
	emit this->updatePointView(entitys);
}