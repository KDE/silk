find_package(KDE4 4.3.0 REQUIRED)
include(KDE4Defaults)

cmake_minimum_required(VERSION 2.6)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})

add_definitions("-DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII")

set(librest_srcs
  librest.cpp
)

kde4_add_library(rest SHARED ${librest_srcs})
target_link_libraries(rest ${KDE4_KIO_LIBS})
set_target_properties(rest PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )

install(TARGETS rest EXPORT LibRestLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS})

install( FILES 
        librest.h
        DESTINATION ${INCLUDE_INSTALL_DIR}/librest COMPONENT Devel)

