#
#   generic CMake installation file for Selkie packages
#

# Don't forget to set the PACKAGE var, otherwise other
# selkie webapps might be overwritten!
# This is the only variable that needs to be set for a
# correct package and it has to be the same as the plugin
# name, otherwise resources, such as .js files won't be
# found
set( PACKAGE "gmail")


# All the rest should go automatically ...

# install the selkie Application file
set( APPNAME "silk-webapp-${PACKAGE}.desktop" )
message( STATUS "installing webapp.desktop to ${XDG_APPS_INSTALL_DIR}/silk/webapps/ RENAME ${APPNAME}")
install( FILES webapp.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} RENAME ${APPNAME} )

# install the selkie plugin file
set( PLUGINNAME "silk-webapp-${PACKAGE}.desktop")
message( STATUS "installing plugin.desktop to ${SERVICES_INSTALL_DIR}/silk/webapps/${PACKAGE}/ RENAME ${PLUGINNAME}")
install( FILES plugin.desktop DESTINATION ${SERVICES_INSTALL_DIR}/silk/webapps/${PACKAGE}/ RENAME ${PLUGINNAME})

# install all action .desktop files from actions/ subdirectory
file(GLOB files "actions/*.desktop")
foreach(file ${files})
    set( desktopfile "silk-webapp-${PACKAGE}-${file}" )
    MESSAGE( STATUS "installing file ${file} ${desktopfile}" )
    install( FILES ${file} DESTINATION ${SERVICES_INSTALL_DIR}/silk/webapps/${PACKAGE}/ )
endforeach()

# install all script .js files from scripts/ subdirectory
file(GLOB files "scripts/*.js")
foreach(file ${files})
    install( FILES ${file} DESTINATION ${DATA_INSTALL_DIR}/silk/webapps/${PACKAGE}/scripts/)
endforeach()
