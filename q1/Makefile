CC=gcc
CFLAGS=-Wall -Wextra -pedantic -pthread -Wno-builtin-declaration-mismatch
SRC=car.c entry.c bridge.c log.c main.c
DEP=car.h entry.h bridge.h direction.h log.h


default: objs
	$(CC) $(CFLAGS) $(SRC) -o ../nr

objs:
	mkdir -p obj
	$(CC) $(CFLAGS) $(SRC) -c
	mv *.o obj

clean:
	rm -f obj/*.o ../nr
