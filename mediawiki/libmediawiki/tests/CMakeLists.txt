if( KDE4_BUILD_TESTS )
include_directories( ${CMAKE_SOURCE_DIR}/mediawiki/libmediawiki ${KDE4_INCLUDES} )

MACRO(MEDIAWIKI_UNIT_TESTS)

  FOREACH( _testname ${ARGN} )

    add_definitions( -DTEST_DATA="\"${CMAKE_CURRENT_SOURCE_DIR}\"" )
    kde4_add_unit_test( ${_testname} TESTNAME mediawiki-${_testname} NOGUI ${_testname}.cpp libmediawikitest/fakeserver.cpp )
    target_link_libraries( ${_testname} ${KDE4_KIO_LIBS} ${QT_QTTEST_LIBRARY} mediawiki -fprofile-arcs)

  ENDFOREACH( _testname )

ENDMACRO( MEDIAWIKI_UNIT_TESTS )

MEDIAWIKI_UNIT_TESTS(
    logintest
    logouttest
    queryimagestest
    querysiteinfousergroupstest
    querysiteinfogeneraltest
    mediawikitest
)


endif( KDE4_BUILD_TESTS )
