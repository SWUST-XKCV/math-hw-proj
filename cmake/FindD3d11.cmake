set(TARGET_NAME D3d11)

find_path(${TARGET_NAME}_INCLUDE_DIRS
    NAMES d3d11.h
    PATHS "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.22621.0\\um"
    REGISTRY_VIEW 64
)

if(NOT ${TARGET_NAME}_INCLUDE_DIRS)
    message(WARNING "${TARGET_NAME} include dirs not found.")
endif()

find_path(${TARGET_NAME}_LIBRARY_DIRS
    NAMES d3d11.lib
    PATHS "C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.22621.0\\um\\x64"
    REGISTRY_VIEW 64
)

if(NOT ${TARGET_NAME}_LIBRARY_DIRS)
    message(WARNING "${TARGET_NAME} library dirs not found.")
endif()

if(${TARGET_NAME}_LIBRARY_DIRS AND ${TARGET_NAME}_INCLUDE_DIRS)
    message(STATUS "Found ${TARGET_NAME}.")
    set(${TARGET_NAME}_FOUND true)
endif()

