cmake_minimum_required(VERSION 3.19)
project(bootstrap VERSION 0.1.3 LANGUAGES C)

add_executable(bootstrap main.c)

add_subdirectory(src)
target_link_libraries(bootstrap PUBLIC m)
target_include_directories(bootstrap PUBLIC include)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  set_target_properties(
    bootstrap PROPERTIES
      # The top-level compile_commands.json is a symbolic link to this file.
      EXPORT_COMPILE_COMMANDS ON
      # Enable so that tests can link against the primary executable.
      ENABLE_EXPORTS ON
  )
  enable_testing()
  add_subdirectory(test)
endif()
