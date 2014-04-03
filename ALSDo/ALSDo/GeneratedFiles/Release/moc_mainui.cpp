/****************************************************************************
** Meta object code from reading C++ file 'mainui.h'
**
** Created: Mon Mar 31 08:28:45 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../mainui.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainUI[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      26,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,    8,    7,    7, 0x05,
      71,   56,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
     105,    7,    7,    7, 0x08,
     118,    7,    7,    7, 0x08,
     125,    7,    7,    7, 0x08,
     142,    7,    7,    7, 0x08,
     162,    7,    7,    7, 0x08,
     173,    7,    7,    7, 0x08,
     186,    7,    7,    7, 0x08,
     209,    7,    7,    7, 0x08,
     224,    7,    7,    7, 0x08,
     243,    7,    7,    7, 0x08,
     267,    7,    7,    7, 0x08,
     284,    7,    7,    7, 0x08,
     294,    7,    7,    7, 0x08,
     308,    7,    7,    7, 0x08,
     320,    7,    7,    7, 0x08,
     334,    7,    7,    7, 0x08,
     352,    7,    7,    7, 0x08,
     364,    7,    7,    7, 0x08,
     375,    7,    7,    7, 0x08,
     386,    7,    7,    7, 0x08,
     400,  394,    7,    7, 0x08,
     422,  394,    7,    7, 0x08,
     449,  446,    7,    7, 0x08,
     488,  469,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainUI[] = {
    "MainUI\0\0entitys\0updatePointView(hash_map<int,Entity*>*)\0"
    "header,content\0updateNodeInfo(QString&,QString&)\0"
    "newProject()\0quit()\0showAllObjects()\0"
    "saveCurrentScreen()\0itemView()\0"
    "editorView()\0filterPtsByOBAFilter()\0"
    "classifyObjs()\0resetEntitysType()\0"
    "extractBuildingManual()\0exportBuilding()\0"
    "autoRun()\0datasetInfo()\0importDTM()\0"
    "importPoint()\0importRectangle()\0"
    "importTIN()\0text2las()\0las2text()\0"
    "about()\0mx,my\0showMousePos(int,int)\0"
    "identifyObject(int,int)\0id\0"
    "cancelBuilding(int)\0paramTag,currentID\0"
    "useEntityParam(QString,int)\0"
};

const QMetaObject MainUI::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainUI,
      qt_meta_data_MainUI, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainUI::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainUI::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainUI))
        return static_cast<void*>(const_cast< MainUI*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: updatePointView((*reinterpret_cast< hash_map<int,Entity*>*(*)>(_a[1]))); break;
        case 1: updateNodeInfo((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: newProject(); break;
        case 3: quit(); break;
        case 4: showAllObjects(); break;
        case 5: saveCurrentScreen(); break;
        case 6: itemView(); break;
        case 7: editorView(); break;
        case 8: filterPtsByOBAFilter(); break;
        case 9: classifyObjs(); break;
        case 10: resetEntitysType(); break;
        case 11: extractBuildingManual(); break;
        case 12: exportBuilding(); break;
        case 13: autoRun(); break;
        case 14: datasetInfo(); break;
        case 15: importDTM(); break;
        case 16: importPoint(); break;
        case 17: importRectangle(); break;
        case 18: importTIN(); break;
        case 19: text2las(); break;
        case 20: las2text(); break;
        case 21: about(); break;
        case 22: showMousePos((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        case 23: identifyObject((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        case 24: cancelBuilding((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 25: useEntityParam((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 26;
    }
    return _id;
}

// SIGNAL 0
void MainUI::updatePointView(hash_map<int,Entity*> * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainUI::updateNodeInfo(QString & _t1, QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
