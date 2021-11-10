#include "log.h"
#include "ferry.h"
#include "car.h"

#include <string.h>
#include <stdlib.h>

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
	Ferry ferry = Ferry_new(20);
	const int cars = check_args(argc, argv);
	Car cpool[cars];

	for(int i = 0; i < cars; ++i)
	{
		Car car = Car_new(ferry);
		if (!car) return EXIT_FAILURE;

		cpool[i] = car;
	}

	pthread_create(Ferry_tid(ferry), NULL, Ferry_start, ferry);

	for(int i = 0; i < cars; ++i)
	{
		pthread_create(Car_tid(cpool[i]), NULL, Car_start, cpool[i]);
	}

	for(int i = 0; i < cars; ++i)
	{
		pthread_join(*Car_tid(cpool[i]), NULL);
	}

	pthread_join(*Ferry_tid(ferry), NULL);
}




