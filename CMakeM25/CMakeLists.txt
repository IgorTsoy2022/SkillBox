cmake_minimum_required (VERSION 3.8)

# Enabling hot reloading for MSVC compilers, if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("CMakeM25" VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

# set(SOURCES SRC/CMakeM25main.cpp SRC/CMakeM25.cpp)

# include_directories("include/")

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(SRC)


if (CMAKE_VERSION VERSION_GREATER 3.12)
   set_property(TARGET CMakeM25 PROPERTY CXX_STANDARD 20)
endif()
