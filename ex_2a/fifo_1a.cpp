#include <iomanip>
#include "fifo_1a.h"

fifo_1a::fifo_1a(sc_module_name name, unsigned int fifo_size) : fifo_size(fifo_size) {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// register processes

	// ####################### UP TO HERE ####################### //

	SC_METHOD(set_flags);
	sensitive << rd_ptr << wr_ptr;

	// initialization of member variables/ports
	fifo_data = new unsigned char[fifo_size];
	rd_ptr.write(0);
	wr_ptr.write(0);
	fill_level = 0;

	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// initialize output ports

	// ####################### UP TO HERE ####################### //
}

void fifo_1a::write_fifo() {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //

	// ####################### UP TO HERE ####################### //
}

void fifo_1a::read_fifo() {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //

	// ####################### UP TO HERE ####################### //
}

void fifo_1a::set_flags() {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// set 'full' & 'empty' flags according to fill level

	// ####################### UP TO HERE ####################### //
}
