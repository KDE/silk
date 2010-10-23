project( mediawiki )
find_package( KDE4 REQUIRED )
include( KDE4Defaults )
include_directories( ${KDE4_INCLUDES} )

set( mediawiki_SRCS
    mediawiki.cpp
)

kde4_add_library( mediawiki ${mediawiki_SRCS} )
target_link_libraries( mediawiki ${KDE4_KCORE_LIBS} )
