cmake_minimum_required(VERSION 3.2)
project(binapi)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fsanitize=address")

add_definitions(
    -UNDEBUG
    -DDTF_HEADER_ONLY
    -D__FLATJSON__CHILDS_TYPE=std::uint32_t
    -D__FLATJSON__VLEN_TYPE=std::uint32_t
)

include_directories(
    ./include
)

if (DEFINED ${BOOST_INCLUDE_DIR})
    include_directories(
        ${BOOST_INCLUDE_DIR}
    )
endif()

set(BINAPI_HEADERS
    binapi/api.hpp
    binapi/flatjson.hpp
    binapi/dtf.hpp
    binapi/double_type.hpp
    binapi/invoker.hpp
    binapi/io_state.hpp
    binapi/message.hpp
    binapi/pairslist.hpp
    binapi/reports.hpp
    binapi/tools.hpp
    binapi/types.hpp
    binapi/websocket.hpp
)

set(BINAPI_SOURCES
    src/api.cpp
    src/io_state.cpp
    src/pairslist.cpp
    src/reports.cpp
    src/tools.cpp
    src/types.cpp
    src/websocket.cpp
)

add_executable(
    ${PROJECT_NAME}
    #
    main.cpp
    #
    ${BINAPI_SOURCES}
)

if (DEFINED ${BOOST_LIB_DIR})
    target_link_libraries(
        ${PROJECT_NAME}
        -L${BOOST_LIB_DIR}
    )
endif()

target_link_libraries(
    ${PROJECT_NAME}
    z
    crypto
    ssl
    pthread
)
