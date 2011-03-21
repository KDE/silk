PROJECT( mediawiki )

FIND_PACKAGE(KDE4 REQUIRED)
INCLUDE(KDE4Defaults)
INCLUDE(MacroLibrary)

INCLUDE_DIRECTORIES( ${KDE4_INCLUDES} )

SET( CMAKE_CXX_FLAGS "-fprofile-arcs -ftest-coverage" )
OPTION( STATIC_LIBRARY "Build libmediawiki as static libraries." FALSE )

IF( STATIC_LIBRARY )

    SET( LIBRARY_TYPE STATIC )
    ADD_DEFINITIONS( -DMEDIAWIKI_STATIC_LIBS )
    MESSAGE( STATUS "Building libmediawiki as static libraries" )

ELSE( STATIC_LIBRARY )

    SET( LIBRARY_TYPE SHARED )

ENDIF( STATIC_LIBRARY )

ADD_SUBDIRECTORY( tests )

SET(mediawiki_SRCS
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

SET(mediawiki_HRCS
    image.h
    imageinfo.h
    mediawiki.h
    login.h
    logout.h
    queryimageinfo.h
    queryimages.h
    querysiteinfogeneral.h
    generalinfo.h
    querysiteinfousergroups.h
    queryrevision.h
    queryinfo.h
    edit.h
    job.h
    usergroup.h
    revision.h
    page.h
    parse.h
    protection.h
    upload.h
    mediawiki_export.h
)

SET( SILK_MEDIAWIKI_LIBS mediawiki ${KDE4_KIO_LIBS} PARENT_SCOPE )

KDE4_ADD_LIBRARY( mediawiki ${LIBRARY_TYPE} ${mediawiki_SRCS} )
TARGET_LINK_LIBRARIES( mediawiki ${KDE4_KIO_LIBS} -fprofile-arcs )

INSTALL( TARGETS mediawiki ${INSTALL_TARGETS_DEFAULT_ARGS} )
INSTALL( FILES ${mediawiki_HRCS} DESTINATION ${INCLUDE_INSTALL_DIR}/libmediawiki COMPONENT Devel)

IF ( GENERATE_DOC )

    FIND_PATH(DOXYGEN_DIR doxygen /usr/local/bin /usr/bin)

    IF ( DOXYGEN_DIR )

        MESSAGE( STATUS " Doxygen generation of the documentation" )
        MESSAGE( STATUS ${CMAKE_CURRENT_SOURCE_DIR} )
        EXEC_PROGRAM("cd  ${CMAKE_CURRENT_SOURCE_DIR}; ${DOXYGEN_DIR}/doxygen")

    ELSE ( DOXYGEN_DIR )

        MESSAGE( STATUS "Doxygen not found, I cannot generate the documentation" )

    ENDIF ( DOXYGEN_DIR )

ENDIF ( GENERATE_DOC )
