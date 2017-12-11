#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include "args.h"

typedef struct NodePos {
  float x;
  float y;
} NodePos;

float getNodeDistance(NodePos*, NodePos*);

typedef struct Instance {
  short nodes;
  float  max_risk;
  float *demand;

  NodePos *positions;
} Instance; 

Instance* getInstanceFromFile(Options*);

#endif //_INSTANCE_H_
