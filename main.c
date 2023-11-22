#include <assert.h>
#include <curses.h>
#include <glob.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "dyn_array.h"

static int glob_specs(char *name, struct DynArray *entries) {
  assert(name);
  assert(entries);

  int name_length = strlen(name);
  char *pattern = malloc(name_length + 3);
  if (!pattern) {
    goto cleanup;
  }
  memcpy(pattern, name, name_length);
  memcpy(pattern + name_length + 1, "/*", 3);

  glob_t *pglob = 0;

  int retval = 0;
  if ((retval = glob(pattern, GLOB_ONLYDIR | GLOB_MARK, 0, pglob))) {
    goto cleanup;
  }

  // GLOB_ONLYDIR is just a hint. We still need to actually verify the only
  // results we are reading in are directories.
  // TODO:

  globfree(pglob);

cleanup:
  if (pattern) {
    free(pattern);
  }
  return retval;
}

static void cleanup(int sig) {
  endwin();

  exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
  int num = 0;

  if (argc != 2) {
    fprintf(stderr, "Usage: gen-flake <spec>\n");
    exit(EXIT_FAILURE);
  }

  // Allow interrupting the program cleanly.
  // TODO: How does this cleanup spec correctly?
  signal(SIGINT, cleanup);

  initscr();
  keypad(stdscr, TRUE); // Enable keyboard mapping.
  nonl();               // Disables NL to CR/NL conversion on output.

  cleanup(0);
}
