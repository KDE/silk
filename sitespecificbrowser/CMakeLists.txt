project(sitespecificbrowser)

find_package(KDE4 REQUIRED)
include (KDE4Defaults)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

set(sitespecificbrowser_SRCS
    main.cpp
    view.cpp
    page.cpp
)

KDE4_ADD_EXECUTABLE(sitespecificbrowser ${sitespecificbrowser_SRCS})

target_link_libraries(sitespecificbrowser
    ${QT_QTWEBKIT_LIBRARY}
    ${KDE4_KDEUI_LIBS}
)

install(FILES silk-webapp.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})

install( TARGETS sitespecificbrowser ${INSTALL_TARGETS_DEFAULT_ARGS} )
install( FILES sitespecificbrowser.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES
    sitespecificbrowser-gmail.desktop
    sitespecificbrowser-reviewboard.desktop
    DESTINATION ${SERVICES_INSTALL_DIR})

