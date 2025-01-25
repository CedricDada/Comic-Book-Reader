/****************************************************************************
** Meta object code from reading C++ file 'BookManager.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../model/BookManager.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BookManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSBookManagerENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSBookManagerENDCLASS = QtMocHelpers::stringData(
    "BookManager",
    "bookReady",
    "",
    "AbstractBook*",
    "book",
    "pageProcessed",
    "index",
    "openFailed",
    "error"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSBookManagerENDCLASS_t {
    uint offsetsAndSizes[18];
    char stringdata0[12];
    char stringdata1[10];
    char stringdata2[1];
    char stringdata3[14];
    char stringdata4[5];
    char stringdata5[14];
    char stringdata6[6];
    char stringdata7[11];
    char stringdata8[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSBookManagerENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSBookManagerENDCLASS_t qt_meta_stringdata_CLASSBookManagerENDCLASS = {
    {
        QT_MOC_LITERAL(0, 11),  // "BookManager"
        QT_MOC_LITERAL(12, 9),  // "bookReady"
        QT_MOC_LITERAL(22, 0),  // ""
        QT_MOC_LITERAL(23, 13),  // "AbstractBook*"
        QT_MOC_LITERAL(37, 4),  // "book"
        QT_MOC_LITERAL(42, 13),  // "pageProcessed"
        QT_MOC_LITERAL(56, 5),  // "index"
        QT_MOC_LITERAL(62, 10),  // "openFailed"
        QT_MOC_LITERAL(73, 5)   // "error"
    },
    "BookManager",
    "bookReady",
    "",
    "AbstractBook*",
    "book",
    "pageProcessed",
    "index",
    "openFailed",
    "error"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSBookManagerENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   32,    2, 0x06,    1 /* Public */,
       5,    1,   35,    2, 0x06,    3 /* Public */,
       7,    1,   38,    2, 0x06,    5 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::QString,    8,

       0        // eod
};

Q_CONSTINIT const QMetaObject BookManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSBookManagerENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSBookManagerENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSBookManagerENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<BookManager, std::true_type>,
        // method 'bookReady'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<AbstractBook *, std::false_type>,
        // method 'pageProcessed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'openFailed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void BookManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<BookManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->bookReady((*reinterpret_cast< std::add_pointer_t<AbstractBook*>>(_a[1]))); break;
        case 1: _t->pageProcessed((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->openFailed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (BookManager::*)(AbstractBook * );
            if (_t _q_method = &BookManager::bookReady; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (BookManager::*)(int );
            if (_t _q_method = &BookManager::pageProcessed; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (BookManager::*)(const QString & );
            if (_t _q_method = &BookManager::openFailed; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject *BookManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BookManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSBookManagerENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int BookManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void BookManager::bookReady(AbstractBook * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BookManager::pageProcessed(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void BookManager::openFailed(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
