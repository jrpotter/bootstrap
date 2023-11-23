#include "sput.h"
#include "test_config.h"
#include "test_dyn_array.h"
#include "test_loader.h"
#include "test_path.h"

int main(int argc, char *argv[]) {
  sput_start_testing();

  sput_enter_suite("config");
  sput_run_test(test_config_load_root_dir_invalid);
  sput_run_test(test_config_load_target_invalid);
  sput_run_test(test_config_load_success);

  sput_enter_suite("dyn_array");
  sput_run_test(test_dyn_array_empty);
  sput_run_test(test_dyn_array_nonzero_capacity);

  sput_enter_suite("path");
  sput_run_test(test_join_path_single_segments);
  sput_run_test(test_join_path_multiple_segments);

  sput_enter_suite("loader");
  sput_run_test(test_read_spec_json_missing);
  sput_run_test(test_read_spec_json_minimal);
  sput_run_test(test_read_spec_json_invalid);

  sput_finish_testing();

  return sput_get_return_value();
}
