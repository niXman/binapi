cmake_minimum_required(VERSION 3.2)
project(bg_api)

set(CMAKE_CXX_STANDARD 14)

# Run genLUTs.py
find_package(PythonInterp 3.9 REQUIRED)
execute_process(
    COMMAND ${PYTHON_EXECUTABLE} genLUTs.py
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/include/bg_api
)

#set(Boost_DEBUG 1)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address /O2")

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
find_package(OpenSSL REQUIRED)

include_directories(./include)

set(BG_API_HEADERS
    include/bg_api/api.hpp
    include/bg_api/flatjson.hpp
    include/bg_api/dtf.hpp
    include/bg_api/double_type.hpp
    include/bg_api/enums.hpp
    include/bg_api/errors.hpp
    include/bg_api/invoker.hpp
    include/bg_api/message.hpp
    include/bg_api/types.hpp
    include/bg_api/websocket.hpp
    include/bg_api/LUTs.hpp
    include/bg_api/simdjson.h
)

set(BG_API_SOURCES
    src/api.cpp
    src/enums.cpp
    src/errors.cpp
    src/types.cpp
    src/websocket.cpp
    include/bg_api/LUTs.cpp
    include/bg_api/simdjson.cpp
)

add_executable(
    ${PROJECT_NAME}
    #
    main.cpp
    #
    ${BG_API_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PUBLIC OpenSSL::SSL)
