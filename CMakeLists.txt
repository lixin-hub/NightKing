cmake_minimum_required(VERSION 3.5)

project(NightKing VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
set(UI
        page/pages/genarate/genaratepage.ui
        component/popwindow/adjustlightui.ui
        component/myprogressbar/myprogressbar.ui
        )
set(PROJECT_SOURCES
        main.cpp
        settingwindow/settingwindow.h
        settingwindow/settingwindow.cpp
        component/MonitorInfo/monitorinfo.h component/MonitorInfo/monitorinfo.cpp component/MonitorInfo/monitorinfo.ui
        component/DisplayOrder/displayorder.h component/DisplayOrder/displayorder.cpp component/DisplayOrder/displayorder.ui
        component/MarkDisplay/markdisplay.h component/MarkDisplay/markdisplay.cpp
        page/genarate/genaratepage.cpp component/MyTitleBar.cpp component/MyTitleBar.h component/BaseWindow.cpp component/BaseWindow.h
        genaratepage.h displaypage.h page/dispaly/displaypage.cpp

        res.qrc
        component/popwindow/AdjustLightWindow.h component/popwindow/AdjustLightWindow.cpp
        component/popwindow/adjustlightui.h component/popwindow/adjustlightui.cpp
        component/myprogressbar/myprogressbar.h component/myprogressbar/myprogressbar.cpp
        common/plans/Gamma/CGammaRamp.h common/plans/Gamma/CGammaRamp.cpp common/plans/Gamma/gdi32.dll lib/user32.dll lib/ole32.dll lib/wmiprov.dll
        common/monitor/mymonitors.h common/monitor/mymonitors.cpp
        common/plans/Mask/TransparentWindow.cpp
        common/plans/Mask/TransparentWindow.h
        common/plans/IOCTL/Ioctl.cpp common/plans/IOCTL/Ioctl.h common/plans/WMI/WMI.cpp common/plans/WMI/WMI.h
        common/Util/EDIDParser/EDIDParser.cpp common/Util/EDIDParser/EDIDParser.h component/DisplayOrder/DisplayOrder.cpp component/DisplayOrder/DisplayOrder.h common/Util/FileUitl/FileUtil.cpp common/Util/FileUitl/FileUtil.h common/Util/JsonUtil/Jsonutil.cpp common/Util/JsonUtil/JsonUtil.h
        common/plans/DDC_CI/DDC_CI.cpp common/plans/DDC_CI/DDC_CI.h common/monitor/MonitorController.cpp common/monitor/MonitorController.h common/plans/WMI/MyThread.cpp common/plans/WMI/MyThread.h common/plans/WMI/WMI2.cpp WindowService.cpp WindowService.h
        )
if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(NightKing
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            )
    # Define target properties for Android with Qt 6 as:
    #    set_property(TARGET NightKing APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else ()
    if (ANDROID)
        add_library(NightKing SHARED
                ${PROJECT_SOURCES}
                )
        # Define properties for Android with Qt 5 after find_package() calls as:
        #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else ()
        add_executable(NightKing
                ${PROJECT_SOURCES}
                )
    endif ()
endif ()

target_link_libraries(NightKing PRIVATE Qt${QT_VERSION_MAJOR}::Widgets dxva2.lib wbemuuid.lib Ole32.Lib)

set_target_properties(NightKing PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        #        WIN32_EXECUTABLE TRUE #加上这个属性qDebug不会显示
        )

install(TARGETS NightKing
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(NightKing)
endif ()
