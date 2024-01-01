cmake_minimum_required(VERSION 3.15)
project("<NAME>" VERSION 0.1.0)

# The default CMAKE_BUILD_TYPE is undefined, i.e. toolchain specific. Enforcing
# a default makes builds work more consistently.
if (NOT CMAKE_BUILD_TYPE)
  message("CMAKE_BUILD_TYPE was not set. Defaulting to Debug")
  set(CMAKE_BUILD_TYPE Debug)
endif()

# The primary executable.
add_executable("<NAME>" main.c)

# Include the cmake commands found in `src`. This subdirectory is responsible
# for linking the other project source files to our executable.
add_subdirectory(src)

# An example on linking additional libraries. In this case, links `math`.
target_link_libraries("<NAME>" PUBLIC m)

# Location of different headers to include when compiling the executable.
target_include_directories("<NAME>" PUBLIC include)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  set_target_properties(
    "<NAME>" PROPERTIES
      # The top-level `compile_commands.json` is a symbolic link to this file.
      EXPORT_COMPILE_COMMANDS ON
      # Enable so that tests can link against the primary executable.
      ENABLE_EXPORTS ON
  )

  # Turns on testing. This must be done at the root `CMakeLists.txt` file. If
  # looking for additional testing configuration, consider using
  # `include(CTest)` instead.
  enable_testing()

  # Add test files to our cmake configuration.
  add_subdirectory(test)
endif()
