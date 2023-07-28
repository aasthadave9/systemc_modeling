#include "systemc.h"
#include "stimulus.h"
#include "counter.h"
#include "bcd_decoder.h"

int sc_main(int argc, char *argv[]) {

	sc_signal<bool> clock, reset_n;
	sc_signal<unsigned short int> count_val;
	sc_signal<char> v_hi, v_lo;
	
	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	sc_set_time_resolution(1, SC_NS);

	//module instantiation
	stimulus st1("st1");
	counter c1("c1");
	bcd_decoder bd1("bd1");
	
	//make connections
	st1.clk(clock);
	st1.rst_n(reset_n);
	c1.clk(clock);
	c1.rst_n(reset_n);
	c1.cnt(count_val);
	bd1.val(count_val);
	bd1.hi(v_hi);
	bd1.lo(v_lo);	

	//generate trace file, define traces
	sc_trace_file *tf = sc_create_vcd_trace_file("traces");
	sc_trace(tf, st1.clk, "clk");
	sc_trace(tf, st1.rst_n, "rst_n");
	sc_trace(tf, c1.cnt, "count_val");
	sc_trace(tf, bd1.hi, "v_hi");
	sc_trace(tf, bd1.lo, "v_lo");


	// ####################### UP TO HERE ####################### //

	int n_cycles;
	if(argc != 2) {
		cout << "default n_cycles = 2000" << endl;
		n_cycles = 2000;
	}
	else {
		n_cycles = atoi(argv[1]);
		cout << "n_cycles = " << n_cycles << endl;
	}

	sc_start(n_cycles, SC_NS);

	sc_close_vcd_trace_file(tf);

	return 0;
}
