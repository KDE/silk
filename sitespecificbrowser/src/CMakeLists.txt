
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

set(sitespecificbrowser_SRCS
    main.cpp
    view.cpp
    page.cpp
    webapp.cpp
    webappaction.cpp
    scriptapi.cpp
)

kde4_add_ui_files(sitespecificbrowser_SRCS  prefs_base.ui)

kde4_add_kcfg_files(sitespecificbrowser_SRCS settings.kcfgc )

KDE4_ADD_EXECUTABLE(sitespecificbrowser ${sitespecificbrowser_SRCS})

target_link_libraries(sitespecificbrowser
    ${QT_QTWEBKIT_LIBRARY}
    ${KDE4_KDEUI_LIBS}
)

install( TARGETS sitespecificbrowser ${INSTALL_TARGETS_DEFAULT_ARGS} )
install( FILES sitespecificbrowser.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES sitespecificbrowserui.rc  DESTINATION  ${DATA_INSTALL_DIR}/sitespecificbrowser )

