set(webappeditor_SRCS
    selkieeditor.cpp
    dashboard.cpp
    webappeditor.cpp
    webappactioneditor.cpp
    package.cpp
    selkie-remixer.cpp
 )

kde4_add_ui_files(webappeditor_SRCS webapp.ui webappaction.ui)

kde4_add_kcfg_files(webappeditor_SRCS settings.kcfgc )

kde4_add_executable(selkie-remixer ${webappeditor_SRCS})

target_link_libraries(selkie-remixer
    ${QT_QTWEBKIT_LIBRARY}
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KIO_LIBS}
    ${KDE4_KTEXTEDITOR_LIBS}
    #${KDE4_PLASMA_LIBS}
)

install(TARGETS selkie-remixer ${INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( FILES webappeditor.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES webappeditor.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES webappeditorui.rc  DESTINATION  ${DATA_INSTALL_DIR}/webappeditor )


# selkiepkg
set(selkiepkg_SRCS
    selkiepkg.cpp
    package.cpp
)

kde4_add_executable(selkiepkg ${selkiepkg_SRCS})

target_link_libraries(selkiepkg ${KDE4_KDEUI_LIBS} ${KDE4_PLASMA_LIBS} ${KDE4_KIO_LIBS})

install(TARGETS selkiepkg ${INSTALL_TARGETS_DEFAULT_ARGS})