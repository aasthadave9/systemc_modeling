#include "consumer.h"
#include "producer.h"
#include "fifo_1a.h"

int sc_main(int argc, char *argv[]) {
	// the following instruction generates a clock signal with clock named
	// "clock" with a period of 100 ns, a duty cycle of 50%, and a falling edge
	// after 50 ns
	sc_clock clk("clock", 100, SC_NS, 0.5, 50, SC_NS, false);

	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// fill in the required commands to instantiate and connect producer, fifo,
	// and consumer

	// ####################### UP TO HERE ####################### //

	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// fill in code to generate traces that can be used to observe the
	// functionality of the model with the waveform viewer gtkwave

	// ####################### UP TO HERE ####################### //

	sc_time sim_dur = sc_time(5000, SC_NS);
	if(argc != 2) {
		cout << "Default simulation time = " << sim_dur << endl;
	}
	else {
		sim_dur = sc_time(atoi(argv[1]), SC_NS);
		cout << "Simulation time = " << sim_dur << endl;
	}

	// start simulation
	sc_start(sim_dur);

	sc_close_vcd_trace_file(tf);

	return 0;
}
