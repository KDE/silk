project(sitespecificbrowser)

find_package(KDE4 REQUIRED)
include (KDE4Defaults)

add_subdirectory(src)

add_subdirectory(services)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

set ( testgreasemonkeyscript_SRCS
    tests/test_greasemonkeyscript.cpp
    src/greasemonkeyscript.cpp )

kde4_add_executable( testgreasemonkeyscript TEST ${testgreasemonkeyscript_SRCS})
target_link_libraries( testgreasemonkeyscript ${KDE4_KDEUI_LIBS} )
