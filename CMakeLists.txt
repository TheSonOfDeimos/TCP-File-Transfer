cmake_minimum_required(VERSION 3.10)



# set the project name
project(Daemon)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(SOURCE_DIR src)
set(COMMON_DIR ${SOURCE_DIR}/common)
set(DAEMON_DIR ${SOURCE_DIR}/daemon)
set(DAEMON_DETAIL_DIR ${DAEMON_DIR}/detail)

set(SOURCE_EXE 
    ${DAEMON_DIR}/main.cpp
    ${DAEMON_DETAIL_DIR}/daemon_body.cpp
    ${COMMON_DIR}/log_writer.cpp
    ${DAEMON_DETAIL_DIR}/monitor_proc.cpp
    ${DAEMON_DETAIL_DIR}/detail.h)

# add the executable
add_executable(${PROJECT_NAME} ${SOURCE_EXE})

target_link_libraries(${PROJECT_NAME} pthread)