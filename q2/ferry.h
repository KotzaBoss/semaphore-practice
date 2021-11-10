#ifndef __FERRY_H__
#define __FERRY_H__

#include <pthread.h>

#define FERRY_WAIT_TIME (1)

typedef struct Ferry* Ferry;

Ferry Ferry_new(size_t capacity);
pthread_t* Ferry_tid(Ferry self);
void Ferry_queue_car(Ferry self);
void Ferry_complete_boarding(Ferry self);
void Ferry_embark(Ferry self);
void* Ferry_start(void* self_);

#endif
