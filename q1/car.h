#ifndef __CAR_H__
#define __CAR_H__

#include "entry.h"
#include "direction.h"

#include <pthread.h>

typedef struct Car* Car;

#define CAR_ID pthread_self()

Car Car_new(Entry entry_pool[2]);
pthread_t* Car_tid(Car self);
Direction Car_direction(Car self);
void Car_queue_for_entry(Car self);
void Car_queue_for_crossing(Car self);
void Car_traverse_bridge(Car self);
void Car_exit_bridge(Car self);
void* Car_start(void* self_);

#endif
