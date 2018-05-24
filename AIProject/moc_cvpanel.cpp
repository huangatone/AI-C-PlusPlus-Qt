/****************************************************************************
** Meta object code from reading C++ file 'cvpanel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "cvpanel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cvpanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CVPanel_t {
    QByteArrayData data[18];
    char stringdata0[264];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CVPanel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CVPanel_t qt_meta_stringdata_CVPanel = {
    {
QT_MOC_LITERAL(0, 0, 7), // "CVPanel"
QT_MOC_LITERAL(1, 8, 31), // "on_treeWidget_itemDoubleClicked"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(4, 58, 4), // "item"
QT_MOC_LITERAL(5, 63, 6), // "column"
QT_MOC_LITERAL(6, 70, 25), // "on_treeWidget_itemClicked"
QT_MOC_LITERAL(7, 96, 16), // "on_btnDo_clicked"
QT_MOC_LITERAL(8, 113, 11), // "slotSetFile"
QT_MOC_LITERAL(9, 125, 11), // "slotPreview"
QT_MOC_LITERAL(10, 137, 23), // "on_functionList_clicked"
QT_MOC_LITERAL(11, 161, 5), // "index"
QT_MOC_LITERAL(12, 167, 16), // "slot_btn_clicked"
QT_MOC_LITERAL(13, 184, 21), // "on_pushButton_clicked"
QT_MOC_LITERAL(14, 206, 23), // "slot_currentItemChanged"
QT_MOC_LITERAL(15, 230, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(16, 247, 7), // "current"
QT_MOC_LITERAL(17, 255, 8) // "previous"

    },
    "CVPanel\0on_treeWidget_itemDoubleClicked\0"
    "\0QTreeWidgetItem*\0item\0column\0"
    "on_treeWidget_itemClicked\0on_btnDo_clicked\0"
    "slotSetFile\0slotPreview\0on_functionList_clicked\0"
    "index\0slot_btn_clicked\0on_pushButton_clicked\0"
    "slot_currentItemChanged\0QListWidgetItem*\0"
    "current\0previous"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CVPanel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   59,    2, 0x09 /* Protected */,
       6,    2,   64,    2, 0x09 /* Protected */,
       7,    0,   69,    2, 0x09 /* Protected */,
       8,    0,   70,    2, 0x09 /* Protected */,
       9,    0,   71,    2, 0x09 /* Protected */,
      10,    1,   72,    2, 0x09 /* Protected */,
      12,    0,   75,    2, 0x09 /* Protected */,
      13,    0,   76,    2, 0x08 /* Private */,
      14,    2,   77,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int,    4,    5,
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int,    4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QModelIndex,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 15, 0x80000000 | 15,   16,   17,

       0        // eod
};

void CVPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CVPanel *_t = static_cast<CVPanel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_treeWidget_itemDoubleClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->on_treeWidget_itemClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->on_btnDo_clicked(); break;
        case 3: _t->slotSetFile(); break;
        case 4: _t->slotPreview(); break;
        case 5: _t->on_functionList_clicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 6: _t->slot_btn_clicked(); break;
        case 7: _t->on_pushButton_clicked(); break;
        case 8: _t->slot_currentItemChanged((*reinterpret_cast< QListWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QListWidgetItem*(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject CVPanel::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CVPanel.data,
      qt_meta_data_CVPanel,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CVPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CVPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CVPanel.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CVPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
