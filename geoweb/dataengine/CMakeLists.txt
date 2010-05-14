# A name for the project
project(plasma-geoweb)

# Find the required Libaries
find_package(KDE4 REQUIRED)
include(KDE4Defaults)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES})

# We add our source code here
set(geoweb_SRCS geoweb.cpp geoloc.cpp provider.cpp)

# Now make sure all files get to the right place
kde4_add_kcfg_files(geoweb_SRCS settings.kcfgc)
kde4_add_plugin(plasma_engine_geoweb ${geoweb_SRCS})

target_link_libraries(plasma_engine_geoweb
                      ${KDE4_KDECORE_LIBS}
                      ${KDE4_PLASMA_LIBS}
                      ${KDE4_KROSSCORE_LIBS})

install(TARGETS plasma_engine_geoweb
        DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES plasma-engine-geoweb.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})

install(FILES geoweb.kcfg
        DESTINATION ${KCFG_INSTALL_DIR})

if(ENABLE_HELLO_PLUGIN)
    install(DIRECTORY providerPlugins/helloPlugin/
            DESTINATION ${KDE4_DATA_INSTALL_DIR}/plasma_engine_geoweb/providerPlugins/helloPlugin
            PATTERN *~ EXCLUDE)
endif(ENABLE_HELLO_PLUGIN)

if(ENABLE_OPENSTREETMAP_PLUGIN)
    install(DIRECTORY providerPlugins/openstreetmap/
            DESTINATION ${KDE4_DATA_INSTALL_DIR}/plasma_engine_geoweb/providerPlugins/openstreetmap
            PATTERN *~ EXCLUDE)
endif(ENABLE_OPENSTREETMAP_PLUGIN)