#include "sput.h"
#include "test_config.h"
#include "test_dyn_array.h"

int main(int argc, char *argv[]) {
  sput_start_testing();

  sput_enter_suite("config");
  sput_run_test(test_config_load_root_dir_invalid);
  sput_run_test(test_config_load_target_invalid);
  sput_run_test(test_config_load_success);

  sput_enter_suite("dyn_array");
  sput_run_test(test_dyn_array_empty);
  sput_run_test(test_dyn_array_nonzero_capacity);

  sput_finish_testing();

  return sput_get_return_value();
}
