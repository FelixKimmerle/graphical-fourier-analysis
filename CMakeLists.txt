cmake_minimum_required(VERSION 3.28)
project(GraphicalFourierAnalysis LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Fetch SFML so no system installation is required.
include(FetchContent)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(GraphicalFourierAnalysis
    main.cpp
    FT/FT.cpp
    Math/Lexer.cpp
    Math/MathParser.cpp
    Math/Programm.cpp
    Math/Result.cpp
    Math/Token.cpp
    Math/TokenDefinition.cpp
    Math/TokenPosition.cpp
    Plot/RectanglePlot.cpp
    Plot/RoundChart.cpp)

target_include_directories(GraphicalFourierAnalysis PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(GraphicalFourierAnalysis PRIVATE SFML::Graphics)

if(MSVC)
    target_compile_options(GraphicalFourierAnalysis PRIVATE /W4)
else()
    target_compile_options(GraphicalFourierAnalysis PRIVATE -Wall -Wextra)
endif()
