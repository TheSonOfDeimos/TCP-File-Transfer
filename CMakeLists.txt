cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set the project name
project(Daemon)

set(SOURCE_DIR src)
set(DAEMON_DIR ${SOURCE_DIR}/daemon)
set(SOURCE_EXE 
    ${DAEMON_DIR}/main.cpp
    ${DAEMON_DIR}/daemon_body.cpp
    ${DAEMON_DIR}/log_writer.cpp
    ${DAEMON_DIR}/monitor_proc.cpp
    ${DAEMON_DIR}/detail.h)

# add the executable
add_executable(${PROJECT_NAME} ${SOURCE_EXE})