set(TESTS_NO_DB
        Connector
        EventLoopThread
        Logger
        Main
        TcpServer
        Thread
)

if(LBS_WITH_TEST)
    foreach(test ${TESTS_NO_DB})
        add_executable(${test} ${test}/${test}Test.cpp)
        target_link_libraries(${test} lbs_net_lib)
    endforeach()

    set(TESTS_DB
            DbMysql
            UserMapper
            )

    foreach(test ${TESTS_DB})
        add_executable(${test} ${test}/${test}Test.cpp)
        target_link_libraries(${test} lbs_db_lib)
    endforeach()
endif()

add_executable(Logger Logger/LoggerTest.cpp ../utils/Logger.cpp ../utils/Timestamp.cpp ../utils/LoggerStream.cpp)