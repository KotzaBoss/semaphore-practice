# Narrow road

The abstractions used for this solution are the "entry", the "bridge" and the "car". The main actors in 
the program are the cars, each one of which is a thread sharing one of two entries and the bridge.
There are 4 stages for this problem

1.	When a car wants to cross the bridge it first has to make sure that it can acquire the bridge. 
	During this process, reference counting happens to know whether a car requesting the bridge is 
	the first for this entry point. This solution is inspired by the "Reader Writer problem" described in the book 
	and as such it shares the same problem of potentially halting the other entry for as long cars are queueing up.
2.	Once the bridge is acquired the cars need to make sure that the bridge doesnt collapse by waiting
	on the capacity semaphore, which is initialised to 3.
3.	Crossing the bridge is just sleeping for a random amount of time
4.	Once a car is done traversing the bridge it posts its exit on the capacity semaphore
	and decrements the reference counter. If the car is the last on the bridge then it
	releases the bridge.

# Ferry