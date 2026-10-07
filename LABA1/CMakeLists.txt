cmake_minimum_required(VERSION 3.16)
project(laba1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core)
find_package(Threads REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "./**.cpp" "./**.ui" "./**.h" "./**.hpp")

# Статическая сборка
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${GUI_RESOURCES})
# target_include_directories(${PROJECT_NAME} PRIVATE ../${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Threads::Threads
    diagrams
)
