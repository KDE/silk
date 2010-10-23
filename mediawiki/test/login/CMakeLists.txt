project(test_login)
find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include_directories(${KDE4_INCLUDES} ${QT_INCLUDES} ${QT_QTCORE_INCLUDE_DIR} ${QT_QTTEST_INCLUDE_DIR})

set(test_login_SRCS
    ../fake_server/server.cpp
    ../../mediawiki.cpp
    ../../login.cpp
    test_login.cpp
)

kde4_add_executable(test_login ${test_login_SRCS} ${test_login_HDRS})
target_link_libraries( test_login ${QT_QTCORE_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QT_QTTEST_LIBRARIES})
