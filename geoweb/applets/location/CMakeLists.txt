# Project Needs a name of course
project(plasma-geoweb)

# Find the required Libaries
find_package(KDE4 REQUIRED)
include(KDE4Defaults)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   ${CMAKE_CURRENT_BINARY_DIR}/../../dataengine/)

set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/../../dataengine/settings.cpp PROPERTIES GENERATED 1)

# We add our source code here
set(geoweb-location_SRCS plasma-geoweb-location.cpp geocodehandler.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/../../dataengine/settings.cpp)

# Now make sure all files get to the right place
kde4_add_ui_files(geoweb-location_SRCS locationConf.ui)
kde4_add_plugin(plasma_applet_geoweb-location ${geoweb-location_SRCS})

target_link_libraries(plasma_applet_geoweb-location
                      ${KDE4_PLASMA_LIBS}
                      ${KDE4_KDEUI_LIBS}
                      ${QT_QTNETWORK_LIBRARY}
                      ${QT_QTXML_LIBRARY})

install(TARGETS plasma_applet_geoweb-location
        DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES plasma-applet-geoweb-location.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
