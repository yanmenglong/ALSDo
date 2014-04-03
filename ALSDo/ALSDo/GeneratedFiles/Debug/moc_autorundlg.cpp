/****************************************************************************
** Meta object code from reading C++ file 'autorundlg.h'
**
** Created: Mon Mar 31 08:26:34 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../autorundlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'autorundlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AutoRunDlg[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      30,   11,   11,   11, 0x08,
      52,   11,   11,   11, 0x08,
      66,   11,   11,   11, 0x08,
      89,   11,   11,   11, 0x08,
     144,  131,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_AutoRunDlg[] = {
    "AutoRunDlg\0\0clickLasOpenBtn()\0"
    "clickProjectOpenBtn()\0clickRunBtn()\0"
    "cellSizeValueChanged()\0"
    "useOBASeedGeneratorCheckBoxStateChanged()\0"
    "percent,text\0showStepInformation(int,QString)\0"
};

const QMetaObject AutoRunDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_AutoRunDlg,
      qt_meta_data_AutoRunDlg, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AutoRunDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AutoRunDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AutoRunDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AutoRunDlg))
        return static_cast<void*>(const_cast< AutoRunDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int AutoRunDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: clickLasOpenBtn(); break;
        case 1: clickProjectOpenBtn(); break;
        case 2: clickRunBtn(); break;
        case 3: cellSizeValueChanged(); break;
        case 4: useOBASeedGeneratorCheckBoxStateChanged(); break;
        case 5: showStepInformation((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
