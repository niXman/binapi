cmake_minimum_required(VERSION 3.2)
project(binapi)

set(CMAKE_CXX_STANDARD 20)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED SSL Crypto)
find_package(ZLIB REQUIRED)

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

target_link_libraries(binapi PUBLIC
        ZLIB::ZLIB
        OpenSSL::Crypto OpenSSL::SSL
        ${BOOST_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT})

target_include_directories(binapi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(example_asynchronous examples/asynchronous/main.cpp)
target_link_libraries(example_asynchronous binapi)

add_executable(example_asynchronous-user_data examples/asynchronous-user_data/main.cpp)
target_link_libraries(example_asynchronous-user_data binapi)

add_executable(example_synchronous examples/synchronous/main.cpp)
target_link_libraries(example_synchronous binapi)

add_executable(example_synchronous-user_data examples/synchronous-user_data/main.cpp)
target_link_libraries(example_synchronous-user_data binapi)

add_executable(example_websockets examples/websockets/main.cpp)
target_link_libraries(example_websockets binapi)

add_executable(example_api main.cpp)
target_link_libraries(example_api binapi)
