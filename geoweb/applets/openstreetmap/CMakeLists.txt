# Project Needs a name of course
project(plasma-geoweb)

# Find the required Libaries
find_package(KDE4 REQUIRED)
include(KDE4Defaults)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES})

# We add our source code here
set(geoweb-openstreetmap_SRCS plasma-geoweb-openstreetmap.cpp)

# Now make sure all files get to the right place
kde4_add_plugin(plasma_applet_geoweb-openstreetmap ${geoweb-openstreetmap_SRCS})
target_link_libraries(plasma_applet_geoweb-openstreetmap
                      ${KDE4_PLASMA_LIBS}
                      ${KDE4_KDEUI_LIBS})

install(TARGETS plasma_applet_geoweb-openstreetmap
        DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES plasma-applet-geoweb-openstreetmap.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
