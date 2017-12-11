#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <log.h>
#include "definitions.h"


Variables *getVariablesFromInstance(Instance *instance) {
  log_debug("Creating variableSet");
  Variables *variableSet = calloc(1, sizeof(Variables));
  short nodes = instance->nodes;
  short i, j, k;

  variableSet->length = instance->nodes;
  log_debug("creating variables"); 

  variableSet->conflicts = calloc(nodes, sizeof(FCList**));
  variableSet->past = calloc(nodes, sizeof(FCList **));
  variableSet->future = calloc(nodes, sizeof(FCList **));
  variableSet->currentDomain = calloc(nodes, sizeof(Domain**));

  for(i = 0; i < nodes; i++) {
    log_trace("i: %hd", i);
    variableSet->conflicts[i] = calloc(nodes, sizeof(FCList*));
    variableSet->past[i] = calloc(nodes, sizeof(FCList *));
    variableSet->future[i] = calloc(nodes, sizeof(FCList *));
    variableSet->currentDomain[i] = calloc(nodes, sizeof(Domain*));
    for(j = 0; j < nodes; j++) {
      log_trace("j: %hd", j);
      variableSet->conflicts[i][j] = calloc(nodes, sizeof(FCList));
      variableSet->past[i][j] = calloc(nodes, sizeof(FCList ));
      variableSet->future[i][j] = calloc(nodes, sizeof(FCList));
      variableSet->currentDomain[i][j] = calloc(nodes, sizeof(Domain));


      for(k = 0; k < nodes; k++) {
        Domain domain = variableSet->currentDomain[i][j][k];
        domain.length = 2;
        domain.values = calloc(2, sizeof(bool));
        domain.values[0] = 1;
        domain.values[1] = 0;
      }
    }
  }
  return variableSet;
}

short* fcMax(FCList *list) {
  short* maxEl = calloc(3, sizeof(short));
  short l;
  short* current;
  for(l = 0; l < list->length; l++) {
    current = (list->elements)[l];
    if(current[2] > maxEl[2]) {
      maxEl[2] = current[2];
      maxEl[1] = current[1];
      maxEl[0] = current[0];
    } else if(current[2] == maxEl[2]){
      if(current[1] > maxEl[1]) {
        maxEl[1] = current[1];
        maxEl[0] = current[0];
      } else if(current[1] == maxEl[1]) {
        if(current[0] > maxEl[0]) {
          maxEl[0] = current[0];
        }
      }
    }
  }

  return maxEl;
}

void fcPush(FCList* list, short i, short j, short k) {
  list->elements = realloc(list->elements, (list->length + 1) * sizeof(short *));
  list->elements[list->length] = calloc(3, sizeof(short));
  list->elements[list->length][0] = i;
  list->elements[list->length][1] = j;
  list->elements[list->length][2] = k;
  list->length++;
}

short* fcPop(FCList* list) {
  short *elements = list->elements[list->length - 1];
  short *elCpy = calloc(3, sizeof(short));
  memcpy(elCpy, elements, 3*sizeof(short));
  free(elements);
  list->elements = realloc(list->elements, (list->length - 1) * sizeof(short *));
  list->length--;
  return elCpy;
}

FCList* fcRemove(FCList *list, short i, short j, short k) {
  short l, m;
  FCList *newList = calloc(1, sizeof(FCList));
  newList->elements = calloc(list->length - 1, sizeof(short *));
  newList->length = list->length - 1;
  for(l =0, m = 0; l < list->length; l++) {
    short *elements = list->elements[l];
    if(elements[0] == i && elements[1] == j && elements[2] == k){
      newList->elements[m] = calloc(3, sizeof(short));
      memcpy(&(newList->elements[m]), elements, 3*sizeof(short));
      m++;
    }
  }
  return newList;
}

bool fcExists(FCList *list, short i, short j, short k) {
  short l = 0;
  for(l = 0; l < list->length; l++) {
    short *elements = list->elements[l];
    if(elements[0] == i && elements[1] == j && elements[2] == k) {
      return true;
    }
  }
  return false;
}

FCList* fcUnion(FCList* a, FCList* b) {
  FCList *new = calloc(1, sizeof(FCList));
  short i = 0;

  for(i = 0; i < a->length; i++) {
    short *elements = a->elements[i];
    fcPush(new, elements[0], elements[1], elements[2]);
  }

  for(i = 0; i < b->length; i++) {
    short *elements = b->elements[i];
    if(!(fcExists(new, elements[0], elements[1], elements[2]))) {
      fcPush(new, elements[0], elements[1], elements[2]);
    }
  }

  return new;
}

FCList* fcSetDifference(FCList *a, FCList *b) {
  FCList *new = calloc(1, sizeof(FCList));
  short i = 0;
  for(i = 0; i < a->length; i++) {
    short *elements = a->elements[i];
    if(!fcExists(b, elements[0], elements[1], elements[2])) {
      fcPush(new, elements[0], elements[1], elements[2]);
    }
  }

  return new;
}

void fcFree(FCList * list) {
  while(list->length) {
    fcPop(list);
  }
  free(list);
  list = NULL;
}


bool domainEmpty(Domain *domain) {
  return domain->length == 0;
}

void domainRemove(Domain* domain, bool element) {
  int i = 0;
  for(i = 0; i < domain->length; i++) {
    if(domain->values[i] == element) {
      free(&(domain->values[i]));
      if(domain->length == 1) {
        domain->values = NULL;
      } else {
        domain->values = realloc(domain->values, domain->length - 1 * sizeof(bool));
        domain->values[0] = !element;
      }
      break;
    }
  }
}

void domainRestart(Domain *domain) {
  domain->values = realloc(domain->values, 2*sizeof(bool));
  domain->length = 2;
  domain->values[0] = 1;
  domain->values[0] = 0;
}

void domainPush(Domain * domain, bool value) {
  domain->values = realloc(domain->values, (domain->length+1) * sizeof(bool));
  domain->values[domain->length] = value;
  domain->length++;
}

bool domainPop(Domain * domain) {
  bool value = domain->values[domain->length - 1];
  domain->values = realloc(domain->values, (domain->length - 1) * sizeof(bool));
  domain->length--;
  if(domain->length == 0) {
    domain->values = NULL;
  }
  return value;
}

bool domainExists(Domain * domain, bool value){
  short i = 0;
  for(i=0; i < domain->length; i++) {
    if(domain->values[i] == value) {
      return true;
    }
  }

  return false;
}

Domain* domainUnion(Domain * a, Domain * b) {
  Domain *new = calloc(1, sizeof(Domain));
  short i = 0;

  for(i = 0; i < a->length; i++) {
    domainPush(new, a->values[i]);
  }

  for(i = 0; i < b->length; i++) {
    if(!domainExists(new, b->values[i])) {
      domainPush(new, b->values[i]);
    }
  }
  return new;
}

Domain* domainSetDifference(Domain * a, Domain * b) {
  Domain * new = calloc(1, sizeof(Domain));
  short i = 0;
  for(i = 0; i < a->length; i++) {
    if(!domainExists(b, a->values[i])) {
      domainPush(new, a->values[i]);
    }
  }
  return new;
}

void domainFree(Domain *dom) {
  while(dom->length) {
    domainPop(dom);
  }
  free(dom);
  dom = NULL;
}
