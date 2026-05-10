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
# Includes
# =========================

include_directories(
    ${CMAKE_SOURCE_DIR}/src
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

# =========================
# Server
# =========================

if(BUILD_SERVER)

    add_executable(mini_thriftme_server
        src/server/main.cpp
        ${COMMON_SOURCES}
    )

endif()

# =========================
# Client
# =========================

if(BUILD_CLIENT)

    add_executable(mini_thriftme_client
        src/client/main.cpp
        ${COMMON_SOURCES}
    )

endif()
