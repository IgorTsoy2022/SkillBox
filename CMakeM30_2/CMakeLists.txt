cmake_minimum_required (VERSION 3.8)

# Enable hot reloading for MSVC compilers, if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("CMakeM30_2")

set(CMAKE_CXX_STANDARD 17)

SET(BUILD_CPR_TESTS OFF)
SET(CMAKE_USE_OPENSSL OFF)

SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG dd967cb48ea6bcbad9f1da5ada0db8ac0d532c06) # Replace with your desired git commit from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)

add_executable (CMakeM30_2 "CMakeM30_2.cpp")

target_link_libraries (${PROJECT_NAME} PRIVATE cpr::cpr)

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET CMakeM30_2 PROPERTY CXX_STANDARD 20)
endif()
