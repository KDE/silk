project(plasma-potd)
set(potd_SRCS
    plasmapictureoftheday.cpp
    setting.cpp)

kde4_add_ui_files(potd_SRCS setting.ui )

kde4_add_plugin(plasma_applet_pictureoftheday ${potd_SRCS})
target_link_libraries(plasma_applet_pictureoftheday ${KDE4_PLASMA_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KFILE_LIBS} )

install(TARGETS plasma_applet_pictureoftheday DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-pictureoftheday-default.desktop DESTINATION ${SERVICES_INSTALL_DIR})
