cmake_minimum_required(VERSION 3.10)

project(TcpApp)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Lib boost
find_package(Boost COMPONENTS system filesystem thread REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

set(SOURCE_DIR src)
set(COMMON_DIR ${SOURCE_DIR}/common)


# ======================= Daemon ================================
set(DAEMON_DIR ${SOURCE_DIR}/daemon)
set(DAEMON_DETAIL_DIR ${DAEMON_DIR}/detail)
set(SOURCE_DAEMON 
    ${DAEMON_DIR}/main.cpp
    ${DAEMON_DETAIL_DIR}/daemon_body.cpp
    ${COMMON_DIR}/log_writer.cpp
    ${DAEMON_DETAIL_DIR}/monitor_proc.cpp
    ${DAEMON_DETAIL_DIR}/detail.h)

set(DAEMON_TARG Daemon)
add_executable(${DAEMON_TARG} ${SOURCE_DAEMON})
target_link_libraries(${DAEMON_TARG} pthread)

# ===================== TCP Server ==============================
set(SERVER_DIR ${SOURCE_DIR}/server)
set(SOURCE_SERVER
    ${SERVER_DIR}/server.cpp
    ${SERVER_DIR}/session.cpp
    ${SERVER_DIR}/main.cpp)

set(SERVER_TARG Server)
add_executable(${SERVER_TARG} ${SOURCE_SERVER})
target_link_libraries(${SERVER_TARG} ${Boost_LIBRARIES} pthread)

#===================== TCP Client ===============================
set(CLIENT_DIR ${SOURCE_DIR}/client)
set(SOURCE_CLIENT
    ${CLIENT_DIR}/client.h
    ${CLIENT_DIR}/main.cpp)

set(CLIENT_TARG Client)
add_executable(${CLIENT_TARG} ${SOURCE_CLIENT})
target_link_libraries(${CLIENT_TARG} ${Boost_LIBRARIES} pthread)
