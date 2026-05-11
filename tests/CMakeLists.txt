add_executable(test_wt wt.cpp)
target_link_libraries(test_wt PRIVATE miniproyecto1)
add_test(
  NAME basic_test_wt
  COMMAND test_wt "${CMAKE_SOURCE_DIR}/input/english.50MB"
)
target_compile_options(test_wt PRIVATE -O3)

add_executable(test_occ occ.cpp)
target_link_libraries(test_occ PRIVATE miniproyecto1)
add_test(
  NAME basic_test_occ
  COMMAND test_occ "${CMAKE_SOURCE_DIR}/input/english.50MB"
)
target_compile_options(test_occ PRIVATE -O3)
