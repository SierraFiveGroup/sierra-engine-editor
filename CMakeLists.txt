cmake_minimum_required(VERSION 3.21)

# ---- Project ----
project(SierraEngineEditor
    VERSION 0.0.1
    DESCRIPTION "Sierra Engine Editor - Editor component for the Sierra Engine"
    LANGUAGES CXX
)

# ---- C++ Standard ----
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- Configure Qt ----
# Enable Qt's automatic MOC, UIC, RCC processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGLWidgets)

# ---- Source / Include directories ----
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(INC_DIR "${CMAKE_SOURCE_DIR}/include")

file(GLOB_RECURSE PROJECT_SOURCES
    "${SRC_DIR}/*.cpp"
    "${INC_DIR}/*.h"
    "${INC_DIR}/*.hpp"
)

# ---- Executable ----
add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
)

# Add include directory
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${INC_DIR}
)

# Link Qt
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::OpenGLWidgets
)

# ---- Platform-Specific Settings ----
if(WIN32)
    # Avoid console window for GUI apps on Windows
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
elseif(APPLE)
    # Bundle application on macOS
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
    )
endif()

# ---- Deployment Helpers (optional) ----
# Qt6 includes built-in deployment tools:
#   macdeployqt (macOS)
#   windeployqt (Windows)
# You can add custom commands if desired.