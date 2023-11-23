#include "sput.h"
#include "test_config.h"

int main(int argc, char *argv[]) {
  sput_start_testing();

  test_config_run();

  sput_finish_testing();

  return sput_get_return_value();
}
