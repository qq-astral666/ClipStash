/****************************************************************************
** Meta object code from reading C++ file 'ClipModel.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/app/ClipModel.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ClipModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.2. It"
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
struct qt_meta_tag_ZN9ClipModelE_t {};
} // unnamed namespace

template <> constexpr inline auto ClipModel::qt_create_metaobjectdata<qt_meta_tag_ZN9ClipModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ClipModel",
        "QML.Element",
        "auto",
        "QML.Creatable",
        "false",
        "QML.UncreatableReason",
        "ClipModel is owned by AppController",
        "searchTextChanged",
        "",
        "filterChanged",
        "countChanged",
        "reload",
        "indexOfId",
        "id",
        "searchText",
        "filter",
        "count",
        "Role",
        "ClipIdRole",
        "ClipTypeRole",
        "ClipTextRole",
        "PreviewRole",
        "SourceAppRole",
        "SourceBundleRole",
        "CreatedAtRole",
        "PinnedRole",
        "UseCountRole",
        "ImageWidthRole",
        "ImageHeightRole",
        "CharCountRole",
        "LineCountRole",
        "SectionNameRole"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'searchTextChanged'
        QtMocHelpers::SignalData<void()>(7, 8, QMC::AccessPublic, QMetaType::Void),
        // Signal 'filterChanged'
        QtMocHelpers::SignalData<void()>(9, 8, QMC::AccessPublic, QMetaType::Void),
        // Signal 'countChanged'
        QtMocHelpers::SignalData<void()>(10, 8, QMC::AccessPublic, QMetaType::Void),
        // Method 'reload'
        QtMocHelpers::MethodData<void()>(11, 8, QMC::AccessPublic, QMetaType::Void),
        // Method 'indexOfId'
        QtMocHelpers::MethodData<int(qint64) const>(12, 8, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::LongLong, 13 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'searchText'
        QtMocHelpers::PropertyData<QString>(14, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'filter'
        QtMocHelpers::PropertyData<int>(15, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 1),
        // property 'count'
        QtMocHelpers::PropertyData<int>(16, QMetaType::Int, QMC::DefaultPropertyFlags, 2),
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'Role'
        QtMocHelpers::EnumData<enum Role>(17, 17, QMC::EnumFlags{}).add({
            {   18, Role::ClipIdRole },
            {   19, Role::ClipTypeRole },
            {   20, Role::ClipTextRole },
            {   21, Role::PreviewRole },
            {   22, Role::SourceAppRole },
            {   23, Role::SourceBundleRole },
            {   24, Role::CreatedAtRole },
            {   25, Role::PinnedRole },
            {   26, Role::UseCountRole },
            {   27, Role::ImageWidthRole },
            {   28, Role::ImageHeightRole },
            {   29, Role::CharCountRole },
            {   30, Role::LineCountRole },
            {   31, Role::SectionNameRole },
        }),
    };
    QtMocHelpers::UintData qt_constructors {};
    QtMocHelpers::ClassInfos qt_classinfo({
            {    1,    2 },
            {    3,    4 },
            {    5,    6 },
    });
    return QtMocHelpers::metaObjectData<ClipModel, void>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_constructors, qt_classinfo);
}
Q_CONSTINIT const QMetaObject ClipModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractListModel::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9ClipModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9ClipModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN9ClipModelE_t>.metaTypes,
    nullptr
} };

void ClipModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ClipModel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->searchTextChanged(); break;
        case 1: _t->filterChanged(); break;
        case 2: _t->countChanged(); break;
        case 3: _t->reload(); break;
        case 4: { int _r = _t->indexOfId((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ClipModel::*)()>(_a, &ClipModel::searchTextChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ClipModel::*)()>(_a, &ClipModel::filterChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ClipModel::*)()>(_a, &ClipModel::countChanged, 2))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->searchText(); break;
        case 1: *reinterpret_cast<int*>(_v) = _t->filter(); break;
        case 2: *reinterpret_cast<int*>(_v) = _t->count(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setSearchText(*reinterpret_cast<QString*>(_v)); break;
        case 1: _t->setFilter(*reinterpret_cast<int*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *ClipModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ClipModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9ClipModelE_t>.strings))
        return static_cast<void*>(this);
    return QAbstractListModel::qt_metacast(_clname);
}

int ClipModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void ClipModel::searchTextChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ClipModel::filterChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ClipModel::countChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
