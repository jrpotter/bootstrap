#include "sput.h"
#include "test_config.h"
#include "test_dyn_array.h"
#include "test_parser.h"
#include "test_path.h"
#include "test_validator.h"

int main(int argc, char *argv[]) {
  sput_start_testing();

  sput_enter_suite("config");
  sput_run_test(test_config_load_invalid_args);
  sput_run_test(test_config_load_spec_not_found);
  sput_run_test(test_config_load_spec_not_dir);
  sput_run_test(test_config_load_success);

  sput_enter_suite("dyn_array");
  sput_run_test(test_dyn_array_zero_capacity);
  sput_run_test(test_dyn_array_nonzero_capacity);

  sput_enter_suite("path");
  sput_run_test(test_join_path_single_segments);
  sput_run_test(test_join_path_multiple_segments);

  sput_enter_suite("parser");
  sput_run_test(test_parse_spec_json_missing);
  sput_run_test(test_parse_spec_json_minimal);
  sput_run_test(test_parse_spec_json_invalid);

  sput_enter_suite("validator");
  sput_run_test(test_validate_spec_json_not_toplevel_object);

  sput_finish_testing();

  return sput_get_return_value();
}
