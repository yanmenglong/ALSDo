#include "treeinfowidget.h"

#include <QAction>
#include <QMessageBox>
#include <QList>
#include <QFont>

QString token1 = "$1$"; 
QString token2 = "$2$";

TreeInfoWidget::TreeInfoWidget(QWidget *parent)
	: QTreeWidget(parent)
{
	this->setColumnCount(1);
	this->setHeaderHidden(true);	
	QTreeWidgetItem *root = new QTreeWidgetItem(this);
	root->setText(0, "data");
	root->setIcon(0, QIcon(tr("./icons/dataview.png")));
	root->setSizeHint(0, QSize(25, 25));
	root->setFont(0, QFont("Times", 12, 12, false));
	this->insertTopLevelItem(0, root); 
	this->expandAll();
	QObject::connect(this, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(itemSelectClicked(QTreeWidgetItem*, int)));
}

TreeInfoWidget::~TreeInfoWidget()
{

}

QSize TreeInfoWidget::sizeHint() const
{
	return QSize(300, 300);
}

void TreeInfoWidget::updateNodeInfo(QString &header, QString &content)
{
	this->clear();
	QTreeWidgetItem *root = new QTreeWidgetItem(this);
	root->setText(0, header);
	root->setIcon(0, QIcon(tr("./icons/dataview.png")));
	root->setSizeHint(0, QSize(25, 25));
	root->setFont(0, QFont("Times", 12, 12, false));
	this->insertTopLevelItem(0, root); 
	
	QStringList strList = content.split(token1), itemList;
	QString str, item;
	QTreeWidgetItem *leaf1 = NULL, *leaf2 = NULL; 
	for (int ii = 0; ii < strList.size(); ++ii) 
	{
		str = strList.at(ii);
		if(!str.contains(token2)) 
		{
			leaf1 = new QTreeWidgetItem(root);
			leaf1->setText(0, str);
			leaf1->setSizeHint(0, QSize(20, 20));
			leaf1->setFont(0, QFont("Times", 11, 11, false));
			//leaf1->setTextColor(0, info.color());
			root->addChild(leaf1);
		}
		else 
		{
			itemList = str.split(token2);
			leaf1 = new QTreeWidgetItem(root);
			leaf1->setText(0, itemList.at(0));
			leaf1->setSizeHint(0, QSize(20, 20));
			leaf1->setFont(0, QFont("Times", 11, 11, false));
			root->addChild(leaf1);

			for (int jj = 1; jj < itemList.size(); ++jj) 
			{
				item = itemList.at(jj);
				leaf2 = new QTreeWidgetItem(leaf1);
				leaf2->setText(0, item);
				leaf2->setSizeHint(0, QSize(20, 20));
				leaf2->setFont(0, QFont("Times", 11, 11, false));
				leaf1->addChild(leaf2);
			}
		}
	}

	this->expandAll();
}

void TreeInfoWidget::itemSelectClicked(QTreeWidgetItem * item, int column)
{
	QTreeWidgetItem *itemParent = item->parent();
	QString parentText, itemText;
	QStringList strList;
	int currentID = 0;
	if(itemParent == NULL)
	{
		return;
	}

	parentText = itemParent->text(0);
	itemText = item->text(0);
	if(!itemText.startsWith("object id"))
	{
		strList = parentText.split(":", QString::SkipEmptyParts);
		currentID = strList.last().toInt();
		strList = itemText.split(":", QString::SkipEmptyParts);
		emit useEntityParam(strList.first(), currentID);
	}
}