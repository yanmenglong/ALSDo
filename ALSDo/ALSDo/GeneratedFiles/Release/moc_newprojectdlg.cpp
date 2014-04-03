/****************************************************************************
** Meta object code from reading C++ file 'newprojectdlg.h'
**
** Created: Mon Mar 31 08:28:45 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../newprojectdlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'newprojectdlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_NewProjectDlg[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x08,
      33,   14,   14,   14, 0x08,
      61,   55,   14,   14, 0x08,
      81,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_NewProjectDlg[] = {
    "NewProjectDlg\0\0clickLasOpenBtn()\0"
    "clickProjectOpenBtn()\0index\0"
    "tabIndexChange(int)\0gridSizeValueChanged()\0"
};

const QMetaObject NewProjectDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_NewProjectDlg,
      qt_meta_data_NewProjectDlg, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &NewProjectDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *NewProjectDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *NewProjectDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NewProjectDlg))
        return static_cast<void*>(const_cast< NewProjectDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int NewProjectDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: clickLasOpenBtn(); break;
        case 1: clickProjectOpenBtn(); break;
        case 2: tabIndexChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: gridSizeValueChanged(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
