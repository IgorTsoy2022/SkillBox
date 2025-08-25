cmake_minimum_required(VERSION 3.31)
project(QtM38_01_Button_Click)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH "C:\\Qt\\6.5.3\\msvc2019_64")

find_package(Qt6 COMPONENTS
    Core
    Gui
    Widgets
    Multimedia
    REQUIRED)

#qt_add_resources(RESOURCES resources.qrc)

qt_add_executable(${PROJECT_NAME}
    main.cpp
    mainwindow.cpp
    mainwindow.h
#    ${RESOURCES}
)

qt_add_resources(${PROJECT_NAME} "resources"
    PREFIX "/"
    FILES
        images/sphere_red.png
        images/sphere_red_pressed.png
        sounds/click.wav
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Multimedia
)