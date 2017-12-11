#include <stdlib.h>
#include <stdbool.h>
#include <log.h>
#include "forwardchecking.h"
#include "instance.h"

#define YES 1
#define NO 0
#define UNK -1

void MainLoop(Instance *instance, Variables *variables) {
  bool consistent = true;
  char solution = UNK;
  short i = 0, j = 0, k = 0;
  short max_fields = instance->nodes;
  log_trace("i, j, k = %hd,%hd,%hd", i, j, k);
  while(solution == UNK) {
    if(consistent) {
      i = Label(instance, variables, &consistent, i, j, k);
      if(i == max_fields && j >= 0) { i=0; j++; }
      if(j == max_fields && k >= 0) { j=0; k++; }
    } else {
      i = Unlabel(instance, variables, &consistent, i, j, k);
      if(i == -1 && j >= 0) { i = max_fields - 1; j--; }
      if(j == -1 && k >= 0) { j = max_fields - 1; k--; } 
    }
    log_trace("i, j, k = %hd,%hd,%hd", i, j, k);

    if(k == max_fields) { solution = YES; }
    else if(k == -1) { solution = NO; }
  }
  return;
}

//A siempre va a estar antes de B.
bool check(Instance *instance, Variables *variables,
    short ia, short ja, short ka,
    short ib, short jb, short kb) {


  bool valA = variables->currentDomain[ia][ja][ka].values[0];
  bool valB = variables->currentDomain[ib][jb][kb].values[0];
  short max_size = instance->nodes - 1;
  //Restricción 1, 
  if(ia == 0 && jb == max_size && ka == kb) {
    return (valA == valB);
  }

  // Restricción 2 y 4
  if(ia == ib) {
    return !((valA && valB));
  }

  //Restricción 3
  if(ka < kb && ja == max_size && ib == 0) {
    return (valA == false && valB == true);
  }

  //Restricción 5, 6 y Riesgo.
  if(ja == ib && ka == kb) {
    float currentRisk = variables->risk[ja][ka];
    float distance = getNodeDistance(&(instance->positions[ja]), &(instance->positions[ib]));
    float cash = instance->demand[ib];

    float newRisk = (valA && valB) ? currentRisk + (cash * distance) : 0;
    return (valA == valB && newRisk < instance->max_risk);
  }

  return true;
}

bool checkForward(Instance *instance, Variables *variables, 
    short ia, short ja, short ka,
    short ib, short jb, short kb) {

  char i = 0;
  Domain *bDomain = &(variables->currentDomain[ib][jb][kb]);
  Domain *reductions = &(variables->reductions[ib][jb][kb]);

  for(i = 0; i < bDomain->length ; i++) {
    if(!check(instance, variables ,
          ia, ja, ka,
          ib, jb, kb)
      ) {
      domainPush(reductions, bDomain->values[0]);
    }
  }

  if(!domainEmpty(reductions)) {
    Domain *difference = domainSetDifference(bDomain, reductions);
    Domain *oldBDomain = bDomain;
    bDomain = difference;
    free(oldBDomain->values);
    free(oldBDomain);
    fcPush(&(variables->future[ia][ja][ka]), ib, jb, kb);
    fcPush(&(variables->past[ib][jb][kb]), ia, ja, ka);
  }

  return !domainEmpty(bDomain);
}


void UndoReductions(Instance* instance, Variables *variables,
    short i, short j, short k) {
  FCList *future = &(variables->future[i][j][k]);
  while(future->length > 0) {
    short *j = fcPop(future);
    Domain *reduction = &(variables->reductions[j[0]][j[1]][j[2]]);
    Domain *domain = &(variables->currentDomain[j[0]][j[1]][j[2]]);
    Domain *oldDomain = domain;
    domain = domainUnion(oldDomain, reduction);
    fcPop(&(variables->past[j[0]][j[1]][j[2]]));
  }
}

void UpdateCurrentDomain(Instance *instance, Variables *variables,
    short i, short j, short k) {
  short w;
  Domain * dom = &(variables->currentDomain[i][j][k]);
  Dom * oldDom = dom;
  Domain * reduction = &(variables->reductions[i][j][k]);
  domainRestart(dom);
  Domain *newDom = domainSetDifference(dom, reduction);
  dom = newDom;
  domainFree(oldDom);
}


short Label(Instance* instance, Variables* variables, 
    bool *consistent, short i, short j, short k) {
  *consistent = false;

  Domain * aDomain = &(variables->currentDomain[i][j][k]);
  short ib = i+1, jb = j, kb = k;
  short w = 0;
  for(w = 0; w < aDomain->length; w++) {
    *consistent = true;
    bool value = aDomain->values[w];
    while(kb < instance->nodes) {
      if(ib == instance->nodes) { jb++; ib = 0; }
      if(jb == instance->nodes) { kb++; jb = 0; }
      *consistent = checkForward(instance, variables, 
          i, j, k, 
          ib, jb, kb);
      if(!(*consistent)) {
        domainRemove(aDomain, value);
        UndoReductions(instance, variables, i, j, k);
        FCList *conflictSet = &(variables->conflicts[i][j][k]);
        FCList *newConflictSet = fcUnion(conflictSet, &(variables->past[i][j][k]));
        variables->conflicts[i][j][k] = *newConflictSet;
        fcFree(conflictSet);
      }
    }
  }
  return (consistent) ? i+1 : i;
}

short Unlabel(Instance* instance, Variables* variables,
    bool* consistent, short i, short j, short k) {

}

