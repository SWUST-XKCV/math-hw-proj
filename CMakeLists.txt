cmake_minimum_required(VERSION 3.20)

project(math-hw-proj)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Get lib sqlite
add_subdirectory(3rd/sqlite)

# Get lib imgui
add_subdirectory(3rd/imgui)

file(GLOB_RECURSE APP_SRC CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/app/*.h
    ${CMAKE_SOURCE_DIR}/src/app/*.c
    ${CMAKE_SOURCE_DIR}/src/app/*.hpp
    ${CMAKE_SOURCE_DIR}/src/app/*.cpp
)

add_executable(app ${APP_SRC})
set_target_properties(
    app
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/bin
)
target_include_directories(app PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/3rd)
target_link_libraries(app PUBLIC sqlite imgui d3d11)

file(GLOB_RECURSE IMGUI_DEMO_SRC CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/imgui-demo/*.h
    ${CMAKE_SOURCE_DIR}/src/imgui-demo/*.c
    ${CMAKE_SOURCE_DIR}/src/imgui-demo/*.hpp
    ${CMAKE_SOURCE_DIR}/src/imgui-demo/*.cpp
)

add_executable(imgui-demo ${IMGUI_DEMO_SRC})
set_target_properties(
    imgui-demo
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/bin
)
target_include_directories(imgui-demo PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/3rd)
target_link_libraries(imgui-demo PUBLIC imgui d3d11)
