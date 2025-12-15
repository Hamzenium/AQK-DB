cmake_minimum_required(VERSION 3.20)
project(AQ_DB)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost CONFIG REQUIRED)

add_executable(AQ_DB
        src/main.cpp
        src/server/server.cpp
        src/server/connection.cpp
        src/command/CommandParser.cpp
        src/command/CommandDispatcher.cpp
        src/storage/KVStore.cpp
        src/storage/Shard.cpp
)

target_include_directories(AQ_DB PRIVATE
        src
        src/server
        src/command
        src/storage
)

target_compile_definitions(AQ_DB PRIVATE
        BOOST_ASIO_DISABLE_BOOST_SYSTEM
)

target_link_libraries(AQ_DB PRIVATE
        Boost::boost
)
