cmake_minimum_required(VERSION 3.5)

project(Service_2 LANGUAGES CXX)

add_compile_definitions(ZMQ_STATIC)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS Network REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

include_directories(
        Sources/
        Resources/
        Sources/ZeroMQ/include/
)

link_directories(
        Sources/ZeroMQ/lib/
)

set(project_headers
    Sources/color_extractor_service.h
    Sources/ping.h
)


set(project_sources
        Sources/main.cpp
        Sources/color_extractor_service.cpp
        Sources/ping.cpp
)

set(ALL_LIBS
        general
        libzmq.a
        ws2_32
        iphlpapi
        Qt6::Core
        Qt6::Network
        Qt6::Widgets
)

add_executable (${PROJECT_NAME} ${project_sources} ${project_headers})

target_link_libraries(${PROJECT_NAME} ${ALL_LIBS})
