cmake_minimum_required(VERSION 3.20)

project(dtk2widget
  VERSION 6.0.1
  DESCRIPTION "Qt6 port of DTK2Widgets"
  LANGUAGES C CXX
)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
  message(FATAL_ERROR "This package supports Linux ONLY.")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Debug is skipped.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

include(GNUInstallDirs)

option(BUILD_TOOLS "Build dtk-svgc" ON)
option(BUILD_EXAMPLES "Build the examples (not installed)" ON)

# We aligned with the QMake configure and decided to disable multimedia support.
option(DTK_NO_MULTIMEDIA "Disable DVideoWidget" ON)
option(DTK_NO_TRANSLATION "Skip building the .qm translation files" OFF)
option(DTK_STATIC_TRANSLATION "Embed translations into the library as a Qt resource" OFF)
option(DTK_STATIC_LIB "Build dtk2widget as a static library" OFF)

# We install dtk-svgc to another path so it does NOT affect the Qt5 build.
set(DTK2WIDGET_TOOL_INSTALL_DIR
  "${CMAKE_INSTALL_PREFIX}/lib/dtk2-qt6"
  CACHE PATH
  "Install dir of dtk-svgc"
)

set(DTK2WIDGET_INCLUDE_INSTALL_DIR
  "${CMAKE_INSTALL_INCLUDEDIR}/dtk2/DWidget"
  CACHE PATH
  "Install dir of headers"
)

set(DTK2WIDGET_TRANSLATIONS_INSTALL_DIR
  "${CMAKE_INSTALL_DATADIR}/dtk2widget/translations"
  CACHE PATH
  "Install dir of i18n translations."
)

find_package(Qt6 6.2 REQUIRED COMPONENTS
  Core Gui Widgets Network Concurrent DBus Multimedia MultimediaWidgets
  WaylandClient
)

find_package(Qt6 QUIET OPTIONAL_COMPONENTS StateMachine LinguistTools Svg)

# Configure Qt private for Qt 6.10+
if(Qt6_VERSION VERSION_GREATER_EQUAL 6.10)
  find_package(Qt6 REQUIRED COMPONENTS GuiPrivate WidgetsPrivate WaylandClientPrivate)
endif()
find_package(Dtk6Core REQUIRED)

add_subdirectory(src)

if(BUILD_TOOLS)
  add_subdirectory(tools/svgc)
endif()

if(BUILD_EXAMPLES)
  add_subdirectory(examples/dwidget-examples/collections)
  add_subdirectory(examples/dwidget-examples/containers)
endif()
