set(webappeditor_SRCS
   selkieeditor.cpp
   webappeditor.cpp
   webappactioneditor.cpp
   main.cpp
 )

kde4_add_ui_files(webappeditor_SRCS webapp.ui webappaction.ui)

kde4_add_kcfg_files(webappeditor_SRCS settings.kcfgc )

kde4_add_executable(webappeditor ${webappeditor_SRCS})

target_link_libraries(webappeditor
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KIO_LIBS}
    ${KDE4_KTEXTEDITOR_LIBS})

install(TARGETS webappeditor ${INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( FILES webappeditor.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES webappeditor.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES webappeditorui.rc  DESTINATION  ${DATA_INSTALL_DIR}/webappeditor )
