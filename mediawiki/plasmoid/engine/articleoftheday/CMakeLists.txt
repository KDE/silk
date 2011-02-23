add_definitions( ${QT_DEFINITIONS} ${KDE4_DEFINITIONS} )

include_directories( ${KDE4_INCLUDES} ${CMAKE_SOURCE_DIR}/mediawiki/libmediawiki )

kde4_add_plugin( plasma_engine_articleoftheday articleofthedayengine.cpp )

target_link_libraries( plasma_engine_articleoftheday ${KDE4_KDECORE_LIBS} ${KDE4_PLASMA_LIBS} ${SILK_MEDIAWIKI_LIBS} )

install( TARGETS plasma_engine_articleoftheday DESTINATION ${PLUGIN_INSTALL_DIR} )

install( FILES plasma-engine-articleoftheday.desktop DESTINATION ${SERVICES_INSTALL_DIR} )
