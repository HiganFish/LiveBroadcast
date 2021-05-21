cmake_minimum_required(VERSION 2.8.12)

project("LiveBroadcastServer")

set(CMAKE_CXX_FLAGS "-Wall -Werror -std=c++17 -g -o0")

include_directories(${CMAKE_SOURCE_DIR})

link_directories(/usr/lib64/mysql)

set(NETWORK_SRCS
		network/TcpServer.cpp
		network/Socket.cpp
		network/TcpConnection.cpp
		network/EventLoop.cpp
		network/Acceptor.cpp
		network/multiplexing/Epoll.cpp
		network/Channel.cpp
		network/InetAddress.cpp
		network/SocketOps.cpp
		network/Callback.h
		network/multiplexing/Epoll.cpp
		network/multiplexing/MultiplexingBase.cpp
		network/multiplexing/Select.cpp
		network/TcpClient.cpp
		network/Connector.cpp
		network/EventLoopThread.cpp
		network/EventLoopThreadPool.cpp
		)

set(CONN_SRCS
		network/connection/RtmpPushConnection.cpp
		network/connection/PullConnection.cpp
		network/connection/HttpPullConnection.cpp
		)

set(UTILS_SRCS
		utils/codec/FlvManager.cpp
		utils/codec/RtmpManager.cpp
		utils/codec/RtmpCodec.cpp
		utils/codec/FlvCodec.cpp
		utils/Buffer.cpp
		utils/File.cpp
		utils/Timestamp.cpp
		utils/Logger.cpp
		utils/LoggerStream.cpp
		utils/Format.cpp
		)

set(DB_MYSQL_SRC
		mysql/DbMysql.cpp
		mysql/QueryResult.cpp
		mysql/Field.cpp
		)

set(MAPPER_SRCS
		mapper/UserMapper.cpp
		)

set(THREAD_SRCS
		thread/Thread.cpp
		thread/ThreadPool.cpp
		thread/Mutex.cpp
		thread/Condition.cpp
		thread/CurrentThread.cpp
		)

set(BASE_SRCS
		${UTILS_SRCS}
		${THREAD_SRCS}
)

set(NETWORK_SRCS
		${NETWORK_SRCS}
		)

set(DB_SRCS
		${DB_MYSQL_SRC}
		${MAPPER_SRCS}
		)

add_library(lbs_base_lib ${BASE_SRCS})
target_link_libraries(lbs_base_lib pthread)

add_library(lbs_net_lib ${NETWORK_SRCS})
target_link_libraries(lbs_net_lib lbs_base_lib)

add_library(lbs_db_lib ${DB_SRCS})
target_link_libraries(lbs_db_lib mysqlclient)
target_link_libraries(lbs_db_lib lbs_base_lib)

add_library(lib_conn_lib ${CONN_SRCS})

add_executable(LiveBroadcastServer main.cpp)
target_link_libraries(LiveBroadcastServer lbs_net_lib lib_conn_lib)
# target_link_libraries(LiveBroadcastServer lbs_db_lib)


option(LBS_WITH_TEST "Compile LiveBroadcastServer Test" OFF)

if(LBS_WITH_TEST)
	add_subdirectory(test)
endif()