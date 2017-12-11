#define _GNU_SOURCE

#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <log.h>

#include "args.h"

#define QUOTE(x) #x

#define CHECK_ERROR(X, ...)\
  log_trace(QUOTE(X) ": %d", X);\
  error = X;\
  if(error) { log_error(__VA_ARGS__); break; } \


/**
 * Shows a message showing the parameters accepted by the program.
 */
void showHelp() {
  log_info("RCTVRP usage");
  log_info("============");
  log_info("./rctvrp -f <filepath>");
  log_info("<filepath>: File path where instance is defined");
}


/**
 * Parses the options from the CLI.
 * @param int argc - the number of arguments retrieved by the cli.
 * @param char** argv - the value of each parameter retrieved by the cli.
 * @returns Options* - a struct with the options defined in the cli, 
 * or NULL if there was an error while parsing.
 */

Options* getOptionsFromCLI(int argc, char **argv) {
  char c;
  char *absPath;
  int error = -1;
  Options *opts = calloc(1, sizeof(Options));
  opts->file = NULL;

  while((c = getopt(argc, argv, "f:")) != -1 && error <= 0) {
    switch(c) {
      case 'f':
        CHECK_ERROR(strlen(optarg) == 0, "File is empty");
        absPath = realpath(optarg, NULL);
        CHECK_ERROR(absPath == NULL, 
             "Cannot resolve path: %s", optarg);
        CHECK_ERROR(access(absPath, R_OK) != 0, 
            "Cannot read from path: %s", absPath);
        opts->file = absPath;
        break;
      default:
        error = 1;
        break;
    }
  }

  if(error != 0) {
    if(opts->file != NULL) {
      free(opts->file);
      opts->file = NULL;
    }
    free(opts);
    opts = NULL;
    showHelp();
  } else {
    log_info("Reading instance from %s", opts->file);
  }

  return opts;
}
