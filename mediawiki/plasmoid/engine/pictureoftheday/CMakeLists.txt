find_package( KDE4 REQUIRED )
include( KDE4Defaults )

add_definitions( ${QT_DEFINITIONS} ${KDE4_DEFINITIONS} )

include_directories( ${KDE4_INCLUDES} ${CMAKE_SOURCE_DIR}/mediawiki/libmediawiki )

kde4_add_plugin( plasma_engine_pictureoftheday pictureofthedayengine.cpp )

target_link_libraries( plasma_engine_pictureoftheday ${KDE4_KDECORE_LIBS} ${KDE4_PLASMA_LIBS} mediawiki ${KDE4_KIO_LIBS} )

install( TARGETS plasma_engine_pictureoftheday DESTINATION ${PLUGIN_INSTALL_DIR} )

install( FILES plasma-engine-pictureoftheday.desktop DESTINATION ${SERVICES_INSTALL_DIR} )
