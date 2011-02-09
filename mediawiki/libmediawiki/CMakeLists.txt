project( mediawiki )

find_package( KDE4 REQUIRED )
set( CMAKE_CXX_FLAGS "-fprofile-arcs -ftest-coverage" )
include( KDE4Defaults )
include_directories( ${KDE4_INCLUDES} )

option( STATIC_LIBRARY "Build libmediawiki as static libraries." FALSE )

if( STATIC_LIBRARY )
    set( LIBRARY_TYPE STATIC )
    add_definitions( -DMEDIAWIKI_STATIC_LIBS )
    message( STATUS "Building libmediawiki as static libraries" )
else( STATIC_LIBRARY )
    set( LIBRARY_TYPE SHARED )
endif( STATIC_LIBRARY )

add_subdirectory( tests )

set(mediawiki_SRCS
    image.cpp
    imageinfo.cpp
    mediawiki.cpp
    login.cpp
    logout.cpp
    queryimageinfo.cpp
    queryimages.cpp
    querysiteinfogeneral.cpp
    generalinfo.cpp
    querysiteinfousergroups.cpp
    queryrevision.cpp
    queryinfo.cpp
    edit.cpp
    job.cpp
    usergroup.cpp
    revision.cpp
    page.cpp
    parse.cpp
    protection.cpp
    upload.cpp
)

kde4_add_library( mediawiki ${LIBRARY_TYPE} ${mediawiki_SRCS} )
target_link_libraries( mediawiki ${KDE4_KIO_LIBS} -fprofile-arcs )

install( TARGETS mediawiki ${INSTALL_TARGETS_DEFAULT_ARGS} )

set( SILK_MEDIAWIKI_LIBS mediawiki ${KDE4_KIO_LIBS} PARENT_SCOPE )

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
