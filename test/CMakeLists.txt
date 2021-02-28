set(TESTS_NO_DB
        Connector
        EventLoopThread
        Logger
        Main
        TcpServer
        Thread
        LoggerTest
)

set(TESTS_DB
        DbMysql
        UserMapper
        )

if(LBS_WITH_TEST)
    foreach(test ${TESTS_NO_DB})
        add_executable(${test} ${test}/${test}Test.cpp)
        target_link_libraries(${test} lbs_net_lib)
    endforeach()

    foreach(test ${TESTS_DB})
        add_executable(${test} ${test}/${test}Test.cpp)
        target_link_libraries(${test} lbs_db_lib)
    endforeach()
endif()