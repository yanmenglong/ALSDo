/****************************************************************************
** Meta object code from reading C++ file 'obafilterparamdlg.h'
**
** Created: Mon Mar 31 08:26:34 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../obafilterparamdlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'obafilterparamdlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_OBAFilterParamDlg[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_OBAFilterParamDlg[] = {
    "OBAFilterParamDlg\0"
};

const QMetaObject OBAFilterParamDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_OBAFilterParamDlg,
      qt_meta_data_OBAFilterParamDlg, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &OBAFilterParamDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *OBAFilterParamDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *OBAFilterParamDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OBAFilterParamDlg))
        return static_cast<void*>(const_cast< OBAFilterParamDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int OBAFilterParamDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
