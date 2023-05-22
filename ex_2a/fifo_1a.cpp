#include <iomanip>
#include "fifo_1a.h"

fifo_1a::fifo_1a(sc_module_name name, unsigned int fifo_size) : fifo_size(fifo_size) {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// register processes
	SC_METHOD(write_fifo);
	sensitive << clk.pos();
	SC_METHOD(read_fifo);
	sensitive << clk.pos();

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
	full.initialize(false);
	d_out.initialize(255);
	empty.initialize(true);	
	// ####################### UP TO HERE ####################### //
}

void fifo_1a::write_fifo() {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	unsigned char wrData = 0;
	unsigned int loc = wr_ptr.read();
	if (full.read() == false && wr_en.read() == true) {
		wrData = d_in.read(); //store data into var
		fifo_data[loc] = wrData; //write data to fifo buffer
		loc = (loc + 1) % fifo_size; //incr loc and assign to write ptr
		wr_ptr.write(loc);	
		fill_level++;
		cout << std::setw(9) << sc_time_stamp() << ": '" << name()
					<< " Wrote " << (int)wrData << " to FIFO, " << " wr_ptr: " << (int)wr_ptr.read() << ", FIFO fill level: " << (int)fill_level << endl;
	}
	// ####################### UP TO HERE ####################### //
}

void fifo_1a::read_fifo() {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	unsigned char rdData = 0;
	unsigned int loc = rd_ptr.read();
	if (empty.read() == false && rd_en.read() == true) {
		rdData = fifo_data[loc];
       		d_out.write(rdData);
		loc = (loc + 1) % fifo_size;
		rd_ptr.write(loc);
		fill_level--;
		cout << std::setw(9) << sc_time_stamp() << ": '" << name()
					<< "'\tRead " << (int)rdData << " from FIFO, " << "rd_ptr: " << (int)rd_ptr.read() << ", FIFO fill level: " << (int)fill_level << endl;
	}
	// ####################### UP TO HERE ####################### //
}

void fifo_1a::set_flags() {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// set 'full' & 'empty' flags according to fill level
	full.write(fill_level == fifo_size);
	empty.write(fill_level == 0);

	// ####################### UP TO HERE ####################### //
}
