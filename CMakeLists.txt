cmake_minimum_required(VERSION 3.2)
project(bg_api)

set(CMAKE_CXX_STANDARD 14)

#set(Boost_DEBUG 1)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

include_directories(
    ./include
)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
find_package(OpenSSL REQUIRED)

set(BG_API_HEADERS
    bg_api/api.hpp
    bg_api/flatjson.hpp
    bg_api/dtf.hpp
    bg_api/double_type.hpp
    bg_api/enums.hpp
    bg_api/errors.hpp
    bg_api/invoker.hpp
    bg_api/message.hpp
    bg_api/types.hpp
    bg_api/websocket.hpp
)

set(BG_API_SOURCES
    src/api.cpp
    src/enums.cpp
    src/errors.cpp
    src/types.cpp
    src/websocket.cpp
)

add_executable(
    ${PROJECT_NAME}
    #
    main.cpp
    #
    ${BG_API_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PUBLIC OpenSSL::SSL)
