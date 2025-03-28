cmake_minimum_required (VERSION 3.8)

add_executable (${PROJECT_NAME} CMakeM25main.cpp m25surgery.cpp ${INCLUDE_DIR}/m25surgery.h)
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})
