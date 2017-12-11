#ifndef __FORWARDCHECKING_H_
#define __FORWARDCHECKING_H_

#include <stdbool.h>
#include "instance.h"
#include "definitions.h"


void MainLoop(Instance *, Variables*);
short Label(Instance*, Variables*, bool*, short, short, short);
short Unlabel(Instance*, Variables*, bool*, short, short, short);
bool CheckForward(Instance*, Variables*, short, short, short, short, short, short);
void UndoReductions(Instance*, Variables*, short, short, short);
void UpdateCurrentDomain(Instance *, Variables*, short, short, short);
bool check(Instance*, Variables*, short, short, short, short, short, short);

#endif //__FORWARDCHECKING_H_
