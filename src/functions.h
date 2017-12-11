#ifndef __FUNCTIONS_H_
#define __FUNCTIONS_H_

#include <stdbool.h>

typedef struct Routes {
  unsigned int **routes;
} Routes;

typedef int(*RestrictionFunction)(Instance*, Routes);
typedef double(*ObjectiveFunction)(Instance*, Routes);

double minmizeDistance(Instance*, Routes);

int validateDispatchCycle(Instance*, Routes);
int validateDispatchStart(Instance*, Routes);
int validateRouteSuccession(Instance*, Routes);
int validateNoInnerCycled(Instance*, Routes);
int validateNodeArrivalBeforeDeparture(Instance*, Routes);
int validateMoneySum(Instance*, Routes);
int validateRisk(Instance*, Routes);

#endif
