add_executable(test-hello test_hello.c)
target_link_libraries(test-hello PRIVATE "<NAME>")
target_include_directories(test-hello PRIVATE "<NAME>")

add_test(NAME hello COMMAND ./test-hello)
