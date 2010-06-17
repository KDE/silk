#
#   generic CMake installation file for WebService plugins
#

# Don't forget to set the PACKAGE var, otherwise other
# the webservice plugins might get overwritten!
# This is the only variable that needs to be set for a
# correct package and it has to be the same as the plugin
# name, otherwise resources, such as .png files won't be
# found
set(PACKAGE "twitter")


# All the rest should go automatically ...
# if you need to change something below this line, tell sebas@kde.org what you
# had to change


# The webservice plugin stuff
install(FILES webservice.desktop DESTINATION ${SERVICES_INSTALL_DIR} RENAME webservice_${PACKAGE}.desktop)
install(FILES logo.png DESTINATION ${DATA_INSTALL_DIR}/silk/webservices RENAME logo_${PACKAGE}.png)

install(FILES icons/twitter.png DESTINATION ${DATA_INSTALL_DIR}/plasma-applet-webwelcome/)

# our KControl Module

set(silk_webservice_${PACKAGE}_PART_SRCS
    kcm.cpp)

kde4_add_ui_files(silk_webservice_${PACKAGE}_PART_SRCS config.ui)
kde4_add_plugin(silk_webservice_${PACKAGE} ${silk_webservice_${PACKAGE}_PART_SRCS})

target_link_libraries(silk_webservice_${PACKAGE}  ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${QT_QTGUI_LIBRARY})

install(TARGETS silk_webservice_${PACKAGE}  DESTINATION ${PLUGIN_INSTALL_DIR} )
install( FILES kcm.desktop  DESTINATION ${SERVICES_INSTALL_DIR} RENAME silk_webservice_${PACKAGE}.desktop)
