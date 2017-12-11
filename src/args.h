#ifndef _ARGS_H_
#define _ARGS_H_

/**
 *  Represents the configuration options received from the CLI.
 */
typedef struct Options {
  /**
   *  A path where the instance file is located.
   */
  char* file;
} Options;

Options* getOptionsFromCLI(int, char**);

#endif //_ARGS_H_
