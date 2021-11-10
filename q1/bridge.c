#include "bridge.h"
#include <semaphore.h>
#include <stdlib.h>

struct Bridge {
	int _capacity;
	sem_t capacity;  // make sure max 3 on bridge
	sem_t mutex; // last car has exited the bridge
};

Bridge Bridge_new(unsigned int capacity)
{
	Bridge b = malloc(sizeof(struct Bridge));
	if (!b) return NULL;

	b->_capacity = capacity;
	sem_init(&b->capacity, 0, capacity);
	sem_init(&b->mutex, 0, 1);
	return b;
}

sem_t* Bridge_capacity(Bridge self)
{
	return &self->capacity;
}

sem_t* Bridge_mutex(Bridge self)
{
	return &self->mutex;
}

int Bridge_cars_on_bridge(Bridge self)
{
	int sval;
	sem_getvalue(&self->capacity, &sval);
	return self->_capacity - sval;
}
