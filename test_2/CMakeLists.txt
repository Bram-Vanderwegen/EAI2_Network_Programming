cmake_minimum_required(VERSION 3.5)

project(test_broker_interaction LANGUAGES CXX)

add_compile_definitions(ZMQ_STATIC)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
        Sources/
        C:/Programming_shortcuts/Network_programming/ZeroMQ/include/
)

link_directories(
        C:/Programming_shortcuts/Network_programming/ZeroMQ/lib/
)

set(project_headers

)

set(project_sources
        Sources/main.cpp
)

set(ALL_LIBS
        general
        libzmq.a
        ws2_32
        iphlpapi
)

add_executable (${PROJECT_NAME} ${project_sources} ${project_headers})

target_link_libraries(${PROJECT_NAME} ${ALL_LIBS})
