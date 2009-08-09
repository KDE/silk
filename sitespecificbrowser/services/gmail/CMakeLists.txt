
# Install application launcher
install( FILES sitespecificbrowser-gmail.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )

# Install GMail plugin
install( FILES silk-webapp-gmail.desktop DESTINATION ${SERVICES_INSTALL_DIR})

# Install gmail actions
install( FILES silk-webapp-gmail-compose.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install( FILES silk-webapp-gmail-notify.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install( FILES silk-webapp-gmail-hello.desktop DESTINATION ${SERVICES_INSTALL_DIR})
