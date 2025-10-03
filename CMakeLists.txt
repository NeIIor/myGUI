# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(ReactorSimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(ReactorSimulator
    main.cpp
    ui/UIApplication.cpp
    ui/Widget.cpp
    ui/Events.cpp
    ui/Container.cpp
    ui/Button.cpp
    ui/Window.cpp
    ui/ClockWidget.cpp
    ui/ReactorUI.cpp
    sim/Reactor.cpp
    sim/ReactorRenderer.cpp
    sim/GraphRenderer.cpp
)

target_link_libraries(ReactorSimulator sfml-graphics sfml-window sfml-system)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(ReactorSimulator PRIVATE DEBUG)
    target_compile_options(ReactorSimulator PRIVATE -g -O0)
else()
    target_compile_options(ReactorSimulator PRIVATE -O2)
endif()