cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("CMakeBoost")

set(CMAKE_CXX_STANDARD 17)

set(Boost_INCLUDE_DIR "C:\\Boost_1_88_0")
set(Boost_LIBRARY_DIR "C:\\Boost_1_88_0\\stage\\lib")
find_package(Boost COMPONENTS REQUIRED)

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

add_executable (${PROJECT_NAME} "CMakeBoost.cpp")

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

if (MINGW)
  target_link_libraries(${PROJECT_NAME} ws2_32)
endif()