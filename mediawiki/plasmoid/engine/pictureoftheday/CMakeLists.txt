ADD_DEFINITIONS(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

INCLUDE_DIRECTORIES(${KDE4_INCLUDES}
                    ${MEDIAWIKI_INCLUDE_DIR}
                   )

KDE4_ADD_PLUGIN( plasma_engine_pictureoftheday pictureofthedayengine.cpp )

TARGET_LINK_LIBRARIES(plasma_engine_pictureoftheday ${KDE4_KDECORE_LIBS}
                                                    ${KDE4_PLASMA_LIBS}
                                                    ${KDE4_KIO_LIBS}
                                                    ${MEDIAWIKI_LIBRARIES}
                     )

INSTALL(TARGETS plasma_engine_pictureoftheday DESTINATION ${PLUGIN_INSTALL_DIR})

INSTALL(FILES plasma-engine-pictureoftheday.desktop DESTINATION ${SERVICES_INSTALL_DIR})
