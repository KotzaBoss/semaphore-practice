#ifndef __DIRECTION_H__
#define __DIRECTION_H__

typedef enum {
	EAST = 0,
	WEST
} Direction;

#define Direction_repr(dir) (dir == 0 ? "EAST" : "WEST")

#endif
