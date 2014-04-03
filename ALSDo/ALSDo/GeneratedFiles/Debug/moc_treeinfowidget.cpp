/****************************************************************************
** Meta object code from reading C++ file 'treeinfowidget.h'
**
** Created: Mon Mar 31 08:26:34 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../treeinfowidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'treeinfowidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TreeInfoWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      35,   16,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      78,   63,   15,   15, 0x08,
     124,  112,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_TreeInfoWidget[] = {
    "TreeInfoWidget\0\0paramTag,currentID\0"
    "useEntityParam(QString,int)\0header,content\0"
    "updateNodeInfo(QString&,QString&)\0"
    "item,column\0itemSelectClicked(QTreeWidgetItem*,int)\0"
};

const QMetaObject TreeInfoWidget::staticMetaObject = {
    { &QTreeWidget::staticMetaObject, qt_meta_stringdata_TreeInfoWidget,
      qt_meta_data_TreeInfoWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TreeInfoWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TreeInfoWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TreeInfoWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TreeInfoWidget))
        return static_cast<void*>(const_cast< TreeInfoWidget*>(this));
    return QTreeWidget::qt_metacast(_clname);
}

int TreeInfoWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: useEntityParam((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        case 1: updateNodeInfo((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: itemSelectClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void TreeInfoWidget::useEntityParam(const QString & _t1, const int & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
