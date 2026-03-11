include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(mcse-tests
    order_book_test.cpp
)

target_include_directories(mcse-tests PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/engine
        ${CMAKE_SOURCE_DIR}/src/protocol
)

target_link_libraries(mcse-tests
    mcse-engine
    mcse-protocol
    gtest
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(mcse-tests)
