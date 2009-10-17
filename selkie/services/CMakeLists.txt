
install(FILES silk-webapp.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
install(FILES silk-webapp-action.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})

add_subdirectory(test)
add_subdirectory(gmail)
add_subdirectory(reviewboard)
add_subdirectory(silk)
add_subdirectory(buienradar)
add_subdirectory(formula1timing)
add_subdirectory(planetkde)
