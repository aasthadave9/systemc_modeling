#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdio>
#include "fifo_2.h"
//using namespace std;

fifo_2::fifo_2(sc_module_name name, unsigned int fifo_size) : fifo_size(fifo_size) {
	// initialization of member variables/ports
	fifo_data = new unsigned char[fifo_size];
	wr_ptr = 0;
	rd_ptr = 0;
	fill_level = 0;
	
}
   

bool fifo_2::write_fifo(unsigned char *data, unsigned int &count) {
	bool result = false;

	unsigned int len; // amount of data written
	unsigned char *ptr; // pointer where to put data
	ptr = data;
	sc_time delay;
	
	if(fill_level + (int)count > fifo_size) // not enough space for all data
		len = fifo_size - fill_level; 		// none or less data will be written
	else {
		len = count;
		result = true;
	}

	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// complete process
	//memcpy from data[i] to fifo_data[i] byte by byte up until length=len and keep updating write pointer after every byte is copied. Write until write pointer reaches end of fifo. Then wrap back to fifo beginning and continue calling memcpy
	//have delay 100ns after every byte is written to fifo
	////update fill level
	//decrement count after every byte written??
	//generate messages for sim time, data written, write pointer location
	//need logic to prevent simulataneous reads and writes
	
	delay = sc_time(int(len*100), SC_NS);
	wait(delay);
	
	//write whole chunk
	if(wr_ptr+len < fifo_size) memcpy((fifo_data+wr_ptr), (ptr), len);

	//write two chunks separately
	else {
		memcpy((fifo_data+wr_ptr), ptr, (fifo_size - wr_ptr)); 
		memcpy((fifo_data), ptr, (len-(fifo_size - wr_ptr))); 
	
	
	

	} 
	
	/*
	wait(100*len, SC_NS);
	unsigned int locw = wr_ptr;
	for(unsigned int i = 0; i<len; i++) {
		memcpy((fifo_data + locw), (ptr+i), 1);
		locw = (locw + 1)%fifo_size;	
	} 
	fill_level+=len;
	wr_ptr = (wr_ptr + len)%fifo_size; */

	cout << std::setw(9) << sc_time_stamp() << ": '" << name() << "' " << len << " words have been written: 0x " << hex;
	for(unsigned int i=0; i<len; i++) {
		cout << std::setw(2) << std::setfill('0') << (int)(*(ptr+i)) << " ";
	}

	
	cout << dec;
	cout << " " << endl;
	//update wr ptr and fill level in one go
	wr_ptr = (wr_ptr+len)%fifo_size;
	fill_level = fill_level + len;
	count = len;
	// ####################### UP TO HERE ####################### //
	if(fifo_size <= 50)
		output_fifo_status();
	
	return result;
	
}

bool fifo_2::read_fifo(unsigned char *data, unsigned int &count) {
	
	bool result = false;

	unsigned int len;	// amount of data read
	unsigned char *ptr;	// pointer where to put data
	ptr = data;
	sc_time delay;

	if(fill_level < count)	// not enough data to read
		len = fill_level;	// none or less data will be read
	else {
		len = count;
		result = true;
	}

	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// complete process
	
	delay = sc_time(int(len*100), SC_NS);
	wait(delay);
	
	//read whole chunk
	if(rd_ptr+len < fifo_size) memcpy(ptr, (fifo_data+rd_ptr), len);

	//read two chunks separately
	else {
		memcpy(ptr, (fifo_data+rd_ptr), (fifo_size - rd_ptr)); 
		memcpy(ptr, (fifo_data), (len-(fifo_size - rd_ptr))); 
	
		

	} 
	/*
	wait(100*len, SC_NS);
	unsigned int locr = rd_ptr;
	for(unsigned int i=0; i<len; i++) {
		memcpy((ptr+i), (fifo_data + locr), 1);
		locr = (locr + 1) % fifo_size;
	} 
	fill_level-=len;
	rd_ptr = (rd_ptr + len)%fifo_size; */

	cout << std::setw(9) << sc_time_stamp() << ": '" << name() << "' " << len << " words have been read: 0x " << hex;
	for(unsigned int i=0; i<len; i++) {
		cout << std::setw(2) << std::setfill('0') << (int)(*(ptr+i)) << " ";
	}

	cout << dec;
	cout << " " << endl;
	//update rd ptr and fill level in one go
	rd_ptr = (rd_ptr+len)%fifo_size;
	fill_level = fill_level - len;
	count = len;
	// ####################### UP TO HERE ####################### //
	if(fifo_size <= 50)
		output_fifo_status();
	
	return result;
}

// helper function to output content of FIFO
void fifo_2::output_fifo_status() {
	
	cout << "\tCurrent status of '" << name() << "': write address: "
			<< wr_ptr << ", read address: " << rd_ptr
			<< ", fill level: " << fill_level << endl;
	cout << "\t";
	cout << hex; // switch to hexadecimal mode;
	if(fill_level == 0) {
		for(unsigned int i = 0; i < fifo_size; i++)
			cout << "-- ";
	}
	else if(fill_level == fifo_size) {
		for(unsigned int i = 0; i < fifo_size; i++)
			cout << std::setw(2) << std::setfill('0') << (int)*(fifo_data + i)
					<< " ";
	}
	else if(wr_ptr > rd_ptr) {
		for(unsigned int i = 0; i < fifo_size; i++) {
			if((i >= rd_ptr) && (i < wr_ptr)) {
				cout << std::setw(2) << std::setfill('0')
						<< (int)*(fifo_data + i) << " ";
			}
			else
				cout << "-- ";
		}
	}
	else if(wr_ptr < rd_ptr) {
		for(unsigned int i = 0; i < fifo_size; i++) {
			if((i >= rd_ptr) || (i < wr_ptr)) {
				cout << std::setw(2) << std::setfill('0')
						<< (int)*(fifo_data + i) << " ";
			}
			else
				cout << "-- ";
		}
	}
	cout << dec << std::setfill(' ') << endl << endl;
}
