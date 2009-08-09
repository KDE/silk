
install(FILES silk-webapp.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
install(FILES silk-webapp-action.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})

add_subdirectory(gmail)
add_subdirectory(reviewboard)

