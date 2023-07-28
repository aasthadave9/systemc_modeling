#include "consumer.h"
#include "producer.h"
#include "fifo_1b.h"

int sc_main(int argc, char *argv[]) {
	// the following instruction generates a clock signal with clock named
	// "clock" with a period of 100 ns, a duty cycle of 50%, and a falling edge
	// after 50 ns
	sc_clock clk("clock", 100, SC_NS, 0.5, 50, SC_NS, false);

	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// fill in the required commands to instantiate and connect producer, fifo,
	// and consumer
	sc_signal<unsigned char> data_in, data_out;
	sc_signal<bool> wr_en, full, rd_en, valid;
	
	producer p1("p1");
	fifo_1b f1("f1", 5);
	consumer c1("c1");

	p1.clk(clk);
	p1.d_out(data_out);
	p1.wr_en(wr_en);
	p1.full(full);
	f1.clk(clk);
	f1.d_in(data_out);
	f1.wr_en(wr_en);
	f1.rd_en(rd_en);
	f1.full(full);
	f1.valid(valid);
	f1.d_out(data_in);
	c1.clk(clk);
	c1.d_in(data_in);
	c1.rd_en(rd_en);
	c1.valid(valid);

	// ####################### UP TO HERE ####################### //

	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// fill in code to generate traces that can be used to observe the
	// functionality of the model with the waveform viewer gtkwave
	sc_trace_file *tf = sc_create_vcd_trace_file("traces");
	sc_trace(tf, clk, "clk");
	//sc_trace(tf, c1.data_valid, "data_valid");
	sc_trace(tf, c1.fetch, "fetch");
	sc_trace(tf, f1.fill_level, "fifo_fill_level");
	sc_trace(tf, f1.full, "fifo_full");
	sc_trace(tf, f1.d_in, "fifo_in");
	sc_trace(tf, f1.d_out, "fifo_out");
	sc_trace(tf, f1.valid, "fifo_valid");
	sc_trace(tf, p1.send, "send");
	sc_trace(tf, f1.rd_en, "rd_en");
	sc_trace(tf, f1.wr_en, "wr_en");

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
