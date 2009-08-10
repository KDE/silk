install( FILES sitespecificbrowser-silk.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES silk-webapp-silk.desktop DESTINATION ${SERVICES_INSTALL_DIR})

install( FILES silk-webapp-silk-commitlog.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install( FILES silk-webapp-silk-sourcetree.desktop DESTINATION ${SERVICES_INSTALL_DIR})

install( FILES silk-webapp-silk-sidebar.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES togglesidebar.js DESTINATION ${DATA_INSTALL_DIR}/silk-webapp/silk/)
