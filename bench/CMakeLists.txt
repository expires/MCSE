include(FetchContent)

FetchContent_Declare(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.8.3
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googlebenchmark)

file(GLOB BENCH_SOURCES "*.cpp")

add_executable(mcse-bench
    ${BENCH_SOURCES}
)

target_link_libraries(mcse-bench
    mcse-engine
    benchmark
    benchmark_main
)
