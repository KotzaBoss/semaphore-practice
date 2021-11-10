#include "entry.h"
#include "car.h"
#include "bridge.h"
#include "direction.h"
#include "log.h"

#include <semaphore.h>
#include <stdlib.h>

struct Entry {
	Direction direction;
	Bridge bridge;

	sem_t rc_mut;
	int rc_cars;  // Keep track of first and last of car "batches"
};


Entry Entry_new(Direction dir, Bridge bri)
{
	Entry e = malloc(sizeof(struct Entry));
	if (!e) return NULL;

	e->direction = dir;
	e->bridge = bri;
	sem_init(&e->rc_mut, 0, 1);
	e->rc_cars = 0;
	return e;
}

Direction Entry_direction(Entry self)
{
	return self->direction;
}

void Entry_queue_for_entry(Entry self)
{
	sem_wait(&self->rc_mut);
		logf("Car %ld %s -> Queueing up\n", CAR_ID, ENTRY_DIR);

		++self->rc_cars;
		if (self->rc_cars == 1)  // Relative first waits for the bridge
		{
			logf("Car %ld %s -> WAIT LAST_DONE   : First relative waiting for the last to be done\n", CAR_ID, ENTRY_DIR);

			sem_wait(Bridge_mutex(self->bridge));

			logf("Car %ld %s -> WAIT LAST_DONE OK: First relative car is Ok\n", CAR_ID, ENTRY_DIR);
		}
	sem_post(&self->rc_mut);
}

void Entry_queue_for_crossing(Entry self)
{
	logf("Car %ld %s -> WAIT CAPACITY   : %d cars on road \n", CAR_ID, ENTRY_DIR, Bridge_cars_on_bridge(self->bridge));

	sem_wait(Bridge_capacity(self->bridge));

	logf("Car %ld %s -> WAIT CAPACITY OK: %d cars on the road\n", CAR_ID, ENTRY_DIR, Bridge_cars_on_bridge(self->bridge));
}

void Entry_dismiss_car(Entry self)
{
	sem_wait(&self->rc_mut);

		--self->rc_cars;
		sem_post(Bridge_capacity(self->bridge));

		logf("Car %ld %s -> POST CAPACITY: Car dismissed, %d cars on road\n",
				CAR_ID, ENTRY_DIR, Bridge_cars_on_bridge(self->bridge));

		if (self->rc_cars == 0)  // Relative last notifies of his exit
		{
			sem_post(Bridge_mutex(self->bridge));
		}
	sem_post(&self->rc_mut);
}
