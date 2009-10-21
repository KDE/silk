
find_package(KDE4 REQUIRED)
include (KDE4Defaults)

include_directories( ${QT_INCLUDES} )



set(webslicer2_SRCS
    main.cpp
    slicegraphicswidget.cpp
    slicewidget.cpp
)

KDE4_ADD_EXECUTABLE(webslicer2 ${webslicer2_SRCS})

target_link_libraries(webslicer2
    ${QT_QTWEBKIT_LIBRARY}
)

add_subdirectory(plasma)
