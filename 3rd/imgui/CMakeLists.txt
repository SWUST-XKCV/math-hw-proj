set(TARGET_NAME imgui)

file(GLOB ${TARGET_NAME}_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_opengl2.h
    ${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_opengl2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_glfw.h
    ${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
)

add_library(imgui STATIC ${${TARGET_NAME}_SRC})
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(imgui PRIVATE ${CMAKE_SOURCE_DIR}/src/glfw/include)
target_link_libraries(imgui PUBLIC glfw)
