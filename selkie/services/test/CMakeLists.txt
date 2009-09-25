
# Install the webapp plugin and application launcher entries
install(FILES selkie-test.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES silk-webapp-test.desktop DESTINATION ${SERVICES_INSTALL_DIR})

# Install all the plugins and javascript files
install(FILES silk-webapp-test-toolbar.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES scalediv.js DESTINATION ${DATA_INSTALL_DIR}/silk-webapp/test/)

install(FILES silk-webapp-test-trigger.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES togglechrome.js DESTINATION ${DATA_INSTALL_DIR}/silk-webapp/test/)

install(FILES silk-webapp-test-wildcard.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES silk-webapp-test-editor.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES rotateelement.js DESTINATION ${DATA_INSTALL_DIR}/silk-webapp/test/)


# Install the test sites files
install(FILES testsite/mountain.html DESTINATION ${DATA_INSTALL_DIR}/silk-webapp/test/testsite/)
install(FILES testsite/selkie.html DESTINATION ${DATA_INSTALL_DIR}/silk-webapp/test/testsite/)
install(FILES testsite/beach.html DESTINATION ${DATA_INSTALL_DIR}/silk-webapp/test/testsite/)
install(FILES testsite/default.css DESTINATION ${DATA_INSTALL_DIR}/silk-webapp/test/testsite/)
install(FILES testsite/randa-view.jpg DESTINATION ${DATA_INSTALL_DIR}/silk-webapp/test/testsite/)
install(FILES testsite/recife.jpg DESTINATION ${DATA_INSTALL_DIR}/silk-webapp/test/testsite/)
