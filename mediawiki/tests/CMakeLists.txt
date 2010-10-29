if(KDE4_BUILD_TESTS)
include_directories( ${CMAKE_SOURCE_DIR}/mediawiki ${KDE4_INCLUDES} ${QT_INCLUDES})

MACRO(MEDIAWIKI_UNIT_TESTS)

  FOREACH(_testname ${ARGN})

    add_definitions(-DTEST_DATA="\"${CMAKE_CURRENT_SOURCE_DIR}\"")
    kde4_add_unit_test(${_testname} TESTNAME mediawiki-${_testname} NOGUI ${_testname}.cpp fake_server/server.cpp)
    target_link_libraries(${_testname} ${KDE4_KIO_LIBS} ${QT_QTTEST_LIBRARY}
                          mediawiki )
    include_directories( ${CMAKE_SOURCE_DIR} )

  ENDFOREACH(_testname)

ENDMACRO(MEDIAWIKI_UNIT_TESTS)

MEDIAWIKI_UNIT_TESTS(
    test_general
    test_login
    test_usergroups
)
endif(KDE4_BUILD_TESTS)





#MACRO(MEDIAWIKI_EXECUTABLE_TESTS)
#  FOREACH(_testname ${ARGN})
#    kde4_add_executable(${_testname} NOGUI TEST ${_testname}.cxx)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}")
#    target_link_libraries(${_testname} ${KDE4_KIO_LIBS} ${KDE4_KIO_LIBS} mediawiki)
#    include_directories( ${CMAKE_SOURCE_DIR} )
#  ENDFOREACH(_testname)
#ENDMACRO(MEDIAWIKI_EXECUTABLE_TESTS)

########### automated tests ###############

###########   manual tests  ###############

#KIMAP_EXECUTABLE_TESTS(
#  testimapidle
#  testimapserver
#)
