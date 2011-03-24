ADD_DEFINITIONS(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

KDE4_ADD_PLUGIN(plasma_engine_articleoftheday articleofthedayengine.cpp)

TARGET_LINK_LIBRARIES(plasma_engine_articleoftheday ${KDE4_KDECORE_LIBS}
                                                    ${KDE4_PLASMA_LIBS}
                                                    ${KDE4_KIO_LIBS}
                                                    ${MEDIAWIKI_LIBRARIES}
                     )

INSTALL(TARGETS plasma_engine_articleoftheday DESTINATION ${PLUGIN_INSTALL_DIR})

INSTALL(FILES plasma-engine-articleoftheday.desktop DESTINATION ${SERVICES_INSTALL_DIR})
