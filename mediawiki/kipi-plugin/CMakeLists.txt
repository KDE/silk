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

SET(kipiplugin_wikimedia_PART_SRCS
    plugin_wikimedia.cpp
    wmwindow.cpp
    wikimediajob.cpp
   )

KDE4_ADD_PLUGIN(kipiplugin_wikimedia ${kipiplugin_wikimedia_PART_SRCS})

TARGET_LINK_LIBRARIES(kipiplugin_wikimedia
                      ${KIPI_LIBRARIES}
                      ${KEXIV2_LIBRARIES}
                      ${KDCRAW_LIBRARIES}
                      ${KDE4_KIO_LIBS}
                      ${KDE4_KDEUI_LIBS}
                     )

INSTALL(TARGETS kipiplugin_wikimedia DESTINATION ${PLUGIN_INSTALL_DIR})
INSTALL(FILES kipiplugin_wikimedia.desktop DESTINATION ${SERVICES_INSTALL_DIR})
