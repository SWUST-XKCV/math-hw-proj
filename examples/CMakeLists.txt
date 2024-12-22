set(REGRESSION_TREE_SRC
    regression_tree.cpp
    ${CMAKE_SOURCE_DIR}/src/app/regression_tree.cpp
    ${CMAKE_SOURCE_DIR}/src/app/regression_tree.hpp
    ${CMAKE_SOURCE_DIR}/src/app/utility.cpp
    ${CMAKE_SOURCE_DIR}/src/app/utility.hpp
    ${CMAKE_SOURCE_DIR}/src/app/logger.cpp
    ${CMAKE_SOURCE_DIR}/src/app/logger.hpp
)

add_executable(regression_tree ${REGRESSION_TREE_SRC})
set_target_properties(
    regression_tree
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/bin
)
target_include_directories(regression_tree PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/3rd
)
