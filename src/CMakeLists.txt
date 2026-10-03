find_package(PkgConfig REQUIRED)

pkg_check_modules(DTK2WIDGET_DEPS REQUIRED IMPORTED_TARGET
  gsettings-qt6
  librsvg-2.0
  xext
  x11
  xi
  xcb
  xcb-util
  libstartup-notification-1.0
  wayland-client
)

pkg_check_modules(GTK2 QUIET IMPORTED_TARGET gtk+-2.0)
find_package(LayerShellQt REQUIRED)

set(UTIL_HEADERS
  util/dfileiconprovider.h
  util/dthumbnailprovider.h
  util/dwindowmanagerhelper.h
  util/dwidgetutil.h
  util/ddesktopservices.h
  util/dtrashmanager.h
  util/dsvgrenderer.h
  util/dhidpihelper.h
  util/dregionmonitor.h
)

set(UTIL_PRIVATE_HEADERS
  util/private/dregionmonitor_p.h
  util/private/xeventmonitor_p.h
)

set(UTIL_SOURCES
  util/dfileiconprovider.cpp
  util/dthumbnailprovider.cpp
  util/dwindowmanagerhelper.cpp
  util/dwidgetutil.cpp
  util/dhidpihelper.cpp
  util/ddesktopservices_linux.cpp
  util/dtrashmanager_linux.cpp
  util/dsvgrenderer.cpp
  util/dregionmonitor.cpp
)

set(UTIL_CLASS_HEADERS
  util/DWidgetUtil
  util/DDesktopServices
  util/DTrashManager
  util/DThumbnailProvider
  util/DFileIconProvider
  util/DWindowManagerHelper
  util/DHiDPIHelper
  util/DSvgRenderer
)

set(PRIVATE_HEADERS
  widgets/private/keyboardmonitor/dkeyboardmonitor.h
  widgets/private/startupnotifications/startupnotificationmonitor.h
  widgets/private/mpris/dmprismonitor.h
  widgets/private/mpris/dbusinterface.h
  widgets/private/dmpriscontrol_p.h
  widgets/private/mpris/dbusmpris.h
  widgets/private/dthemehelper.h
  widgets/private/dcircleprogress_p.h
  widgets/private/dlineedit_p.h
  widgets/private/dfilechooseredit_p.h
  widgets/private/dstackwidget_p.h
  widgets/private/dscrollarea_p.h
  widgets/private/dlistwidget_p.h
  widgets/private/dboxwidget_p.h
  widgets/private/dpasswordedit_p.h
  widgets/private/dabstractdialogprivate_p.h
  widgets/private/ddialog_p.h
  widgets/private/dloadingindicator_p.h
  widgets/private/dinputdialog_p.h
  widgets/private/dipv4lineedit_p.h
  widgets/private/dspinbox_p.h
  widgets/private/doptionlist_p.h
  widgets/private/doption_p.h
  widgets/private/dpicturesequenceview_p.h
  widgets/private/dflowlayout_p.h
  widgets/private/dlistview_p.h
  widgets/private/dtextedit_p.h
  widgets/private/dapplication_p.h
  widgets/private/dscrollbar_p.h
  widgets/private/dmainwindow_p.h
  widgets/private/dblureffectwidget_p.h
  widgets/private/dpageindicator_p.h
  widgets/private/daboutdialog_p.h
  widgets/private/darrowrectangle_p.h
  widgets/private/dpushbutton_p.h
  widgets/private/dtickeffect_p.h
  widgets/private/dswitchbutton_p.h
  widgets/private/dimagebutton_p.h
  widgets/private/dpasswdeditanimated_p.h
  widgets/private/dborderlesswidget_p.h
  widgets/private/dkwinblur.h
  widgets/private/dmenueffect.h
  widgets/private/ddeshellmanager.h
  widgets/private/dmenuwayland.h
  widgets/private/dxsettings.h
  widgets/private/settings/buttongroup.h
  widgets/private/settings/combobox.h
  widgets/private/settings/content.h
  widgets/private/settings/contenttitle.h
  widgets/private/settings/navigation.h
  widgets/private/settings/navigationdelegate.h
  widgets/private/settings/shortcutedit.h
)

set(PRIVATE_SOURCES
  widgets/private/keyboardmonitor/dkeyboardmonitor.cpp
  widgets/private/startupnotifications/startupnotificationmonitor.cpp
  widgets/private/mpris/dmprismonitor.cpp
  widgets/private/mpris/dbusinterface.cpp
  widgets/private/mpris/dbusmpris.cpp
  widgets/private/dthemehelper.cpp
  widgets/private/dkwinblur.cpp
  widgets/private/dmenueffect.cpp
  widgets/private/ddeshellmanager.cpp
  widgets/private/dmenuwayland.cpp
  widgets/private/dxsettings.cpp
  widgets/private/settings/buttongroup.cpp
  widgets/private/settings/combobox.cpp
  widgets/private/settings/content.cpp
  widgets/private/settings/contenttitle.cpp
  widgets/private/settings/navigation.cpp
  widgets/private/settings/navigationdelegate.cpp
  widgets/private/settings/shortcutedit.cpp
)

set(WAYLAND_PROTOCOL_SOURCES
  widgets/wayland-blur/blur-client-protocol.c
  widgets/wayland-blur/dde-shell-client-protocol.c
  widgets/wayland-blur/treeland-dde-shell-protocol.c
)

set(PLATFORM_SOURCES
  platforms/x11/xutil.h
  platforms/x11/xutil.cpp
)

set(WIDGETS_HEADERS
  widgets/dabstractdialog.h
  widgets/ddialog.h
  widgets/dialog_constants.h
  widgets/dinputdialog.h
  widgets/daboutdialog.h
  widgets/dsettingsdialog.h
  widgets/dfiledialog.h
  widgets/dmpriscontrol.h
  widgets/dpasswdeditanimated.h
  widgets/dslider.h
  widgets/dapplicationhelper.h
  widgets/dapplicationsettings.h
  widgets/dcheckbox.h
  widgets/dguiapplicationhelper.h
  widgets/dmainwindowbackground.h
  widgets/dmessagebox.h
  widgets/dpalette.h
  widgets/dinputdialog_p.h
  widgets/dplatformtheme.h
  widgets/dthememanager.h
  widgets/dapplication.h
  widgets/dconstants.h
  widgets/dseparatorhorizontal.h
  widgets/dseparatorvertical.h
  widgets/dframe.h
  widgets/dcardwidget.h
  widgets/dfloatingwidget.h
  widgets/dfloatingmessage.h
  widgets/dbackgroundgroup.h
  widgets/dbaseline.h
  widgets/dheaderline.h
  widgets/dbaseexpand.h
  widgets/darrowbutton.h
  widgets/darrowlineexpand.h
  widgets/dswitchlineexpand.h
  widgets/dbuttonlist.h
  widgets/dimagebutton.h
  widgets/dtextbutton.h
  widgets/dhoverbutton.h
  widgets/dicontextbutton.h
  widgets/dloadingindicator.h
  widgets/dsearchedit.h
  widgets/dswitchbutton.h
  widgets/dsegmentedcontrol.h
  widgets/dlineedit.h
  widgets/dwidget.h
  widgets/dwindowmaxbutton.h
  widgets/dwindowminbutton.h
  widgets/dwindowclosebutton.h
  widgets/dwindowoptionbutton.h
  widgets/dshortcutedit.h
  widgets/dsimplelistview.h
  widgets/dsimplelistitem.h
  widgets/dexpandgroup.h
  widgets/dlabel.h
  widgets/dtypographylabel.h
  widgets/dlinkbutton.h
  widgets/dscrollbar.h
  widgets/dscrollarea.h
  widgets/darrowrectangle.h
  widgets/dgraphicsgloweffect.h
  widgets/dbasebutton.h
  widgets/dlistwidget.h
  widgets/dboxwidget.h
  widgets/dcircleprogress.h
  widgets/dstackwidget.h
  widgets/dfilechooseredit.h
  widgets/dpasswordedit.h
  widgets/dipv4lineedit.h
  widgets/dspinbox.h
  widgets/doptionlist.h
  widgets/doption.h
  widgets/dpicturesequenceview.h
  widgets/dflowlayout.h
  widgets/dlistview.h
  widgets/denhancedwidget.h
  widgets/dtextedit.h
  widgets/dtitlebar.h
  widgets/dplatformwindowhandle.h
  widgets/dmainwindow.h
  widgets/dblureffectwidget.h
  widgets/dblursurface.h
  widgets/dpageindicator.h
  widgets/dclipeffectwidget.h
  widgets/dgraphicsclipeffect.h
  widgets/dforeignwindow.h
  widgets/dpushbutton.h
  widgets/dtickeffect.h
  widgets/dwaterprogress.h
  widgets/dsettingswidgetfactory.h
  widgets/dspinner.h
  widgets/dcrumbedit.h
  widgets/dtabbar.h
  widgets/dtabbedstackwidget.h
  widgets/ddocumenttabbar.h
  widgets/dsuggestbutton.h
  widgets/dstyleoption.h
  widgets/dtoast.h
  widgets/danchors.h
  widgets/dwindowgroupleader.h
  widgets/dborderlesswidget.h
  widgets/dcommandlinkbutton.h
  widgets/dborderlesswindow.h
  widgets/dprogressbar.h
)

set(WIDGETS_SOURCES
  widgets/dabstractdialog.cpp
  widgets/dinputdialog.cpp
  widgets/daboutdialog.cpp
  widgets/dsettingsdialog.cpp
  widgets/dfiledialog.cpp
  widgets/dmpriscontrol.cpp
  widgets/dpasswdeditanimated.cpp
  widgets/dtkwidget_init.cpp
  widgets/dslider.cpp
  widgets/dapplicationhelper.cpp
  widgets/dapplicationsettings.cpp
  widgets/dcheckbox.cpp
  widgets/ddialog.cpp
  widgets/dguiapplicationhelper.cpp
  widgets/dmainwindowbackground.cpp
  widgets/dmessagebox.cpp
  widgets/dpalette.cpp
  widgets/dplatformtheme.cpp
  widgets/dthememanager.cpp
  widgets/dapplication.cpp
  widgets/dseparatorhorizontal.cpp
  widgets/dseparatorvertical.cpp
  widgets/dframe.cpp
  widgets/dcardwidget.cpp
  widgets/dfloatingwidget.cpp
  widgets/dfloatingmessage.cpp
  widgets/dbackgroundgroup.cpp
  widgets/dbaseline.cpp
  widgets/dheaderline.cpp
  widgets/dbaseexpand.cpp
  widgets/darrowbutton.cpp
  widgets/darrowlineexpand.cpp
  widgets/dswitchlineexpand.cpp
  widgets/dbuttonlist.cpp
  widgets/dimagebutton.cpp
  widgets/dtextbutton.cpp
  widgets/dhoverbutton.cpp
  widgets/dicontextbutton.cpp
  widgets/dloadingindicator.cpp
  widgets/dsearchedit.cpp
  widgets/dswitchbutton.cpp
  widgets/dsegmentedcontrol.cpp
  widgets/dlineedit.cpp
  widgets/dwindowmaxbutton.cpp
  widgets/dwindowminbutton.cpp
  widgets/dwindowclosebutton.cpp
  widgets/dwindowoptionbutton.cpp
  widgets/dshortcutedit.cpp
  widgets/dsimplelistview.cpp
  widgets/dsimplelistitem.cpp
  widgets/dexpandgroup.cpp
  widgets/dlabel.cpp
  widgets/dtypographylabel.cpp
  widgets/dlinkbutton.cpp
  widgets/dscrollbar.cpp
  widgets/dscrollarea.cpp
  widgets/darrowrectangle.cpp
  widgets/dgraphicsgloweffect.cpp
  widgets/dbasebutton.cpp
  widgets/dlistwidget.cpp
  widgets/dboxwidget.cpp
  widgets/dcircleprogress.cpp
  widgets/dstackwidget.cpp
  widgets/dfilechooseredit.cpp
  widgets/dpasswordedit.cpp
  widgets/dipv4lineedit.cpp
  widgets/dspinbox.cpp
  widgets/doptionlist.cpp
  widgets/doption.cpp
  widgets/dpicturesequenceview.cpp
  widgets/dflowlayout.cpp
  widgets/dlistview.cpp
  widgets/denhancedwidget.cpp
  widgets/dtextedit.cpp
  widgets/dtitlebar.cpp
  widgets/dplatformwindowhandle.cpp
  widgets/dmainwindow.cpp
  widgets/dblureffectwidget.cpp
  widgets/dblursurface.cpp
  widgets/dpageindicator.cpp
  widgets/dclipeffectwidget.cpp
  widgets/dgraphicsclipeffect.cpp
  widgets/dforeignwindow.cpp
  widgets/dtickeffect.cpp
  widgets/dwaterprogress.cpp
  widgets/dsettingswidgetfactory.cpp
  widgets/dspinner.cpp
  widgets/dcrumbedit.cpp
  widgets/dtabbar.cpp
  widgets/dtabbedstackwidget.cpp
  widgets/ddocumenttabbar.cpp
  widgets/dsuggestbutton.cpp
  widgets/dstyleoption.cpp
  widgets/dtoast.cpp
  widgets/danchors.cpp
  widgets/dwindowgroupleader.cpp
  widgets/dborderlesswidget.cpp
  widgets/dcommandlinkbutton.cpp
  widgets/dborderlesswindow.cpp
)

set(WIDGETS_CLASS_HEADERS
  widgets/DPasswdEditAnimated
  widgets/DListWidget
  widgets/DWidget
  widgets/DFontSizeManager
  widgets/DPushButton
  widgets/DGuiApplicationHelper
  widgets/DApplicationHelper
  widgets/DPalette
  widgets/DListView
  widgets/DCheckBox
  widgets/DDialog
  widgets/DImageButton
  widgets/DLabel
  widgets/DAction
  widgets/DAnchorsBase
  widgets/DBorderlessWindow
  widgets/DCommandLinkButton
  widgets/DMenu
  widgets/DTitlebar
  widgets/DWindow
  widgets/DMainWindow
  widgets/DMainWindowBackground
  widgets/DAboutDialog
  widgets/DApplication
  widgets/DBlurEffectWidget
  widgets/DBlurSurface
  widgets/DClipEffectWidget
  widgets/DGraphicsDropShadowEffect
  widgets/DPlatformWindowHandle
  widgets/DGraphicsClipEffect
  widgets/DForeignWindow
  widgets/DExpandGroup
  widgets/DArrowButton
  widgets/DArrowLineExpand
  widgets/DThemeManager
  widgets/DWaterProgress
  widgets/DSimpleListView
  widgets/DSimpleListItem
  widgets/DSearchEdit
  widgets/DPageIndicator
  widgets/DSettingsWidgetFactory
  widgets/DSettingsDialog
  widgets/DSpinner
  widgets/DCrumbEdit
  widgets/DTabBar
  widgets/DTabbedStackWidget
  widgets/DDocumentTabBar
  widgets/DSuggestButton
  widgets/DStyleOption
  widgets/DToast
  widgets/DFileDialog
  widgets/DLineEdit
  widgets/DStyleOptionLineEdit
  widgets/DWindowGroupLeader
  widgets/DAnchors
  widgets/DSegmentedControl
  widgets/DSegmentedHighlight
  widgets/DMessageBox
  widgets/DToolButton
  widgets/DIconButton
  widgets/DProgressBar
  widgets/DFrame
  widgets/DCardWidget
  widgets/DFloatingWidget
  widgets/DFloatingMessage
  widgets/DBackgroundGroup
  widgets/DTypographyLabel
  widgets/DSectionTitle
  widgets/DHoverButton
  widgets/DIconTextButton
  widgets/DApplicationSettings
)

if(NOT DTK_NO_MULTIMEDIA)
  list(APPEND WIDGETS_HEADERS widgets/dvideowidget.h)
  list(APPEND WIDGETS_SOURCES widgets/dvideowidget.cpp)
  list(APPEND WIDGETS_CLASS_HEADERS widgets/DVideoWidget)
endif()

set(RESOURCES
  widgets/themes/dui_theme_dark.qrc
  widgets/themes/dui_theme_light.qrc
  widgets/icons.qrc
)

set(GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")

set(_module_headers dtkwidget_global.h ${UTIL_HEADERS} ${WIDGETS_HEADERS})
list(REMOVE_DUPLICATES _module_headers)
set(_content "#ifndef DTK_WIDGETS_MODULE_H\n#define DTK_WIDGETS_MODULE_H\n")

foreach(_header IN LISTS _module_headers)
  get_filename_component(_name "${_header}" NAME)
  string(APPEND _content "#include \"${_name}\"\n")
endforeach()

string(APPEND _content "#endif\n")
file(CONFIGURE OUTPUT "${GENERATED_DIR}/DtkWidgets" CONTENT "${_content}")

set(_content "")

foreach(_header IN LISTS UTIL_CLASS_HEADERS WIDGETS_CLASS_HEADERS)
  get_filename_component(_name "${_header}" NAME)
  string(APPEND _content "#define DTKWIDGET_CLASS_${_name}\n")
endforeach()

file(CONFIGURE OUTPUT "${GENERATED_DIR}/dtkwidget_config.h" CONTENT "${_content}")

set_source_files_properties(
  "${CMAKE_CURRENT_BINARY_DIR}/dtk2widget_autogen/mocs_compilation.cpp"
  PROPERTIES COMPILE_OPTIONS "-include;${CMAKE_CURRENT_SOURCE_DIR}/widgets/dplatformwindowhandle.h"
)

set(QM_FILES "")

if(NOT DTK_NO_TRANSLATION)
  if(NOT TARGET Qt6::lrelease)
    message(FATAL_ERROR "lrelease not found, install qt6-l10n-tools or pass -DDTK_NO_TRANSLATION=ON")
  endif()

  file(GLOB TS_FILES "${PROJECT_SOURCE_DIR}/translations/*.ts")

  foreach(_ts IN LISTS TS_FILES)
    get_filename_component(_name "${_ts}" NAME_WE)
    set(_qm "${CMAKE_CURRENT_BINARY_DIR}/translations/${_name}.qm")

    add_custom_command(
      OUTPUT "${_qm}"
      COMMAND Qt6::lrelease -silent "${_ts}" -qm "${_qm}"
      DEPENDS "${_ts}"
      VERBATIM
    )

    list(APPEND QM_FILES "${_qm}")
  endforeach()
endif()

if(DTK_STATIC_LIB)
  set(_lib_type STATIC)
else()
  set(_lib_type SHARED)
endif()

add_library(dtk2widget ${_lib_type}
  dtkwidget_global.h
  ${UTIL_HEADERS} ${UTIL_PRIVATE_HEADERS} ${UTIL_SOURCES}
  ${PRIVATE_HEADERS} ${PRIVATE_SOURCES}
  ${WIDGETS_HEADERS} ${WIDGETS_SOURCES}
  ${PLATFORM_SOURCES}
  ${WAYLAND_PROTOCOL_SOURCES}
  ${RESOURCES}
  ${QM_FILES}
)

add_library(Dtk2widget::Dtk2widget ALIAS dtk2widget)

set_target_properties(dtk2widget PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
  EXPORT_NAME Dtk2widget
)

if(DTK_STATIC_TRANSLATION AND QM_FILES)
  qt_add_resources(dtk2widget dtk2widget_translations
    PREFIX "/dtk/translations"
    BASE "${CMAKE_CURRENT_BINARY_DIR}/translations"
    FILES ${QM_FILES}
  )
endif()

target_compile_definitions(dtk2widget
  PUBLIC
    QT_MESSAGELOGCONTEXT
  PRIVATE
    LIBDTK2WIDGET_LIBRARY
    SN_API_NOT_YET_FROZEN
    QT_NO_KEYWORDS
)

if(DTK_NO_MULTIMEDIA)
  target_compile_definitions(dtk2widget PRIVATE DTK_NO_MULTIMEDIA)
endif()

if(DTK_STATIC_LIB)
  target_compile_definitions(dtk2widget PUBLIC DTK_STATIC_LIB)
endif()

if(DTK_STATIC_TRANSLATION)
  target_compile_definitions(dtk2widget PRIVATE DTK_STATIC_TRANSLATION)
endif()

if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(sw_64|mips64|mips32)")
  target_compile_definitions(dtk2widget PRIVATE FORCE_RASTER_WIDGETS)
endif()

if(GTK2_FOUND)
  target_compile_definitions(dtk2widget PRIVATE USE_GTK_PLUS_2_0)
  target_include_directories(dtk2widget PRIVATE ${GTK2_INCLUDE_DIRS})
endif()

target_include_directories(dtk2widget
  PUBLIC
    "$<BUILD_INTERFACE:${GENERATED_DIR}>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/util>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/widgets>"
    "$<INSTALL_INTERFACE:${DTK2WIDGET_INCLUDE_INSTALL_DIR}>"
  PRIVATE
    widgets/private/keyboardmonitor
    widgets/private/startupnotifications
    widgets/wayland-blur
)

target_link_libraries(dtk2widget
  PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Concurrent
    Qt6::DBus
    Dtk6::Core
  PRIVATE
    Qt6::GuiPrivate
    Qt6::WidgetsPrivate
    Qt6::WaylandClientPrivate
    Qt6::Multimedia
    Qt6::MultimediaWidgets
    LayerShellQt::Interface
    PkgConfig::DTK2WIDGET_DEPS
)

if(TARGET Qt6::StateMachine)
  target_link_libraries(dtk2widget PRIVATE Qt6::StateMachine)
endif()

install(TARGETS dtk2widget
  EXPORT Dtk2widgetTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${DTK2WIDGET_INCLUDE_INSTALL_DIR}
)

file(GLOB _public_headers
  "${CMAKE_CURRENT_SOURCE_DIR}/util/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/widgets/*.h"
)

set(_class_headers "")

foreach(_header IN LISTS UTIL_CLASS_HEADERS WIDGETS_CLASS_HEADERS)
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${_header}")
    list(APPEND _class_headers "${_header}")
  endif()
endforeach()

install(FILES
  dtkwidget_global.h
  "${GENERATED_DIR}/DtkWidgets"
  "${GENERATED_DIR}/dtkwidget_config.h"
  ${_public_headers}
  ${_class_headers}
  DESTINATION ${DTK2WIDGET_INCLUDE_INSTALL_DIR}
)

if(QM_FILES)
  install(FILES ${QM_FILES} DESTINATION ${DTK2WIDGET_TRANSLATIONS_INSTALL_DIR})
endif()

include(CMakePackageConfigHelpers)
set(CMAKE_CONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/Dtk2widget")
set(TOOL_INSTALL_DIR "${DTK2WIDGET_TOOL_INSTALL_DIR}")

install(EXPORT Dtk2widgetTargets
  NAMESPACE Dtk2widget::
  DESTINATION ${CMAKE_CONFIG_INSTALL_DIR}
)

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/misc/Dtk2widgetConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/Dtk2widgetConfig.cmake"
  INSTALL_DESTINATION ${CMAKE_CONFIG_INSTALL_DIR}
  PATH_VARS DTK2WIDGET_INCLUDE_INSTALL_DIR TOOL_INSTALL_DIR
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/Dtk2widgetConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/Dtk2widgetConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/Dtk2widgetConfigVersion.cmake"
  DESTINATION ${CMAKE_CONFIG_INSTALL_DIR}
)

cmake_path(APPEND PC_LIBDIR "\${prefix}" "${CMAKE_INSTALL_LIBDIR}")
cmake_path(APPEND PC_INCLUDEDIR "\${prefix}" "${DTK2WIDGET_INCLUDE_INSTALL_DIR}")

if(IS_ABSOLUTE "${CMAKE_INSTALL_LIBDIR}")
  set(PC_LIBDIR "${CMAKE_INSTALL_LIBDIR}")
endif()

if(IS_ABSOLUTE "${DTK2WIDGET_INCLUDE_INSTALL_DIR}")
  set(PC_INCLUDEDIR "${DTK2WIDGET_INCLUDE_INSTALL_DIR}")
endif()

configure_file("${PROJECT_SOURCE_DIR}/misc/dtk2widget.pc.in"
  "${CMAKE_CURRENT_BINARY_DIR}/dtk2widget.pc" @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/dtk2widget.pc"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

if(NOT DEFINED MKSPECS_INSTALL_DIR)
  set(MKSPECS_INSTALL_DIR "${QT6_INSTALL_ARCHDATA}/mkspecs")
endif()

cmake_path(ABSOLUTE_PATH CMAKE_INSTALL_LIBDIR BASE_DIRECTORY "${CMAKE_INSTALL_PREFIX}"
  OUTPUT_VARIABLE PRI_LIBDIR)

cmake_path(ABSOLUTE_PATH DTK2WIDGET_INCLUDE_INSTALL_DIR BASE_DIRECTORY "${CMAKE_INSTALL_PREFIX}"
  OUTPUT_VARIABLE PRI_INCLUDEDIR)

set(PRI_TOOLDIR "${PRI_LIBDIR}/libdtk-${PROJECT_VERSION}/D2widget/bin")
set(PRI_DEPENDS "core gui dtkcore network concurrent multimedia multimediawidgets")

if(TARGET Qt6::StateMachine)
  string(APPEND PRI_DEPENDS " statemachine")
endif()

string(APPEND PRI_DEPENDS " widgets widgets_private gui_private dbus waylandclient_private")

configure_file("${PROJECT_SOURCE_DIR}/misc/qt_lib_dtk2widget.pri.in"
  "${CMAKE_CURRENT_BINARY_DIR}/qt_lib_dtk2widget.pri" @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/qt_lib_dtk2widget.pri"
  DESTINATION ${MKSPECS_INSTALL_DIR}/modules)
