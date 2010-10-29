project(mediawiki)

find_package(KDE4 REQUIRED)

include(KDE4Defaults)
include_directories(${KDE4_INCLUDES} ${QT_INCLUDES})

add_subdirectory(tests)

set(mediawiki_SRCS
    mediawiki.cpp
    login.cpp
    general.cpp
    usergroups.cpp
)


kde4_add_library( mediawiki ${mediawiki_SRCS})
target_link_libraries( mediawiki ${KDE4_KIO_LIBS})
include_directories( ${CMAKE_SOURCE_DIR} )
install( TARGETS mediawiki ${INSTALL_TARGETS_DEFAULT_ARGS} )
