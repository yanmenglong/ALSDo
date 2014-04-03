/****************************************************************************
** Meta object code from reading C++ file 'pointview.h'
**
** Created: Mon Mar 31 08:28:45 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../pointview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pointview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PointView[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   11,   10,   10, 0x05,
      39,   11,   10,   10, 0x05,
      66,   63,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      94,   86,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_PointView[] = {
    "PointView\0\0mx,my\0showMousePos(int,int)\0"
    "identifyObject(int,int)\0ID\0"
    "cancelBuilding(int)\0entitys\0"
    "updateRegion(hash_map<int,Entity*>*)\0"
};

const QMetaObject PointView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_PointView,
      qt_meta_data_PointView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PointView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PointView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PointView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PointView))
        return static_cast<void*>(const_cast< PointView*>(this));
    return QWidget::qt_metacast(_clname);
}

int PointView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: showMousePos((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        case 1: identifyObject((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        case 2: cancelBuilding((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 3: updateRegion((*reinterpret_cast< hash_map<int,Entity*>*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void PointView::showMousePos(const int & _t1, const int & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PointView::identifyObject(const int & _t1, const int & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void PointView::cancelBuilding(const int & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
