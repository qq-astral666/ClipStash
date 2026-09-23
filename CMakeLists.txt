cmake_minimum_required(VERSION 3.21)

project(ClipStash
    VERSION 1.0.0
    DESCRIPTION "Keyboard-first clipboard manager for macOS"
    LANGUAGES CXX)

if(APPLE)
    enable_language(OBJCXX)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(CLIPSTASH_BUILD_TESTS "Build unit tests" ON)

find_package(Qt6 6.9 REQUIRED COMPONENTS Core Gui Widgets Sql Quick QuickControls2)
qt_standard_project_setup(REQUIRES 6.9)

# --------------------------------------------------------------------------
# Core library: storage + domain logic. No UI, no platform code -> testable.
# --------------------------------------------------------------------------
qt_add_library(clipstash_core STATIC
    src/core/ClipItem.h
    src/core/ClipItem.cpp
    src/core/ClipStore.h
    src/core/ClipStore.cpp
)
target_include_directories(clipstash_core PUBLIC src/core)
target_link_libraries(clipstash_core PUBLIC Qt6::Core Qt6::Gui Qt6::Sql)

# --------------------------------------------------------------------------
# Application
# --------------------------------------------------------------------------
qt_add_executable(ClipStash MACOSX_BUNDLE
    src/main.cpp
    src/app/AppController.h
    src/app/AppController.cpp
    src/app/ClipModel.h
    src/app/ClipModel.cpp
    src/app/ClipboardWatcher.h
    src/app/ClipboardWatcher.cpp
    src/app/ClipImageProvider.h
    src/app/ClipImageProvider.cpp
    src/app/TrayIcon.h
    src/app/TrayIcon.cpp
    src/platform/Platform.h
)
target_include_directories(ClipStash PRIVATE src/app src/platform)
target_compile_definitions(ClipStash PRIVATE PROJECT_VERSION_STRING="${PROJECT_VERSION}")

if(APPLE)
    target_sources(ClipStash PRIVATE src/platform/Platform_mac.mm)
    set_source_files_properties(src/platform/Platform_mac.mm PROPERTIES COMPILE_OPTIONS "-fobjc-arc")
    target_link_libraries(ClipStash PRIVATE
        "-framework AppKit"
        "-framework Carbon"
        "-framework ApplicationServices"
        "-framework ServiceManagement")
else()
    target_sources(ClipStash PRIVATE src/platform/Platform_stub.cpp)
endif()

set(CLIPSTASH_QML_FILES
    qml/Main.qml
    qml/Theme.qml
    qml/Icon.qml
    qml/KeyCap.qml
    qml/TypeBadge.qml
    qml/FilterBar.qml
    qml/ClipDelegate.qml
    qml/PreviewPane.qml
    qml/MetaRow.qml
    qml/EmptyState.qml
    qml/SettingsPage.qml
    qml/SettingRow.qml
    qml/SectionTitle.qml
    qml/Toggle.qml
    qml/Segmented.qml
    qml/PillButton.qml
)

# Flatten qml/ into the module root so every file sits next to the generated
# qmldir (singletons and sibling types resolve without extra imports).
foreach(qml_file IN LISTS CLIPSTASH_QML_FILES)
    get_filename_component(qml_name "${qml_file}" NAME)
    set_source_files_properties("${qml_file}" PROPERTIES QT_RESOURCE_ALIAS "${qml_name}")
endforeach()
set_source_files_properties(qml/Theme.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(ClipStash
    URI ClipStash
    VERSION 1.0
    QML_FILES ${CLIPSTASH_QML_FILES}
)

target_link_libraries(ClipStash PRIVATE
    clipstash_core
    Qt6::Widgets
    Qt6::Quick
    Qt6::QuickControls2
)

set_target_properties(ClipStash PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Info.plist.in"
    MACOSX_BUNDLE_GUI_IDENTIFIER "app.clipstash.ClipStash"
    MACOSX_BUNDLE_BUNDLE_NAME "ClipStash"
    MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
)

if(APPLE AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources/AppIcon.icns")
    set(app_icon "${CMAKE_CURRENT_SOURCE_DIR}/resources/AppIcon.icns")
    set_source_files_properties(${app_icon} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    target_sources(ClipStash PRIVATE ${app_icon})
    set_target_properties(ClipStash PROPERTIES MACOSX_BUNDLE_ICON_FILE "AppIcon.icns")
endif()

install(TARGETS ClipStash BUNDLE DESTINATION . RUNTIME DESTINATION bin)

# --------------------------------------------------------------------------
# Tests
# --------------------------------------------------------------------------
if(CLIPSTASH_BUILD_TESTS)
    enable_testing()
    find_package(Qt6 REQUIRED COMPONENTS Test)
    qt_add_executable(tst_clipstore tests/tst_clipstore.cpp)
    target_link_libraries(tst_clipstore PRIVATE clipstash_core Qt6::Test)
    add_test(NAME clipstore COMMAND tst_clipstore)
endif()
