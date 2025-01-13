cmake_minimum_required(VERSION 3.10)

project(horizon)

# Set compiler options
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG -march=native -mtune=native -masm=intel")

# Add include directories
include_directories(
    ./src
    ./deps
)

# Add source files
set(SOURCES
    ./deps/string/string.cc
    ./src/colorize/colorize.cc
    ./src/defines/keywords_primary_data_types.cc
    ./src/errors/errors.cc
    ./src/lexer/lexer.cc
    ./src/misc/misc.cc
    ./src/parser/parser.cc
    ./src/entry/horizon.cc
)

# Create the executable target
add_executable(${PROJECT_NAME} ${SOURCES})