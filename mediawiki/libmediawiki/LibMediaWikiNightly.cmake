set(CTEST_SOURCE_DIRECTORY "$ENV{HOME}/Workspace/project-silk/src/mediawiki/libmediawiki/")
set(CTEST_BINARY_DIRECTORY "$ENV{HOME}/Workspace/project-silk/build/mediawiki/libmediawiki/")

include(${CTEST_SOURCE_DIRECTORY}/CTestConfig.cmake)

set(WITH_MEMCHECK TRUE)
set(WITH_COVERAGE TRUE)

#######################################################################

find_program(CTEST_GIT_COMMAND NAMES git)
find_program(CTEST_COVERAGE_COMMAND NAMES gcov)
find_program(CTEST_MEMORYCHECK_COMMAND NAMES valgrind)

set(CTEST_MEMORYCHECK_SUPPRESSIONS_FILE ${CTEST_BINARY_DIRECTORY}/tests/valgrind.supp)

if(NOT EXISTS "${CTEST_SOURCE_DIRECTORY}")
  set(CTEST_CHECKOUT_COMMAND "${CTEST_GIT_COMMAND} clone git@gitorious.org:~yoms/project-silk/isi-project-silk.git ${CTEST_SOURCE_DIRECTORY}")
endif()

set(CTEST_UPDATE_COMMAND "${CTEST_GIT_COMMAND}")

set(CTEST_CONFIGURE_COMMAND "${CMAKE_COMMAND} -DWITH_TESTING:BOOL=ON ${CTEST_BUILD_OPTIONS}")
set(CTEST_CONFIGURE_COMMAND "${CTEST_CONFIGURE_COMMAND} \"-G${CTEST_CMAKE_GENERATOR}\"")
set(CTEST_CONFIGURE_COMMAND "${CTEST_CONFIGURE_COMMAND} \"${CTEST_SOURCE_DIRECTORY}\"")

ctest_start("Experimental")
ctest_update()
ctest_configure()
ctest_build()
ctest_test()
if (WITH_MEMCHECK AND CTEST_COVERAGE_COMMAND)
  ctest_coverage()
endif (WITH_MEMCHECK AND CTEST_COVERAGE_COMMAND)
if (WITH_MEMCHECK AND CTEST_MEMORYCHECK_COMMAND)
  ctest_memcheck()
endif (WITH_MEMCHECK AND CTEST_MEMORYCHECK_COMMAND)
#ctest_submit()
