project( mediawiki )

find_package( KDE4 REQUIRED )
set( CMAKE_CXX_FLAGS "-fprofile-arcs -ftest-coverage" )
include( KDE4Defaults )
include_directories( ${KDE4_INCLUDES} )

add_subdirectory( tests )

set(mediawiki_SRCS
    mediawiki.cpp
    login.cpp
    logout.cpp
    general.cpp
    querysiteinfousergroups.cpp
)

kde4_add_library( mediawiki ${mediawiki_SRCS} )
target_link_libraries( mediawiki ${KDE4_KIO_LIBS} -fprofile-arcs )

install( TARGETS mediawiki ${INSTALL_TARGETS_DEFAULT_ARGS} )

FIND_PATH(DOXYGEN_DIR doxygen
/usr/local/bin
/usr/bin
)
IF ( DOXYGEN_DIR )
MESSAGE( STATUS " Doxygen generation of the documentation" )
MESSAGE( STATUS ${CMAKE_CURRENT_SOURCE_DIR} )
EXEC_PROGRAM(
"cd  ${CMAKE_CURRENT_SOURCE_DIR}; ${DOXYGEN_DIR}/doxygen"
)
ELSE ( DOXYGEN_DIR )
MESSAGE( STATUS "Doxygen not found, I cannot generate the documentation" )
ENDIF ( DOXYGEN_DIR )
