cmake_minimum_required(VERSION 3.28)
project(json_parser)

set(CMAKE_CXX_STANDARD 20)

# Add main executable
add_executable(json_parser main.cpp json_parser.cpp)

# Include FetchContent module
include(FetchContent)

# Download and add Catch2
FetchContent_Declare(
        catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.3.0 # Replace with the latest version tag
)

FetchContent_MakeAvailable(catch2)

# Add test executable
add_executable(tests tests/tests.cpp json_parser.cpp)  # Include parse_json.cpp

# Link Catch2 with the test executable
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

# Enable CTest framework
enable_testing()

# Discover and add tests to CTest
include(Catch)
catch_discover_tests(tests)
