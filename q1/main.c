#include "entry.h"
#include "car.h"
#include "bridge.h"
#include "log.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define DEFAULT_CARS 5

static
int check_args(int argc, char* argv[])
{
	int cars;
	if (argc == 1)  // nr
	{
		cars = DEFAULT_CARS;
	}
	else if (argc == 3)  // nr -c 50
	{
		if (strcmp("-c", argv[1]))
		{
			log("Expected second argument to be '-c'\n");
			exit(EXIT_FAILURE);
		}
		if ((cars = atoi(argv[2])) <= 0)
		{
			log("Bad argument for '-c' expected number >1\n");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		log("Usage nr -c [positive number]\n");
		exit(EXIT_FAILURE);
	}
	return cars;
}

int main(int argc, char* argv[])
{
	const int cars = check_args(argc, argv);

	srand(666);

	Bridge bridge = Bridge_new(3);
	Car car_pool[cars];
	Entry entries[2] = { Entry_new(WEST, bridge), Entry_new(EAST, bridge) };

	for (int i = 0; i < cars; ++i)
	{
		Car car = Car_new(entries);
		if (!car)
		{
			logf("Not enough memory to instantiate more than %d cars. Exiting.\n", i + 1);
			return EXIT_FAILURE;
		}

		car_pool[i] = car;
	}


	logf("Total Cars = %d\n", cars);
	int east = 0, west = 0;
	for (int i = 0; i < cars; ++i)
	{
		switch (Car_direction(car_pool[i]))
		{
			case EAST:
				++east;
				break;
			case WEST:
				++west;
				break;
		}
	}
	logf("EAST = %d\nWEST = %d\n", east, west);
	log("===============================\n");

	for (int i = 0; i < cars; ++i)
	{
		sleep(rand() % 2 + 1);
		pthread_create(Car_tid(car_pool[i]), NULL, Car_start, car_pool[i]);
	}

	for (int i = 0; i < cars; ++i)
	{
		pthread_join(*Car_tid(car_pool[i]), NULL);
	}

	return EXIT_SUCCESS;
}





