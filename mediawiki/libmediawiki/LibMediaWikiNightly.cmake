# From this line down, this script may be customized
# on the Clients tab of the CDash createProject page.
#
set(CTEST_BINARY_DIRECTORY "~/Workspace/project-silk/build/mediawiki/libmediawiki")
set(CTEST_SOURCE_DIRECTORY "~/Workspace/project-silk/src/mediawiki/libmediawiki")
set(OPTION_BUILD "-DKDE4_BUILD_TESTS=on")

set(CTEST_CMAKE_GENERATOR "Unix Makefiles")
include("${CTEST_SOURCE_DIRECTORY}/CTestConfig.cmake")

set(CTEST_CONFIGURE_COMMAND "${CMAKE_COMMAND} ${OPTION_BUILD}")

ctest_start(Experimental)
ctest_update(SOURCE ${CTEST_SOURCE_DIRECTORY})
ctest_configure(BUILD "${CTEST_BINARY_DIRECTORY}" RETURN_VALUE res)
ctest_build(BUILD "${CTEST_BINARY_DIRECTORY}" RETURN_VALUE res)
ctest_test(BUILD "${CTEST_BINARY_DIRECTORY}" RETURN_VALUE res)
#ctest_submit(RETURN_VALUE res)

message("DONE")

