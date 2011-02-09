project( editSample )

find_package( KDE4 REQUIRED )
include( KDE4Defaults )
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ${CMAKE_SOURCE_DIR}/mediawiki/libmediawiki )

kde4_add_ui_files (ui_src mainwindow.ui)
set(editSample_SRCS
    main.cpp
    mainwindow.cpp
)
KDE4_ADD_EXECUTABLE( editSample ${editSample_SRCS} ${ui_src} )
target_link_libraries( editSample ${KDE4_KIO_LIBS} mediawiki)
install( TARGETS editSample ${INSTALL_TARGETS_DEFAULT_ARGS} )
