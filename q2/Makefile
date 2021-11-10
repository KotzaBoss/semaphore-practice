CC=gcc
CFLAGS=-Wall -Wextra -pedantic -pthread -Wno-builtin-declaration-mismatch
SRC=log.c ferry.c car.c main.c
DEP=log.h ferry.h car.h


default: objs
	$(CC) $(CFLAGS) obj/*.o -o ../ferry

objs:
	mkdir -p obj
	$(CC) $(CFLAGS) $(SRC) -c
	mv *.o obj

clean:
	rm -f obj/*.o ../ferry
