install( FILES selkie-test.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES silk-webapp-test.desktop DESTINATION ${SERVICES_INSTALL_DIR})

install( FILES silk-webapp-test-toolbar.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES toolbaraction.js DESTINATION ${DATA_INSTALL_DIR}/silk-webapp/test/)

install( FILES silk-webapp-test-trigger.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES triggeraction.js DESTINATION ${DATA_INSTALL_DIR}/silk-webapp/test/)
