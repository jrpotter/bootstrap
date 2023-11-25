#include "error.h"

void error_free(struct Error *error) {
  if (!error) {
    return;
  }
  free((void *)error->message);
  free(error);
  error = 0;
}
