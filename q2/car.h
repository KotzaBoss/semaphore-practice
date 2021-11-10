#ifndef __CAR_H__
#define __CAR_H__

#include "ferry.h"

#include <pthread.h>

#define CAR_ID pthread_self()

typedef struct Car* Car;

Car Car_new(Ferry ferry);
void Car_queue(Car self);
void Car_board_ferry(Car self);
void Car_complete_boarding(Car self);
void* Car_start(void* self_);
pthread_t* Car_tid(Car self);

#endif
