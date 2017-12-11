#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <log.h>

#include "instance.h"

#define QUOTE(x) #x

#define RETURN_NULL_WHEN(X) \
  log_trace("Test for return: " QUOTE(X) "=%d", X);\
  if(X) { \
    log_error("error while reading instance from file");\
    return NULL;\
  } \

#define CLOSE_WHEN(A, B) \
  if(A) { fclose(B); } \

#define FREE_WHEN(X, V) \
  if(X) { free(V); V = NULL; } \

/**
 * Gets the euclidean distance from one node to another one.
 * @param NodePos* a - a Node position
 * @param NodePos* b - another Node position
 * @returns float - The distance between node a and b. 
 */
float getNodeDistance(NodePos* a, NodePos* b) {
  return (sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2)));
}


/**
 * Gets an instance configuration from a file.
 * @param Options* opts - an Options struct, with data retrieved from CLI.
 * @returns Instance* - the parsed instance from the specified file, or NULL if file wasn't
 * successfully parsed.
 */
Instance* getInstanceFromFile(Options *opts) {
  unsigned short i;
  int error = 0;
  Instance* instance = NULL;
  FILE* input = fopen(opts->file, "r");
  RETURN_NULL_WHEN(input == NULL);
  
  instance = calloc(1, sizeof(Instance));
  log_debug("Reading amount of nodes");
  FREE_WHEN(fscanf(input, "%hd", &(instance->nodes)) < 1, instance);
  CLOSE_WHEN(instance == NULL, input);
  RETURN_NULL_WHEN(instance == NULL);
  instance->nodes++;
  log_debug("Amount of nodes is %hu (%hu and 1 auxiliary)", 
            instance->nodes, instance->nodes -1 );
  log_debug("Reading max risk amount");
  FREE_WHEN(fscanf(input, "%f",&(instance->max_risk)) < 1, instance);
  CLOSE_WHEN(instance == NULL, input);
  RETURN_NULL_WHEN(instance == NULL);
  log_debug("Maximum risk allowed is %f", instance->max_risk);

  instance->demand = calloc(instance->nodes, sizeof(double));

  log_debug("Reading demand for nodes");
  for(i=0; i<instance->nodes - 1; i++) {
    log_debug("Reading demand for node %h", i);
    FREE_WHEN(fscanf(input, "%f", &(instance->demand)[i]) < 1, instance);
    CLOSE_WHEN(instance == NULL, input);
    RETURN_NULL_WHEN(instance == NULL);
  }

  memcpy(&(instance->demand[instance->nodes - 1]),
         &(instance->demand[0]), sizeof(float));

  log_debug("Reading node positions");
  instance->positions = calloc(instance->nodes, sizeof(NodePos));

  for(i=0; i<instance->nodes - 1 && !error; i++) {
    log_debug("Reading position for node %hd", i);
    NodePos* position = &(instance->positions[i]);
    error = (fscanf(input, "%f %f", &(position->x), &(position->y)) < 2);
    FREE_WHEN(error, instance->positions);
    FREE_WHEN(error, instance);
    CLOSE_WHEN(error, input);
    RETURN_NULL_WHEN(error);
  }

  memcpy(&(instance->positions[instance->nodes - 1]),
      &(instance->positions[0]),
      sizeof(NodePos));

  log_info("Data from instance input:");
  log_info("Number of nodes: %hu", instance->nodes);
  log_info("Maximum risk: %lf", instance->max_risk);
  for(i=0;i<instance->nodes;i++) {
    NodePos *position = &(instance->positions[i]);

    log_info("Node %hd, position: (%f, %f), demand: %f",
              i, position->x, position->y, instance->demand[i]);
  }
  fclose(input);
  return instance; 
}
