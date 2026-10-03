find_package(Qt6 REQUIRED COMPONENTS Svg)

set(SOURCES
  main.cpp
  mainwindow.cpp
  buttontab.cpp
  linetab.cpp
  bartab.cpp
  inputtab.cpp
  slidertab.cpp
  buttonlisttab.cpp
  segmentedcontrol.cpp
  indicatortab.cpp
  widgetstab.cpp
  simplelistviewtab.cpp
  singlelistview.cpp
  singlelistitem.cpp
  multilistview.cpp
  multilistitem.cpp
  graphicseffecttab.cpp
  images.qrc
  resources.qrc
)

if(NOT DTK_NO_MULTIMEDIA)
  list(APPEND SOURCES cameraform.cpp cameraform.ui)
endif()

add_executable(collections ${SOURCES})
target_include_directories(collections PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(collections PRIVATE
  dtk2widget
  Qt6::Svg
  Qt6::Multimedia
  Qt6::MultimediaWidgets
)

if(DTK_NO_MULTIMEDIA)
  target_compile_definitions(collections PRIVATE DTK_NO_MULTIMEDIA)
endif()
