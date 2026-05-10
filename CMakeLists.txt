cmake_minimum_required(VERSION 3.16)

project(mini_thriftme VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# =========================
# Options
# =========================

option(BUILD_CLIENT "Build RPC client" ON)
option(BUILD_SERVER "Build RPC server" ON)

# =========================
# Find dependencies
# =========================

find_package(Threads REQUIRED)

find_program(THRIFT_COMPILER thrift REQUIRED)

# =========================
# Paths
# =========================

set(GENERATED_DIR
    ${CMAKE_BINARY_DIR}/generated
)

file(MAKE_DIRECTORY ${GENERATED_DIR})

# =========================
# Thrift generation
# =========================

set(THRIFT_FILE
    ${CMAKE_SOURCE_DIR}/proto/vehicle.thrift
)

add_custom_command(
    OUTPUT
        ${GENERATED_DIR}/VehicleService.cpp
        ${GENERATED_DIR}/VehicleService.h
        ${GENERATED_DIR}/vehicle_types.h

    COMMAND ${THRIFT_COMPILER}
        --gen cpp
        -out ${GENERATED_DIR}
        ${THRIFT_FILE}

    DEPENDS
        ${THRIFT_FILE}

    COMMENT "Generating Thrift C++ sources"
)

add_custom_target(generate_thrift
    DEPENDS
        ${GENERATED_DIR}/VehicleService.cpp
        ${GENERATED_DIR}/VehicleService.h
        ${GENERATED_DIR}/vehicle_types.h
)

# =========================
# Includes
# =========================

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${GENERATED_DIR}
)

# =========================
# Common sources
# =========================

set(COMMON_SOURCES
    src/broker/ServiceBroker.cpp
    src/services/VehicleService.cpp
    src/transport/TcpServer.cpp
    src/transport/TcpClient.cpp
)

set(THRIFT_GENERATED_SOURCES
    ${GENERATED_DIR}/VehicleService.cpp
)

# =========================
# Server
# =========================

if(BUILD_SERVER)

    add_executable(mini_thriftme_server
        src/server/main.cpp
        ${COMMON_SOURCES}
        ${THRIFT_GENERATED_SOURCES}
    )

    add_dependencies(
        mini_thriftme_server
        generate_thrift
    )

    target_link_libraries(
        mini_thriftme_server
        thrift
        Threads::Threads
    )

    add_executable(thrift_server
        src/server/thrift_server.cpp
        ${THRIFT_GENERATED_SOURCES}
    )

    add_dependencies(
        thrift_server
        generate_thrift
    )

    target_link_libraries(thrift_server
        thrift
        Threads::Threads
    )

endif()

# =========================
# Client
# =========================

if(BUILD_CLIENT)

    add_executable(mini_thriftme_client
        src/client/main.cpp
        ${COMMON_SOURCES}
        ${THRIFT_GENERATED_SOURCES}
    )

    add_dependencies(
        mini_thriftme_client
        generate_thrift
    )

    target_link_libraries(
        mini_thriftme_client
        thrift
        Threads::Threads
    )

    add_executable(thrift_client
        src/client/thrift_client.cpp
        ${THRIFT_GENERATED_SOURCES}
    )

    add_dependencies(
        thrift_client
        generate_thrift
    )

    target_link_libraries(thrift_client
        thrift
        Threads::Threads
    )

endif()