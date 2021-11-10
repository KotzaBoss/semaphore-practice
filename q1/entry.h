#ifndef __ENTRY_H__
#define __ENTRY_H__

#include "direction.h"
#include "bridge.h"

typedef struct Entry* Entry;

#define R_CAR_WITH_DIR logf("Car -> { %ld, %s }\n", pthread_self(), Direction_repr(self->direction))
#define ENTRY_DIR Direction_repr(self->direction)

Entry Entry_new(Direction dir, Bridge bri);
Direction Entry_direction(Entry self);
void Entry_queue_for_entry(Entry self);
void Entry_queue_for_crossing(Entry self);
void Entry_dismiss_car(Entry self);

#endif
