install( FILES selkie-test.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES silk-webapp-test.desktop DESTINATION ${SERVICES_INSTALL_DIR})

install( FILES silk-webapp-test-toolbar.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES toolbaraction.js DESTINATION ${DATA_INSTALL_DIR}/silk-webapp/test/)

install( FILES silk-webapp-test-trigger.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES triggeraction.js DESTINATION ${DATA_INSTALL_DIR}/silk-webapp/test/)

install( FILES silk-webapp-test-glob.desktop DESTINATION ${SERVICES_INSTALL_DIR})


# Install the test sites files
install(FILES testsite/index.html DESTINATION ${DATA_INSTALL_DIR}/silk-webapp/test/testsite/)
install(FILES testsite/selkie.html DESTINATION ${DATA_INSTALL_DIR}/silk-webapp/test/testsite/)
install(FILES testsite/beach.html DESTINATION ${DATA_INSTALL_DIR}/silk-webapp/test/testsite/)
install(FILES testsite/default.css DESTINATION ${DATA_INSTALL_DIR}/silk-webapp/test/testsite/)
install(FILES testsite/randa-view.jpg DESTINATION ${DATA_INSTALL_DIR}/silk-webapp/test/testsite/)
install(FILES testsite/recife.jpg DESTINATION ${DATA_INSTALL_DIR}/silk-webapp/test/testsite/)
