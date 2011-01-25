project( sampleMediawiki )

find_package( KDE4 REQUIRED )
include( KDE4Defaults )
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ${CMAKE_SOURCE_DIR}/mediawiki/libmediawiki )

set(samplemediawiki_SRCS
    main.cpp
    mainwindow.cpp
)
kde4_add_ui_files (ui_src mainwindow.ui)
KDE4_ADD_EXECUTABLE( sampleMediawiki ${samplemediawiki_SRCS} ${ui_src} )
target_link_libraries( sampleMediawiki ${KDE4_KIO_LIBS} mediawiki)
install( TARGETS sampleMediawiki ${INSTALL_TARGETS_DEFAULT_ARGS} )
