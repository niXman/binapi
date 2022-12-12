cmake_minimum_required(VERSION 3.2)
project(bg_api)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fsanitize=address")

add_definitions(
    -UNDEBUG
    -DDTF_HEADER_ONLY
)

include_directories(
    ./include
)

if (DEFINED ${BOOST_INCLUDE_DIR})
    include_directories(
        ${BOOST_INCLUDE_DIR}
    )
endif()

set(BG_API_HEADERS
    bg_api/api.hpp
    bg_api/flatjson.hpp
    bg_api/dtf.hpp
    bg_api/double_type.hpp
    bg_api/enums.hpp
    bg_api/errors.hpp
    bg_api/invoker.hpp
    bg_api/message.hpp
    bg_api/pairslist.hpp
    bg_api/reports.hpp
    bg_api/tools.hpp
    bg_api/types.hpp
    bg_api/websocket.hpp
)

set(BG_API_SOURCES
    src/api.cpp
    src/enums.cpp
    src/errors.cpp
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
    ${BG_API_SOURCES}
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
