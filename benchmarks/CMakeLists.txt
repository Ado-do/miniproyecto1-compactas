add_executable(bench_example example.cpp)
target_link_libraries(bench_example PRIVATE miniproyecto1 bench-lib)
target_compile_options(bench_example PRIVATE -O3)
