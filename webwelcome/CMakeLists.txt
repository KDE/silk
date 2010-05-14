project(plasma-webwelcome)

find_package(KDE4 REQUIRED)
include (KDE4Defaults)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

set(webwelcome_SRCS
    stylesheet.cpp
    applet.cpp
    dialog.cpp
    )

#kde4_add_ui_files(webwelcome_SRCS wwConfig.ui )
kde4_add_plugin(plasma_applet_webwelcome ${webwelcome_SRCS})

target_link_libraries(
    plasma_applet_webwelcome
    ${KDE4_PLASMA_LIBS}
    #${KDE4_KIO_LIBS}
    ${QT_QTWEBKIT_LIBRARY}
)

install(TARGETS plasma_applet_webwelcome DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-applet-webwelcome.desktop DESTINATION ${SERVICES_INSTALL_DIR})

#install(FILES user.css DESTINATION ${DATA_INSTALL_DIR}/plasma-applet-webwelcome/)


# install all .png files from images/ subdirectory
set( INSTALLPATH "${DATA_INSTALL_DIR}/silk/webwelcome/" )
file(GLOB files "images/*.png")
foreach(file ${files})
    install( FILES ${file} DESTINATION ${INSTALLPATH}/scripts/)
endforeach()
