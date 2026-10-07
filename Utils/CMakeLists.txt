cmake_minimum_required(VERSION 3.26)
project(Utils LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Quick QuickWidgets Graphs)

add_library(diagrams STATIC
    diagrams.cpp
)

qt_add_resources(diagrams "diagram_resources"
    PREFIX "/Diagrams"
    FILES
        graphs.qml
)

target_include_directories(diagrams PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(diagrams PUBLIC
    Qt6::Core
    Qt6::Widgets
    Qt6::Quick
    Qt6::QuickWidgets
    Qt6::Graphs
)

add_executable(diagrams_test diagrams_test.cpp)
target_link_libraries(diagrams_test PRIVATE diagrams)
