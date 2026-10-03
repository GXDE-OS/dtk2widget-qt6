find_package(Qt6 REQUIRED COMPONENTS Svg)

add_executable(containers
  src/main.cxx
  src/main_window/main_window.cxx
  src/main_window/main_window.hxx
  src/container_widget/container_widget.cxx
  src/container_widget/container_widget.hxx
  res/containers.qrc
)

target_link_libraries(containers PRIVATE dtk2widget Qt6::Svg)
