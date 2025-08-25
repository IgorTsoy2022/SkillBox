cmake_minimum_required(VERSION 3.31)

project(QtM38_02_HTML)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.5.3/msvc2019_64")

find_package(Qt6 COMPONENTS
    Core
    Gui
    Widgets
    WebEngineWidgets
REQUIRED)

add_executable(${PROJECT_NAME}
    main.cpp
    mainwindow.h
    mainwindow.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Widgets
    Qt::WebEngineWidgets
)