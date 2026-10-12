cmake_minimum_required(VERSION 3.14)
project(Flashies LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Core model classes shared by the CLI and every test binary
add_library(flashies_core STATIC Card.cpp Deck.cpp User.cpp)
target_include_directories(flashies_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Console app
add_executable(flashcards main.cpp)
target_link_libraries(flashcards PRIVATE flashies_core)

# ---------- Tests (Catch2 v2, single header: catch.hpp) ----------
option(FLASHIES_BUILD_TESTS "Build the Catch2 unit tests" ON)

if(FLASHIES_BUILD_TESTS)
    enable_testing()

    # Each test file defines CATCH_CONFIG_MAIN, so each becomes its own binary
    foreach(name card deck user)
        add_executable(test_${name} test_${name}.cpp)
        target_link_libraries(test_${name} PRIVATE flashies_core)
        add_test(NAME ${name} COMMAND test_${name})
    endforeach()

    # test_app exercises the JSON load/save functions in main.cpp,
    # compiled without its main() so Catch2 can provide one
    add_executable(test_app test_app.cpp main.cpp)
    target_compile_definitions(test_app PRIVATE FLASHIES_NO_MAIN)
    target_link_libraries(test_app PRIVATE flashies_core)
    add_test(NAME app COMMAND test_app WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()
