if( KDE4_BUILD_TESTS )

include_directories( ../ )

MACRO( MEDIAWIKI_UNIT_TESTS )

  FOREACH( _testname ${ARGN} )

    add_definitions( -DTEST_DATA="\"${CMAKE_CURRENT_SOURCE_DIR}\"" )
    kde4_add_unit_test( ${_testname} TESTNAME mediawiki-${_testname} NOGUI ${_testname}.cpp libmediawikitest/fakeserver.cpp )
    target_link_libraries( ${_testname} ${KDE4_KIO_LIBS} ${QT_QTTEST_LIBRARY} mediawiki -fprofile-arcs)
    set_property(TEST mediawiki-${_testname} PROPERTY LABELS library)
  ENDFOREACH( _testname )

ENDMACRO( MEDIAWIKI_UNIT_TESTS )

MEDIAWIKI_UNIT_TESTS(
    logintest
    logouttest
    queryimageinfotest
    queryimagestest
    querysiteinfousergroupstest
    querysiteinfogeneraltest
    mediawikitest
    queryrevisiontest
    queryinfotest
    edittest
    uploadtest
    parsetest
)
set( ressources_RC
    queryrevisiontest_content.rc
    queryrevisiontest_onetitle.rc
    queryrevisiontest.rc
    queryrevisiontest_cuted.rc
    queryrevisiontest_timestamponly.rc
    queryrevisiontest_useronly.rc
    queryrevisiontest_parsetree.rc
    queryrevisiontest_rollback.rc
    parsetest.rc
)
install(FILES ${ressources_RC} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


endif( KDE4_BUILD_TESTS )
