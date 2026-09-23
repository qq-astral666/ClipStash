/****************************************************************************
** Meta object code from reading C++ file 'AppController.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/app/AppController.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AppController.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13AppControllerE_t {};
} // unnamed namespace

template <> constexpr inline auto AppController::qt_create_metaobjectdata<qt_meta_tag_ZN13AppControllerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "AppController",
        "QML.Element",
        "auto",
        "QML.Creatable",
        "false",
        "QML.UncreatableReason",
        "AppController is created in main.cpp",
        "showRequested",
        "",
        "hideRequested",
        "settingsRequested",
        "toast",
        "message",
        "systemNotification",
        "title",
        "windowVisibleChanged",
        "totalCountChanged",
        "accessibilityTrustedChanged",
        "pausedChanged",
        "settingsChanged",
        "launchAtLoginChanged",
        "showWindow",
        "hideWindow",
        "toggleWindow",
        "openSettings",
        "activate",
        "id",
        "paste",
        "togglePin",
        "remove",
        "clearHistory",
        "openUrl",
        "url",
        "targetScreenGeometry",
        "QRect",
        "prepareWindow",
        "QWindow*",
        "window",
        "QRectF",
        "card",
        "radius",
        "dark",
        "bringToFront",
        "requestAccessibility",
        "refreshAccessibility",
        "quit",
        "model",
        "ClipModel*",
        "isMac",
        "hotkeyOptions",
        "windowVisible",
        "totalCount",
        "accessibilityTrusted",
        "paused",
        "hotkeyPreset",
        "hotkeyText",
        "maxItems",
        "pasteOnSelect",
        "themeMode",
        "ignoreConcealed",
        "vibrancy",
        "launchAtLogin"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'showRequested'
        QtMocHelpers::SignalData<void()>(7, 8, QMC::AccessPublic, QMetaType::Void),
        // Signal 'hideRequested'
        QtMocHelpers::SignalData<void()>(9, 8, QMC::AccessPublic, QMetaType::Void),
        // Signal 'settingsRequested'
        QtMocHelpers::SignalData<void()>(10, 8, QMC::AccessPublic, QMetaType::Void),
        // Signal 'toast'
        QtMocHelpers::SignalData<void(const QString &)>(11, 8, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 12 },
        }}),
        // Signal 'systemNotification'
        QtMocHelpers::SignalData<void(const QString &, const QString &)>(13, 8, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 14 }, { QMetaType::QString, 12 },
        }}),
        // Signal 'windowVisibleChanged'
        QtMocHelpers::SignalData<void()>(15, 8, QMC::AccessPublic, QMetaType::Void),
        // Signal 'totalCountChanged'
        QtMocHelpers::SignalData<void()>(16, 8, QMC::AccessPublic, QMetaType::Void),
        // Signal 'accessibilityTrustedChanged'
        QtMocHelpers::SignalData<void()>(17, 8, QMC::AccessPublic, QMetaType::Void),
        // Signal 'pausedChanged'
        QtMocHelpers::SignalData<void()>(18, 8, QMC::AccessPublic, QMetaType::Void),
        // Signal 'settingsChanged'
        QtMocHelpers::SignalData<void()>(19, 8, QMC::AccessPublic, QMetaType::Void),
        // Signal 'launchAtLoginChanged'
        QtMocHelpers::SignalData<void()>(20, 8, QMC::AccessPublic, QMetaType::Void),
        // Method 'showWindow'
        QtMocHelpers::MethodData<void()>(21, 8, QMC::AccessPublic, QMetaType::Void),
        // Method 'hideWindow'
        QtMocHelpers::MethodData<void()>(22, 8, QMC::AccessPublic, QMetaType::Void),
        // Method 'toggleWindow'
        QtMocHelpers::MethodData<void()>(23, 8, QMC::AccessPublic, QMetaType::Void),
        // Method 'openSettings'
        QtMocHelpers::MethodData<void()>(24, 8, QMC::AccessPublic, QMetaType::Void),
        // Method 'activate'
        QtMocHelpers::MethodData<void(qint64, bool)>(25, 8, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::LongLong, 26 }, { QMetaType::Bool, 27 },
        }}),
        // Method 'togglePin'
        QtMocHelpers::MethodData<void(qint64)>(28, 8, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::LongLong, 26 },
        }}),
        // Method 'remove'
        QtMocHelpers::MethodData<void(qint64)>(29, 8, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::LongLong, 26 },
        }}),
        // Method 'clearHistory'
        QtMocHelpers::MethodData<void()>(30, 8, QMC::AccessPublic, QMetaType::Void),
        // Method 'openUrl'
        QtMocHelpers::MethodData<void(const QString &)>(31, 8, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 32 },
        }}),
        // Method 'targetScreenGeometry'
        QtMocHelpers::MethodData<QRect() const>(33, 8, QMC::AccessPublic, 0x80000000 | 34),
        // Method 'prepareWindow'
        QtMocHelpers::MethodData<void(QWindow *, const QRectF &, int, bool)>(35, 8, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 36, 37 }, { 0x80000000 | 38, 39 }, { QMetaType::Int, 40 }, { QMetaType::Bool, 41 },
        }}),
        // Method 'bringToFront'
        QtMocHelpers::MethodData<void(QWindow *)>(42, 8, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 36, 37 },
        }}),
        // Method 'requestAccessibility'
        QtMocHelpers::MethodData<void()>(43, 8, QMC::AccessPublic, QMetaType::Void),
        // Method 'refreshAccessibility'
        QtMocHelpers::MethodData<void()>(44, 8, QMC::AccessPublic, QMetaType::Void),
        // Method 'quit'
        QtMocHelpers::MethodData<void()>(45, 8, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'model'
        QtMocHelpers::PropertyData<ClipModel*>(46, 0x80000000 | 47, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'isMac'
        QtMocHelpers::PropertyData<bool>(48, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'hotkeyOptions'
        QtMocHelpers::PropertyData<QStringList>(49, QMetaType::QStringList, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'windowVisible'
        QtMocHelpers::PropertyData<bool>(50, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 5),
        // property 'totalCount'
        QtMocHelpers::PropertyData<int>(51, QMetaType::Int, QMC::DefaultPropertyFlags, 6),
        // property 'accessibilityTrusted'
        QtMocHelpers::PropertyData<bool>(52, QMetaType::Bool, QMC::DefaultPropertyFlags, 7),
        // property 'paused'
        QtMocHelpers::PropertyData<bool>(53, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 8),
        // property 'hotkeyPreset'
        QtMocHelpers::PropertyData<int>(54, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 9),
        // property 'hotkeyText'
        QtMocHelpers::PropertyData<QString>(55, QMetaType::QString, QMC::DefaultPropertyFlags, 9),
        // property 'maxItems'
        QtMocHelpers::PropertyData<int>(56, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 9),
        // property 'pasteOnSelect'
        QtMocHelpers::PropertyData<bool>(57, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 9),
        // property 'themeMode'
        QtMocHelpers::PropertyData<int>(58, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 9),
        // property 'ignoreConcealed'
        QtMocHelpers::PropertyData<bool>(59, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 9),
        // property 'vibrancy'
        QtMocHelpers::PropertyData<bool>(60, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 9),
        // property 'launchAtLogin'
        QtMocHelpers::PropertyData<bool>(61, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 10),
    };
    QtMocHelpers::UintData qt_enums {
    };
    QtMocHelpers::UintData qt_constructors {};
    QtMocHelpers::ClassInfos qt_classinfo({
            {    1,    2 },
            {    3,    4 },
            {    5,    6 },
    });
    return QtMocHelpers::metaObjectData<AppController, void>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums, qt_constructors, qt_classinfo);
}
Q_CONSTINIT const QMetaObject AppController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13AppControllerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13AppControllerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13AppControllerE_t>.metaTypes,
    nullptr
} };

void AppController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<AppController *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->showRequested(); break;
        case 1: _t->hideRequested(); break;
        case 2: _t->settingsRequested(); break;
        case 3: _t->toast((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->systemNotification((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 5: _t->windowVisibleChanged(); break;
        case 6: _t->totalCountChanged(); break;
        case 7: _t->accessibilityTrustedChanged(); break;
        case 8: _t->pausedChanged(); break;
        case 9: _t->settingsChanged(); break;
        case 10: _t->launchAtLoginChanged(); break;
        case 11: _t->showWindow(); break;
        case 12: _t->hideWindow(); break;
        case 13: _t->toggleWindow(); break;
        case 14: _t->openSettings(); break;
        case 15: _t->activate((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        case 16: _t->togglePin((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1]))); break;
        case 17: _t->remove((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1]))); break;
        case 18: _t->clearHistory(); break;
        case 19: _t->openUrl((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 20: { QRect _r = _t->targetScreenGeometry();
            if (_a[0]) *reinterpret_cast<QRect*>(_a[0]) = std::move(_r); }  break;
        case 21: _t->prepareWindow((*reinterpret_cast<std::add_pointer_t<QWindow*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QRectF>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[4]))); break;
        case 22: _t->bringToFront((*reinterpret_cast<std::add_pointer_t<QWindow*>>(_a[1]))); break;
        case 23: _t->requestAccessibility(); break;
        case 24: _t->refreshAccessibility(); break;
        case 25: _t->quit(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 21:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QWindow* >(); break;
            }
            break;
        case 22:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QWindow* >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::showRequested, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::hideRequested, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::settingsRequested, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)(const QString & )>(_a, &AppController::toast, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)(const QString & , const QString & )>(_a, &AppController::systemNotification, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::windowVisibleChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::totalCountChanged, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::accessibilityTrustedChanged, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::pausedChanged, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::settingsChanged, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::launchAtLoginChanged, 10))
            return;
    }
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< ClipModel* >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<ClipModel**>(_v) = _t->model(); break;
        case 1: *reinterpret_cast<bool*>(_v) = _t->isMac(); break;
        case 2: *reinterpret_cast<QStringList*>(_v) = _t->hotkeyOptions(); break;
        case 3: *reinterpret_cast<bool*>(_v) = _t->windowVisible(); break;
        case 4: *reinterpret_cast<int*>(_v) = _t->totalCount(); break;
        case 5: *reinterpret_cast<bool*>(_v) = _t->accessibilityTrusted(); break;
        case 6: *reinterpret_cast<bool*>(_v) = _t->paused(); break;
        case 7: *reinterpret_cast<int*>(_v) = _t->hotkeyPreset(); break;
        case 8: *reinterpret_cast<QString*>(_v) = _t->hotkeyText(); break;
        case 9: *reinterpret_cast<int*>(_v) = _t->maxItems(); break;
        case 10: *reinterpret_cast<bool*>(_v) = _t->pasteOnSelect(); break;
        case 11: *reinterpret_cast<int*>(_v) = _t->themeMode(); break;
        case 12: *reinterpret_cast<bool*>(_v) = _t->ignoreConcealed(); break;
        case 13: *reinterpret_cast<bool*>(_v) = _t->vibrancy(); break;
        case 14: *reinterpret_cast<bool*>(_v) = _t->launchAtLogin(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 3: _t->setWindowVisible(*reinterpret_cast<bool*>(_v)); break;
        case 6: _t->setPaused(*reinterpret_cast<bool*>(_v)); break;
        case 7: _t->setHotkeyPreset(*reinterpret_cast<int*>(_v)); break;
        case 9: _t->setMaxItems(*reinterpret_cast<int*>(_v)); break;
        case 10: _t->setPasteOnSelect(*reinterpret_cast<bool*>(_v)); break;
        case 11: _t->setThemeMode(*reinterpret_cast<int*>(_v)); break;
        case 12: _t->setIgnoreConcealed(*reinterpret_cast<bool*>(_v)); break;
        case 13: _t->setVibrancy(*reinterpret_cast<bool*>(_v)); break;
        case 14: _t->setLaunchAtLogin(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *AppController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AppController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13AppControllerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int AppController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 26)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 26)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void AppController::showRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void AppController::hideRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void AppController::settingsRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void AppController::toast(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void AppController::systemNotification(const QString & _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1, _t2);
}

// SIGNAL 5
void AppController::windowVisibleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void AppController::totalCountChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void AppController::accessibilityTrustedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void AppController::pausedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void AppController::settingsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void AppController::launchAtLoginChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}
QT_WARNING_POP
