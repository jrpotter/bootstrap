#include "sput.h"
#include "test_config.h"
#include "test_dyn_array.h"
#include "test_parser.h"
#include "test_string_buf.h"
#include "test_string_utils.h"
#include "test_validator.h"

int main(int argc, char *argv[]) {
  sput_start_testing();

  sput_enter_suite("config");
  sput_run_test(test_config_new_invalid_args);
  sput_run_test(test_config_new_spec_not_found);
  sput_run_test(test_config_new_spec_not_dir);
  sput_run_test(test_config_new_success);

  sput_enter_suite("dyn_array");
  sput_run_test(test_dyn_array_zero_capacity);
  sput_run_test(test_dyn_array_nonzero_capacity);

  sput_enter_suite("string_buf");
  sput_run_test(test_string_buf_sappend);
  sput_run_test(test_string_buf_cappend);

  sput_enter_suite("string_utils");
  sput_run_test(test_join_single);
  sput_run_test(test_join_multiple);
  sput_run_test(test_strcmp_ci);
  sput_run_test(test_trim_leading);
  sput_run_test(test_trim_trailing);

  sput_enter_suite("parser");
  sput_run_test(test_parser_missing);
  sput_run_test(test_parser_minimal);
  sput_run_test(test_parser_invalid);

  sput_enter_suite("validator");
  sput_run_test(test_validator_toplevel_not_object);
  sput_run_test(test_validator_field_not_object);
  sput_run_test(test_validator_field_name_leading_digit);
  sput_run_test(test_validator_field_name_non_alnum);
  sput_run_test(test_validator_field_type_invalid);
  sput_run_test(test_validator_field_type_unknown);
  sput_run_test(test_validator_valid_type_ci);
  sput_run_test(test_validator_field_required_invalid);
  sput_run_test(test_validator_field_required_valid);
  sput_run_test(test_validator_field_prompt_invalid);
  sput_run_test(test_validator_valid_no_required);

  sput_finish_testing();

  return sput_get_return_value();
}
