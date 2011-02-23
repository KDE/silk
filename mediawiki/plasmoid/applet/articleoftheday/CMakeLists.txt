project(plasma-potd)
set(potd_SRCS
    plasmaarticleoftheday.cpp
    setting.cpp)

kde4_add_ui_files(potd_SRCS setting.ui )

kde4_add_plugin(plasma_applet_articleoftheday ${potd_SRCS})
target_link_libraries(plasma_applet_articleoftheday ${KDE4_PLASMA_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KFILE_LIBS} )

install(TARGETS plasma_applet_articleoftheday DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-articleoftheday-default.desktop DESTINATION ${SERVICES_INSTALL_DIR})
