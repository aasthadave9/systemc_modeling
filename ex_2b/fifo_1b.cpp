#include <iomanip>
#include "fifo_1b.h"

fifo_1b::fifo_1b(sc_module_name name, unsigned int fifo_size) : fifo_size(fifo_size) {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// register process
	SC_METHOD(read_write_fifo);
	sensitive << clk.pos();
	// ####################### UP TO HERE ####################### //

	// initialization of member variables/ports
	fifo_data = new unsigned char[fifo_size];
	rd_ptr.write(0);
	wr_ptr.write(0);
	fill_level = 0;

	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// initialize output ports
	d_out.initialize(-1);
	full.initialize(false);
	valid.initialize(false);
	// ####################### UP TO HERE ####################### //
}


void fifo_1b::read_write_fifo() {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //

	//define flags
	bool write_through=0, write=0, read=0;
		
	//write through flag
	if (fill_level == 0 && wr_en.read()==true) {
		write_through=1;
		write=0;
		fill_level++;
	}
	//write flag
	else if (!full.read() && wr_en.read()==true) {
		write=1;
		write_through=0;
		fill_level++;
	}

	else {
		write=0; write_through=0;
	}

	//read flag
	if (rd_en.read()==true && fill_level > 1 ) {
		read=1; fill_level--;
	}
	else if (rd_en.read()==true && fill_level == 1 && wr_en.read()==false) {
		read=0; fill_level--;
	}
	else {
		read=0;
	}


	if (fill_level <= 0) {
		valid.write(false);
	}
	else {
		valid.write(true);
	}

	if (fill_level == fifo_size+1) {
	       	full.write(true); //output of fifo is part of fifo storage
	}
	else {
	      	full.write(false);
	}
	

	//write through execution
	if (write_through) {
		unsigned char wrThData;
		wrThData = d_in.read();
		d_out.write(wrThData);
		//cout << std::setw(9) << sc_time_stamp() << ": '" << name()
		//			<< " Wrote through " << (int)wrThData << " to d_out, " << " wr_ptr: " << (int)wr_ptr.read() << ", FIFO fill level: " << (int)fill_level << endl;

	}
	//write execution
	if (write) {
		unsigned char wrData;
		unsigned int locw = wr_ptr.read();
		wrData = d_in.read();
		fifo_data[locw] = wrData;
		locw = (locw+1)%fifo_size;
		wr_ptr.write(locw);
		//cout << std::setw(9) << sc_time_stamp() << ": '" << name()
		//			<< " Wrote " << (int)wrData << " to FIFO, " << " wr_ptr: " << (int)wr_ptr.read() << ", FIFO fill level: " << (int)fill_level << endl;

	}
	if(read) {
		unsigned char rdData;
		unsigned int locr = rd_ptr.read();
		rdData = fifo_data[locr];
		d_out.write(rdData);
		locr = (locr+1)%fifo_size;
		rd_ptr.write(locr);
		//cout << std::setw(9) << sc_time_stamp() << ": '" << name()
		//			<< " Read " << (int)rdData << " from FIFO, " << "rd_ptr: " << (int)rd_ptr.read() << ", FIFO fill level: " << (int)fill_level << endl;

	}

}




/*   //without flags - simulation breaks needs debug

	//intermediate data storage
	unsigned char wrThData;
	unsigned char wrData;
	unsigned char rdData;
	//pointer locations
	unsigned int locr = rd_ptr.read();
	unsigned int locw = wr_ptr.read();

	if (fill_level == fifo_size+1) full.write(true);
	else full.write(false);
	if (fill_level == 0) valid.write(false);
	else valid.write(true);

	//write through
	if (fill_level == 0 && wr_en.read() == true) {     
		wrThData = d_in.read();
		d_out.write(wrThData);
		fill_level++;
		cout << std::setw(9) << sc_time_stamp() << ": '" << name()
					<< " Wrote through " << (int)wrThData << " to d_out, " << " wr_ptr: " << (int)wr_ptr.read() << ", FIFO fill level: " << (int)fill_level << endl;
	}

	//write
	else if (wr_en.read() == true && full.read() == false) { 
		wrData = d_in.read();
		fifo_data[locw] = wrData;
		locw = (locw+1)%fifo_size;
		wr_ptr.write(locw);
		fill_level++;
		cout << std::setw(9) << sc_time_stamp() << ": '" << name()
					<< " Wrote " << (int)wrData << " to FIFO, " << " wr_ptr: " << (int)wr_ptr.read() << ", FIFO fill level: " << (int)fill_level << endl;
	}

	
	//read
	if (rd_en.read() == true && fill_level > 1) {  
		rdData = fifo_data[locr];
		d_out.write(rdData);
		locr = (locr+1)%fifo_size;
		fill_level--;
		cout << std::setw(9) << sc_time_stamp() << ": '" << name()
					<< "'\tRead " << (int)rdData << " from FIFO, " << "rd_ptr: " << (int)rd_ptr.read() << ", FIFO fill level: " << (int)fill_level << endl;
	}
	
	//data is already sitting at d_out
	else if(rd_en.read() == true && fill_level == 1) fill_level--; 	



	}


*/

	// ####################### UP TO HERE ####################### //
	
