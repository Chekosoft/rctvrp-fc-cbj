#ifndef __DEFINITIONS_H_
#define __DEFINITIONS_H_

#include <stdbool.h>
#include "instance.h"

typedef struct FCList {
  short length;
  short **elements; //[i][0, 1, 2]
} FCList;

short* fcMax(FCList*);
void fcPush(FCList*, short, short, short);
short* fcPop(FCList*);
FCList* fcRemove(FCList*, short, short, short);
bool fcExists(FCList*, short, short, short);
FCList* fcSetDifference(FCList*, FCList*);
FCList* fcUnion(FCList*, FCList*);
void fcFree(FCList*);


typedef struct Domain {
  char length;
  bool *values;
} Domain;


bool domainEmpty(Domain *);
void domainRestart(Domain *);
void domainRemove(Domain *, bool);
void domainPush(Domain *, bool);
bool domainPop(Domain *);
bool domainExists(Domain *, bool);
Domain* domainUnion(Domain *, Domain *);
Domain* domainSetDifference(Domain *, Domain *);
void domainFree(Domain *);


typedef struct Solution {
  int nodes;
  int *order;
  
  double distance;
  double risk;
} Solution;


// Trio i, j, k
// i: partida
// j: llegada
// k: ruta

typedef struct Variables {
  int length;
  
  FCList ***conflicts;
  FCList ***past;
  FCList ***future;
  Domain ***reductions;
  Domain ***currentDomain;
  
  float **risk;
  float **cash;

  Solution* currentSolution;
} Variables;


Variables* getVariablesFromInstance(Instance *);

#endif //__DEFINITIONS_H_
