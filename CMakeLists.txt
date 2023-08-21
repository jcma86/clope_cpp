cmake_minimum_required(VERSION 3.5)
project(FROM_Cro_TO_ML)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_C_FLAGS "-Wall")# Libraries

add_library(cluster STATIC ./include/cluster.cpp)
add_library(database STATIC ./include/database.cpp)
add_library(dbmushroom STATIC ./include/dbmushroom.cpp)
add_library(transaction STATIC ./include/transaction.cpp)
add_library(txmushroom STATIC ./include/txmushroom.cpp)
add_library(clope STATIC ./include/clope.cpp)

target_link_libraries(${database} transaction)
target_link_libraries(${txmushroom} transaction)
target_link_libraries(${dbmushroom} database txmushroom)
target_link_libraries(${clope} cluster database transaction dbmushroom)

set(TEST_CLOPE test_clope)
add_executable(${TEST_CLOPE} ./test_clope.cpp)
target_link_libraries(${TEST_CLOPE} m clope dbmushroom cluster)