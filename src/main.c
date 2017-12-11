#include <log.h>
#include "args.h"
#include "returns.h"
#include "instance.h"
#include "definitions.h"
#include "forwardchecking.h"

#define LOG_LEVEL LOG_DEBUG

int main(int argc, char *argv[]) {
  log_set_level(LOG_LEVEL);
  Options *opts = getOptionsFromCLI(argc, argv);
  if(opts == NULL) { return MISSING_FILE_PARAM; }

  Instance* instance = getInstanceFromFile(opts);
  if(instance == NULL) { return READ_ERROR; }

  Variables* variables = getVariablesFromInstance(instance);

  MainLoop(instance, variables);
  return OK;
}
