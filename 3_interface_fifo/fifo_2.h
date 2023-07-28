#ifndef __FIFO_2
#define __FIFO_2

// ############# COMPLETE THE FOLLOWING SECTION ############# //
// include the necessary header files
#include "systemc.h"
#include "fifo_if.h"

// ####################### UP TO HERE ####################### //

class fifo_2: public fifo_if, public sc_module {
private:
	const unsigned int fifo_size;
	unsigned char *fifo_data;
	unsigned int wr_ptr, rd_ptr;
	unsigned int fill_level;

	// declaration of helper function
	void output_fifo_status();

	
public:
	// declaration of the interface functions
	bool read_fifo(unsigned char *data, unsigned int &count);
	bool write_fifo(unsigned char *data, unsigned int &count);

	SC_HAS_PROCESS(fifo_2);

	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// constructor declaration
	fifo_2(sc_module_name name, unsigned int fifo_size);
	// ####################### UP TO HERE ####################### //
};

#endif // __FIFO_2
