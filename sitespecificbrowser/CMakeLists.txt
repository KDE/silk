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
)

#install(TARGETS sitespecificbrowser DESTINATION ${SERVICES_INSTALL_DIR})
#install(FILES plasma-applet-lionmail.desktop DESTINATION ${SERVICES_INSTALL_DIR})
#install(FILES desktoptheme/akonadi.svg DESTINATION ${DATA_INSTALL_DIR}/desktoptheme/default/widgets)
