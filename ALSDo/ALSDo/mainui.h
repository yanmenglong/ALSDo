#ifndef MAINUI_H
#define MAINUI_H

#include <QtGui/QMainWindow>
#include <QDockWidget>

class PointView;
class WorkSpace;
class TreeInfoWidget;
class EditorWindow;
class Entity;

#include <hash_map>
using namespace std;
#ifdef WIN32
	using namespace stdext;
#else
	using namespace __gun_cxx;
#endif

enum ProcessStep
{
	none = 0,
	obafilter_filtering = 1,
	obaclassifier_classifying = 2,
	tree_extraction = 3
};

class MainUI : public QMainWindow
{
Q_OBJECT

public:
	MainUI(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainUI();

private slots:
	void newProject();
	void quit();

	void showAllObjects();
	void saveCurrentScreen();
	void itemView();
	void editorView();

	void filterPtsByOBAFilter();
	void classifyObjs();
	void resetEntitysType();
	void extractBuildingManual();
	void exportBuilding();
	//void treeExtract();	
	void autoRun();

	void datasetInfo();	
	void importDTM();
	void importPoint();
	void importRectangle();
	void importTIN();
	void text2las();
	void las2text();	

	void about();	

	// This is the slots respose to the signals that do not belong to this class
	void showMousePos(const int& mx, const int & my);
	void identifyObject(const int& mx, const int& my);
	void cancelBuilding(const int& id);
	void useEntityParam(const QString &paramTag, const int &currentID);	

signals:
	void updatePointView(hash_map<int, Entity*> *entitys);
	void updateNodeInfo(QString &header, QString &content);

private:
    void createUI();

	WorkSpace *_workSpace;
	PointView *_ptView;
	TreeInfoWidget *_treeInfoWidget;
	QDockWidget *_dockWidget;
	EditorWindow *_editorWindow;
	ProcessStep _step;
};

#endif // MAINUI_H
