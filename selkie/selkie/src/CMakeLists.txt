
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

set(selkie_SRCS
    main.cpp
    view.cpp
    page.cpp
    webapp.cpp
    webappwidget.cpp
    webappaction.cpp
    scriptapi.cpp
    greasemonkeyscript.cpp
    ../../remixer/src/package.cpp
)

#kde4_add_ui_files(selkie_SRCS  prefs_base.ui)

#kde4_add_kcfg_files(selkie_SRCS settings.kcfgc )

KDE4_ADD_EXECUTABLE(selkie ${selkie_SRCS})

target_link_libraries(selkie
    ${QT_QTWEBKIT_LIBRARY}
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KIO_LIBS}
)

install( TARGETS selkie ${INSTALL_TARGETS_DEFAULT_ARGS} )
#install( FILES selkie.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
#install( FILES selkieui.rc  DESTINATION  ${DATA_INSTALL_DIR}/selkie )
install( FILES selkie.notifyrc DESTINATION ${DATA_INSTALL_DIR}/selkie )

