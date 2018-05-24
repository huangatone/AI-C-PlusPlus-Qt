/****************************************************************************
** Meta object code from reading C++ file 'dealview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dealview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dealview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DealView_t {
    QByteArrayData data[16];
    char stringdata0[281];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DealView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DealView_t qt_meta_stringdata_DealView = {
    {
QT_MOC_LITERAL(0, 0, 8), // "DealView"
QT_MOC_LITERAL(1, 9, 25), // "on_listWidget_itemClicked"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(4, 53, 4), // "item"
QT_MOC_LITERAL(5, 58, 17), // "on_btnAdd_clicked"
QT_MOC_LITERAL(6, 76, 17), // "on_btnDel_clicked"
QT_MOC_LITERAL(7, 94, 17), // "on_btnRun_clicked"
QT_MOC_LITERAL(8, 112, 21), // "on_btnRefresh_clicked"
QT_MOC_LITERAL(9, 134, 19), // "on_btnReset_clicked"
QT_MOC_LITERAL(10, 154, 25), // "on_btnAddFromTree_clicked"
QT_MOC_LITERAL(11, 180, 22), // "on_sliderA_sliderMoved"
QT_MOC_LITERAL(12, 203, 8), // "position"
QT_MOC_LITERAL(13, 212, 22), // "on_sliderB_sliderMoved"
QT_MOC_LITERAL(14, 235, 22), // "on_sliderC_sliderMoved"
QT_MOC_LITERAL(15, 258, 22) // "on_sliderD_sliderMoved"

    },
    "DealView\0on_listWidget_itemClicked\0\0"
    "QListWidgetItem*\0item\0on_btnAdd_clicked\0"
    "on_btnDel_clicked\0on_btnRun_clicked\0"
    "on_btnRefresh_clicked\0on_btnReset_clicked\0"
    "on_btnAddFromTree_clicked\0"
    "on_sliderA_sliderMoved\0position\0"
    "on_sliderB_sliderMoved\0on_sliderC_sliderMoved\0"
    "on_sliderD_sliderMoved"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DealView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x08 /* Private */,
       5,    0,   72,    2, 0x08 /* Private */,
       6,    0,   73,    2, 0x08 /* Private */,
       7,    0,   74,    2, 0x08 /* Private */,
       8,    0,   75,    2, 0x08 /* Private */,
       9,    0,   76,    2, 0x08 /* Private */,
      10,    0,   77,    2, 0x08 /* Private */,
      11,    1,   78,    2, 0x08 /* Private */,
      13,    1,   81,    2, 0x08 /* Private */,
      14,    1,   84,    2, 0x08 /* Private */,
      15,    1,   87,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   12,

       0        // eod
};

void DealView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DealView *_t = static_cast<DealView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_listWidget_itemClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 1: _t->on_btnAdd_clicked(); break;
        case 2: _t->on_btnDel_clicked(); break;
        case 3: _t->on_btnRun_clicked(); break;
        case 4: _t->on_btnRefresh_clicked(); break;
        case 5: _t->on_btnReset_clicked(); break;
        case 6: _t->on_btnAddFromTree_clicked(); break;
        case 7: _t->on_sliderA_sliderMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->on_sliderB_sliderMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->on_sliderC_sliderMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->on_sliderD_sliderMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject DealView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_DealView.data,
      qt_meta_data_DealView,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DealView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DealView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DealView.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int DealView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
