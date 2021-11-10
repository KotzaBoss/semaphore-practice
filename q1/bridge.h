#ifndef __BRIDGE_H__
#define __BRIDGE_H__

#include <semaphore.h>

typedef struct Bridge* Bridge;

Bridge Bridge_new(unsigned int capacity);
sem_t* Bridge_capacity(Bridge self);
sem_t* Bridge_mutex(Bridge self);
int Bridge_cars_on_bridge(Bridge self);

#endif
