find_package(Qt6 REQUIRED COMPONENTS Svg)

add_executable(dtk-svgc main.cpp)

target_compile_definitions(dtk-svgc PRIVATE QT_DEPRECATED_WARNINGS)
target_link_libraries(dtk-svgc PRIVATE dtk2widget Qt6::Svg)

install(TARGETS dtk-svgc RUNTIME DESTINATION ${DTK2WIDGET_TOOL_INSTALL_DIR})
