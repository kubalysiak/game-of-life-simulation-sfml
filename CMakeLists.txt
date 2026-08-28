cmake_minimum_required(VERSION 3.15)

project(GameOfLifekubalysiak)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

set(SOURCES
    main.cpp
    Root.cpp
    Board.cpp
    Button.cpp
    TextField.cpp
    Label.cpp
    Drawable.cpp
)

set(HEADERS
    Root.h
    Board.h
    Button.h
    TextField.h
    Label.h
    Drawable.h
    Preset.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system)