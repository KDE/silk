
# Install application launcher
install( FILES selkie-planetkde.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )

# Install the Selkie plugin
install( FILES silk-webapp-planetkde.desktop DESTINATION ${SERVICES_INSTALL_DIR})

# Install planetkde actions
install( FILES silk-webapp-planetkde-microblog.desktop DESTINATION ${SERVICES_INSTALL_DIR})

# Our javascript and css foo
install(FILES pko-functions.js DESTINATION ${DATA_INSTALL_DIR}/silk-webapp/planetkde/)
install(FILES pko.css DESTINATION ${DATA_INSTALL_DIR}/silk-webapp/planetkde/)

