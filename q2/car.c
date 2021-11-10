#include "car.h"
#include "ferry.h"

#include <stdlib.h>
#include <unistd.h>

struct Car
{
	pthread_t tid;
	Ferry ferry;
};

Car Car_new(Ferry ferry)
{
	Car c = malloc(sizeof(struct Car));
	if (!c) return NULL;

	c->ferry = ferry;
	return c;
}

void Car_queue(Car self)
{
	Ferry_queue_car(self->ferry);
}

void Car_board_ferry(Car self)
{
	(void)self;
	sleep(1);
}

void Car_complete_boarding(Car self)
{
	Ferry_complete_boarding(self->ferry);
}

void* Car_start(void* self_)
{
	Car self = self_;
	Car_queue(self);
	Car_board_ferry(self);
	Car_complete_boarding(self);
	pthread_exit(0);
}

pthread_t* Car_tid(Car self)
{
	return &self->tid;
}
