project(plasma-webslice)

find_package(KDE4 REQUIRED)
include (KDE4Defaults)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/.. )

#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/.. )

set(webslice_SRCS
    webslice.cpp
    ../slicegraphicswidget.cpp
)

kde4_add_ui_files(webslice_SRCS websliceConfig.ui )
kde4_add_plugin(plasma_applet_webslice ${webslice_SRCS})
target_link_libraries(plasma_applet_webslice
                      ${KDE4_PLASMA_LIBS}
                      ${KDE4_KIO_LIBS}
                      ${QT_QTWEBKIT_LIBRARY})

install(TARGETS plasma_applet_webslice DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-applet-webslice.desktop DESTINATION ${SERVICES_INSTALL_DIR})

