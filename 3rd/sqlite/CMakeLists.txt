set(TARGET_NAME sqlite)

set(${TARGET_NAME}_SRC
    sqlite3.c
    sqlite3.h
)

add_library(sqlite STATIC ${${TARGET_NAME}_SRC})
target_include_directories(sqlite PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
