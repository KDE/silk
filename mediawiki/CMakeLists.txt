
find_package(KDE4 REQUIRED)

if(TEST)
add_subdirectory(test)
endif(TEST)

project(mediawiki)
include(KDE4Defaults)
include_directories(${KDE4_INCLUDES} ${QT_INCLUDES})

set( mediawiki_SRCS
    mediawiki.cpp
    login.cpp
    general.cpp
    usergroups.cpp
)


kde4_add_library( mediawiki ${mediawiki_SRCS})
target_link_libraries( mediawiki ${KDE4_KIO_LIBS})
include_directories( ${CMAKE_SOURCE_DIR} )
install( TARGETS mediawiki ${INSTALL_TARGETS_DEFAULT_ARGS} )
