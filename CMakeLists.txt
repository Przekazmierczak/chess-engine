cmake_minimum_required(VERSION 3.10)
project(ChessMinMax)

# Enable testing
enable_testing()

# Include CMake's FetchContent module
include(FetchContent)

# Download GoogleTest automatically if not available
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

# Set runtime library to MTd (multi-threaded debug)
if(MSVC)
    # For MSVC, set the runtime library to MTd (multi-threaded debug)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

# Add the test executable
add_executable(ChessMinMaxTests Board_unittest.cpp Piece_unittest.cpp Board.cpp Piece.cpp Types.cpp)

# Link GoogleTest libraries
target_link_libraries(ChessMinMaxTests gtest_main)

# Register the test with CTest
add_test(NAME MyTest COMMAND ChessMinMaxTests)