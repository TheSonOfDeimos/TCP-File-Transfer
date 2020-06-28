cmake_minimum_required(VERSION 3.10)

project(TcpApp)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CXX_HEADER_TYPE "/*.hpp")
set(CXX_SOURCE_TYPE "/*.cpp")

# Lib boost
find_package(Boost COMPONENTS system filesystem thread REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

set(SOURCE_DIR src/)
set(COMMON_DIR ${SOURCE_DIR}/common/)


# ======================= Daemon ================================
if (Linux)
    set(DAEMON_DIR ${SOURCE_DIR}/daemon/)
    set(DAEMON_DETAIL_DIR ${DAEMON_DIR}/detail/)
    file(GLOB SOURCE_DAEMON
        ${DAEMON_DIR}${CXX_HEADER_TYPE}
        ${DAEMON_DIR}${CXX_SOURCE_TYPE}
        ${DAEMON_DETAIL_DIR}${CXX_HEADER_TYPE}
        ${DAEMON_DETAIL_DIR}${CXX_SOURCE_TYPE}
        ${COMMON_DIR}${CXX_HEADER_TYPE}
        ${COMMON_DIR}${CXX_SOURCE_TYPE})

    set(DAEMON_TARG Daemon)
    add_executable(${DAEMON_TARG} ${SOURCE_DAEMON})
    target_link_libraries(${DAEMON_TARG} pthread)
endif(Linux)

# ===================== TCP Server ==============================
set(SERVER_DIR ${SOURCE_DIR}/server/)
file(GLOB SOURCE_SERVER
    ${SERVER_DIR}${CXX_SOURCE_TYPE}
    ${SERVER_DIR}${CXX_HEADER_TYPE})

set(SERVER_TARG Server)
add_executable(${SERVER_TARG} ${SOURCE_SERVER})
target_link_libraries(${SERVER_TARG} ${Boost_LIBRARIES} pthread)

#===================== TCP Client ===============================
set(CLIENT_DIR ${SOURCE_DIR}/client/)
file(GLOB SOURCE_CLIENT
    ${CLIENT_DIR}${CXX_SOURCE_TYPE}
    ${CLIENT_DIR}${CXX_HEADER_TYPE})

set(CLIENT_TARG Client)
add_executable(${CLIENT_TARG} ${SOURCE_CLIENT})
target_link_libraries(${CLIENT_TARG} ${Boost_LIBRARIES} pthread)
