
find_package(KDE4 REQUIRED)
include (KDE4Defaults)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

add_subdirectory(selkie)
add_subdirectory(remixer)
add_subdirectory(packages)
