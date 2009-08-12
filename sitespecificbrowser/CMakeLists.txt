project(sitespecificbrowser)

find_package(KDE4 REQUIRED)
include (KDE4Defaults)

add_subdirectory(src)
add_subdirectory(services)
