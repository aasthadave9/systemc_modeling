#ifndef __CONSUMER
#define __CONSUMER

// ############# COMPLETE THE FOLLOWING SECTION ############# //
// include the necessary header files
#include "systemc.h"
#include "fifo_if.h"
// ####################### UP TO HERE ####################### //

SC_MODULE(consumer) {

public:
	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// declare the port of the module
	sc_port<fifo_if> consum2fifo_port;
	// ####################### UP TO HERE ####################### //

private:
	// event for communication between consumer() and fetch_trigger()
	sc_event fetch_event;

private:
	// process declaration
	void consumer_proc();
	void fetch_trigger();

public:
	// constructor
	SC_CTOR(consumer) {
		// process registration
		SC_THREAD(consumer_proc);
		SC_THREAD(fetch_trigger);
	}
	
	
};

#endif // __CONSUMER
