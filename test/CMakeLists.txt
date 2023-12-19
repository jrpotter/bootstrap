file(
  COPY ${CMAKE_CURRENT_SOURCE_DIR}/specs
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

foreach(
  exe
    config
    dyn_array
    parser
    string_buf
    string_utils
    validator
)
  add_executable("test-${exe}" "test_${exe}.c")
  target_link_libraries("test-${exe}" PRIVATE bootstrap)
  target_include_directories("test-${exe}" PRIVATE bootstrap)
endforeach()

# ========================================
# config
# ========================================

foreach(
  arg
    new-invalid-args
    new-spec-not-found
    new-spec-not-dir
    new-success
)
  add_test(NAME "config: ${arg}" COMMAND test-config ${arg})
endforeach()

# ========================================
# dyn_array
# ========================================

foreach(
  arg
    zero-capacity
    nonzero-capacity
)
  add_test(NAME "dyn_array: ${arg}" COMMAND test-dyn_array ${arg})
endforeach()

# ========================================
# parser
# ========================================

foreach(
  arg
    missing 
    minimal
    invalid
)
  add_test(NAME "parser: ${arg}" COMMAND test-parser ${arg})
endforeach()

# ========================================
# string_buf
# ========================================

foreach(
  arg
    sappend
    cappend
)
  add_test(NAME "string_buf: ${arg}" COMMAND test-string_buf ${arg})
endforeach()

# ========================================
# string_utils
# ========================================

foreach(
  arg
    join-single
    join-multiple
    strcmp-ci
    trim-leading
    trim-trailing
)
  add_test(NAME "string_utils: ${arg}" COMMAND test-string_utils ${arg})
endforeach()

# ========================================
# validator
# ========================================

foreach(
  arg
    toplevel-not-object
    field-not-object
    field-name-leading-digit
    field-name-non-alnum
    field-type-invalid
    field-type-unknown
    valid-type-ci
    field-required-invalid
    field-required-valid
    field-prompt-invalid
    valid-no-required
    field-type-yes
)
  add_test(NAME "validator: ${arg}" COMMAND test-validator ${arg})
endforeach()
