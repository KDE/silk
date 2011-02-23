FIND_PACKAGE(KDE4 REQUIRED)
INCLUDE(KDE4Defaults)
INCLUDE(MacroLibrary)
include_directories( ${KDE4_INCLUDES} )

# debug settings
SET(AREA_CODE_GENERAL "51000")
SET(AREA_CODE_LOADING "51001")
ADD_DEFINITIONS(-DKDE_DEFAULT_DEBUG_AREA=${AREA_CODE_GENERAL})
ADD_DEFINITIONS(-DAREA_CODE_GENERAL=${AREA_CODE_GENERAL})
ADD_DEFINITIONS(-DAREA_CODE_LOADING=${AREA_CODE_LOADING})

# Depencies detection required by all plugins
FIND_PACKAGE(Kexiv2 REQUIRED)
FIND_PACKAGE(Kdcraw REQUIRED)
FIND_PACKAGE(Kipi REQUIRED)
FIND_PACKAGE(JPEG REQUIRED)
FIND_PACKAGE(PNG REQUIRED)
FIND_PACKAGE(TIFF REQUIRED)

subdirs(icons)


FIND_FILE(SVN_MARKER entries PATHS ${CMAKE_SOURCE_DIR}/.svn)

IF(NOT SVN_MARKER)
    SET (SVN_MARKER ${CMAKE_SOURCE_DIR}/CMakeLists.txt)  # Dummy file
ENDIF(NOT SVN_MARKER)

# Add a custom command to drive the svn script whenever the svn entries
# file changes.
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates/svnscript.cmake.in_cmake"
		"${CMAKE_CURRENT_BINARY_DIR}/svnscript.cmake"
		@ONLY)

ADD_CUSTOM_COMMAND(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/common/libkipiplugins/svnversion.h"
		    DEPENDS ${SVN_MARKER}
		    COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_BINARY_DIR}/svnscript.cmake")

# Add a custom target to drive the custom command.
ADD_CUSTOM_TARGET(kipiplugins-svnversion
		  ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/common/libkipiplugins/svnversion.h")

# ==================================================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/common/libkipiplugins
		    ${CMAKE_CURRENT_BINARY_DIR}/common/libkipiplugins
		    ${CMAKE_CURRENT_SOURCE_DIR}/common/libkipiplugins/dialogs
		    ${CMAKE_CURRENT_BINARY_DIR}/common/libkipiplugins/dialogs
		    ${CMAKE_CURRENT_SOURCE_DIR}/common/libkipiplugins/widgets
		    ${CMAKE_CURRENT_BINARY_DIR}/common/libkipiplugins/widgets
		    ${CMAKE_CURRENT_SOURCE_DIR}/common/libkipiplugins/tools
		    ${CMAKE_CURRENT_BINARY_DIR}/common/libkipiplugins/tools
		    ${KEXIV2_INCLUDE_DIR}
		    ${KDCRAW_INCLUDE_DIR}
		    ${KIPI_INCLUDE_DIR}
		    ${KDE4_INCLUDES}
		    ${QT4_INCLUDES}
		    )
subdirs(common)

kde4_add_ui_files (ui_src wmwindow.ui)
SET(kipiplugin_wikimedia_PART_SRCS
    plugin_wikimedia.cpp
    wmwindow.cpp
    wikimediajob.cpp
    wmwidget.cpp
    wmlogin.cpp
   )

KDE4_ADD_PLUGIN(kipiplugin_wikimedia ${kipiplugin_wikimedia_PART_SRCS} ${ui_src})

TARGET_LINK_LIBRARIES(kipiplugin_wikimedia
                      kipiplugins
                      mediawiki
                      ${KIPI_LIBRARIES}
                      ${KEXIV2_LIBRARIES}
                      ${KDCRAW_LIBRARIES}
                      ${KDE4_KIO_LIBS}
                      ${KDE4_KDEUI_LIBS}
                     )

INSTALL(TARGETS kipiplugin_wikimedia DESTINATION ${PLUGIN_INSTALL_DIR})
INSTALL(FILES kipiplugin_wikimedia.desktop DESTINATION ${SERVICES_INSTALL_DIR})
