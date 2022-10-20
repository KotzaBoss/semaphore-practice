#include "ferry.h"
#include "car.h"
#include "log.h"

#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <sched.h>
#include <assert.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>

Ferry glob_self = NULL;

struct Ferry
{
	pthread_t tid;

	size_t capacity;
	size_t total;

	bool must_leave;
	sem_t leaving;

	sem_t on_board_mut;
	volatile size_t on_board;  // No funny business in Ferry_start

	sem_t queued_mut;
	volatile size_t queued;

	sem_t available;
};

Ferry Ferry_new(size_t capacity)
{
	assert(glob_self == NULL);

	Ferry f = malloc(sizeof(struct Ferry));
	if (!f) return NULL;

	f->capacity = capacity;
	f->total = 0;

	f->must_leave = false;
	f->on_board = 0;
	f->queued = 0;

	// "initialisation" posts on Ferry_start
	sem_init(&f->leaving, 0, 0);
	sem_init(&f->on_board_mut, 0, 0);
	sem_init(&f->queued_mut, 0, 0);
	sem_init(&f->available, 0, 0);
	return glob_self = f;
}

pthread_t* Ferry_tid(Ferry self)
{
	return &self->tid;
}

void Ferry_queue_car(Ferry self)
{
	while (1)
	{
		sem_wait(&self->queued_mut);
			if (self->must_leave)
			// Try to minimise "leak" of cars after the alarm rings
			{
				sem_post(&self->queued_mut);
				sched_yield();
				continue;
			}

			if (self->queued > 0 && self->queued % self->capacity == 0)
			{
				sem_post(&self->queued_mut);
				sched_yield();
				continue;
			}
			++self->queued;

			logf("Car %ld -> %d queued\n", CAR_ID, self->queued);
		sem_post(&self->queued_mut);
		return;
	}
}

void Ferry_complete_boarding(Ferry self)
{
	sem_wait(&self->on_board_mut);
		logf("Car %ld -> %d on board :: total %d\n", CAR_ID, self->on_board + 1, self->total + 1);

		++self->on_board;
		++self->total;
	sem_post(&self->on_board_mut);
}

void Ferry_embark(Ferry self)
{
	log("Ferry embarks ~~~~~~~~~~~ ");
	for (int i = 4; i > 0; --i)
	{
		logf("%d ", i);
		sleep(1);
	}
	log("\n~~~~~~~~~~~ Ferry back to port\n");

	self->on_board = 0;
	self->must_leave = false;
	self->queued = 0;

	alarm(FERRY_WAIT_TIME);
	sem_post(&self->available);
}

// Compete for queing critical zone, lock it until ferry posts its mutex
static
void Ferry_handle_alarm(int sig)
{
	assert(glob_self != NULL && "Global ferry 'glob_self' must be initialised");
	assert(sig == SIGALRM && "Ferry_handle_alarm must be used with SIGALRM only");

	log("!!! Time to leave\n");

	glob_self->must_leave = true;
	sem_wait(&glob_self->queued_mut);
		sem_post(&glob_self->leaving);
		sem_wait(&glob_self->available);
	sem_post(&glob_self->queued_mut);
}

void* Ferry_start(void* self_)
{
	Ferry self = self_;
	signal(SIGALRM, Ferry_handle_alarm);
	alarm(FERRY_WAIT_TIME);

	// pseudo barrier for cars
	sem_post(&self->queued_mut);
	sem_post(&self->on_board_mut);

	while (1)
	{
		sem_wait(&self->leaving);

		while (self->on_board != self->queued)
			// Wait
			;

		Ferry_embark(self);
	}
}
