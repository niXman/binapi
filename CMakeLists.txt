cmake_minimum_required(VERSION 3.22)
project(binapi)

set(CMAKE_CXX_STANDARD 20)

find_package(Threads REQUIRED)

add_library(binapi SHARED
        src/api.cpp
        src/dtf.cpp
        src/enums.cpp
        src/errors.cpp
        src/pairslist.cpp
        src/reports.cpp
        src/tools.cpp
        src/types.cpp
        src/websocket.cpp)

target_include_directories(binapi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options(binapi PUBLIC
        -pipe -Wall -Wextra
        $<$<CONFIG:DEBUG>:-g3 -Og -fno-inline -fstack-protector-all -fsanitize=address>
        $<$<CONFIG:RELEASE>:-O3 -s>)

target_link_options(binapi PUBLIC
        $<$<CONFIG:DEBUG>:-fsanitize=address>)

target_link_libraries(binapi PUBLIC z crypto ssl ${CMAKE_THREAD_LIBS_INIT} ${BOOST_LIBRARIES})

target_include_directories(binapi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(asynchronous examples/asynchronous/main.cpp)
target_link_libraries(asynchronous binapi)

add_executable(asynchronous-user_data examples/asynchronous-user_data/main.cpp)
target_link_libraries(asynchronous-user_data binapi)

add_executable(synchronous examples/synchronous/main.cpp)
target_link_libraries(synchronous binapi)

add_executable(synchronous-user_data examples/synchronous-user_data/main.cpp)
target_link_libraries(synchronous-user_data binapi)

add_executable(websockets examples/websockets/main.cpp)
target_link_libraries(websockets binapi)
