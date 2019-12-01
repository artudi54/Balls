cmake_minimum_required(VERSION 3.10)
project(Balls VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Dependencies
find_package(Qt5 HINTS $ENV{QTDIR} REQUIRED
COMPONENTS
    Core
    Gui
    Widgets
)

# Executable
set(SOURCES
    src/Ball.cpp
    src/BallColider.cpp
        src/BallAnimator.cpp
    src/BallParameters.cpp
    src/main.cpp
)
set(HEADERS
    src/Ball.hpp
    src/BallColider.hpp
        src/BallAnimator.hpp
    src/BallParameters.hpp
 )
set(UI_FILES
    ui/BallColider.ui
)
qt5_wrap_ui(SOURCES ${UI_FILES})
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${UI_SOURCES} ${RESOURCES})

# Compile options
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_14)
set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Wconversion -Wsign-conversion)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(IGNORED_WARNINGS /wd4514 /wd4571 /wd4623 /wd4625 /wd4626 /wd4668 /wd4710 /wd4774 /wd4820 /wd5026 /wd5027 /wd5045)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /Wall /Za /permissive- ${IGNORED_WARNINGS})
    target_compile_definitions(${PROJECT_NAME} PRIVATE -D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS -D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS)
endif()
set_target_properties(${PROJECT_NAME}
    PROPERTIES
        AUTOMOC ON
        AUTORCC ON
)

# Include directories
target_include_directories(${PROJECT_NAME}
PRIVATE
    src
)


# Linked libraries
target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets)

