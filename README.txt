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

The abstractions for this part as simply the "ferry" and the "car". The cars race to queue and board the ferry
where as the ferry sets a timer and observes the boarding process.
There are 4 stages for this problem

1.	A car attempts to queue itself on the ferry. Should the ferry need to leave or its the 21st car to queue it
	yields control (does not increment the queueing counter) before retrying.
2.	A queued car _will_ end up boarding by acquiring the boarding critical region and incrementing the on_board counter.
3.	When the alarm rings and the ferry must leave then a handler attempts to lock the queueing critical region, post to
	the ferry that it should be ready to leave and wait until the ferry is back to port. The cars aid this race by yielding
	control as mentioned at 2.
4.	The ferry begins to busy wait until the on board cars are equal to the queued cars at which point it leaves the port for
	a few seconds, resets the counters and posts that its back to port, allowing the alarm handler to release control of the
	queueing critical region and allow cars to board again.
