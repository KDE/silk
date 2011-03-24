FIND_PACKAGE(KDE4 REQUIRED)
INCLUDE(KDE4Defaults)
INCLUDE(MacroLibrary)
INCLUDE_DIRECTORIES( ${KDE4_INCLUDES} )

ADD_SUBDIRECTORY( libmediawiki )

# -----------------------------------------------------------------------
# Until libmediawiki is not moved to kdereview, use local version.
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
                      ${CMAKE_CURRENT_SOURCE_DIR}/libmediawiki/cmake/modules
   )
SET(MEDIAWIKI_LOCAL_DIR libmediawiki)
#
# -----------------------------------------------------------------------
ADD_SUBDIRECTORY( plasmoid )

