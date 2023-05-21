#include <iomanip>
#include "fifo_1b.h"

fifo_1b::fifo_1b(sc_module_name name, unsigned int fifo_size) : fifo_size(fifo_size) {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// register process

	// ####################### UP TO HERE ####################### //

	// initialization of member variables/ports
	fifo_data = new unsigned char[fifo_size];
	rd_ptr.write(0);
	wr_ptr.write(0);
	fill_level = 0;

	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// initialize output ports

	// ####################### UP TO HERE ####################### //
}

void fifo_1b::read_write_fifo() {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //

	// ####################### UP TO HERE ####################### //
}
