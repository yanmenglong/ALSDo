#ifndef TREEINFOWIDGET_H
#define TREEINFOWIDGET_H

#include <QMenu>
#include <QMouseEvent>
#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>

class TreeInfoWidget : public QTreeWidget
{
	Q_OBJECT

public:
	TreeInfoWidget(QWidget *parent = 0);
	~TreeInfoWidget();

	QSize sizeHint() const;

private slots:
     void updateNodeInfo(QString &header, QString &content);
	 void itemSelectClicked(QTreeWidgetItem * item, int column);
	 
signals:
	 void useEntityParam(const QString &paramTag, const int &currentID);
};

#endif 
