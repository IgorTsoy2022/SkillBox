cmake_minimum_required (VERSION 3.8)

add_executable (${PROJECT_NAME} CMakeM25main.cpp m25surgery.cpp
                m25cpu.cpp m25ram.cpp m25disk.cpp m25gpu.cpp m25kbd.cpp
                ${INCLUDE_DIR}/m25surgery.h
                ${INCLUDE_DIR}/m25cpu.h ${INCLUDE_DIR}/m25ram.h ${INCLUDE_DIR}/m25disk.h
                ${INCLUDE_DIR}/m25gpu.h ${INCLUDE_DIR}/m25kbd.h)
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})