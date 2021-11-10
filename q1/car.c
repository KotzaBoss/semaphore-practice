#include "car.h"
#include "log.h"
#include "entry.h"
#include "direction.h"

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

struct Car {
	pthread_t tid;
	Entry entry;
};

Car Car_new(Entry entry_pool[2])
{
	Car c = malloc(sizeof(struct Car));
	if (!c) return NULL;

	c->entry = entry_pool[rand() % 2];
	return c;
}

pthread_t* Car_tid(Car self)
{
	return &self->tid;
}

Direction Car_direction(Car self)
{
	return Entry_direction(self->entry);
}

void Car_queue_for_entry(Car self)
{
	Entry_queue_for_entry(self->entry);
}

void Car_queue_for_crossing(Car self)
{
	Entry_queue_for_crossing(self->entry);
}

void Car_traverse_bridge(Car self)
{
	logf("Car %ld %s -> Traversing bridge\n", CAR_ID, Direction_repr(Entry_direction(self->entry)));

	sleep(rand() % 2 + 1);
}

void Car_exit_bridge(Car self)
{
	Entry_dismiss_car(self->entry);
}

void* Car_start(void* self_)
{
	Car self = self_;
	Car_queue_for_entry(self);
	Car_queue_for_crossing(self);
	Car_traverse_bridge(self);
	Car_exit_bridge(self);
	pthread_exit(0);
}
