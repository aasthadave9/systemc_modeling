#include <iostream>
#include <iomanip>
#include "fifo_3.h"
#include "tlm.h"
#include "systemc.h"

// to make things easier readable...
using namespace sc_core;
using namespace tlm;
using namespace tlm_utils;

fifo_3::fifo_3(sc_module_name name, unsigned int fifo_size) :
		fifo2prod_socket("fifo2prod_socket"),
		fifo2consum_socket("fifo2consum_socket"),
		fifo_size(fifo_size),
		r_peq("read_peq"),
		w_peq("write_peq")
{
	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// register nb_transport_fw function with sockets
	fifo2prod_socket.register_nb_transport_fw(this, &fifo_3::nb_transport_fw);
	fifo2consum_socket.register_nb_transport_fw(this, &fifo_3::nb_transport_fw);
	// ####################### UP TO HERE ####################### //

	// register the read and write processes with the simulation kernel
	SC_THREAD(read_fifo);
	sensitive << r_peq.get_event();
	SC_THREAD(write_fifo);
	sensitive << w_peq.get_event();

	// initialization of FIFO: allocate memory and reset read/write addresses
	fifo_data = new unsigned char[fifo_size];

	wr_ptr = 0;
	rd_ptr = 0;
	fill_level = 0;
}

void fifo_3::read_fifo() {

	unsigned int len;	// amount of data read
	unsigned char* ptr;	// pointer where to put data

	tlm_sync_enum tlm_resp;
	tlm_response_status status;
	sc_time delay;
	tlm_phase phase;
	tlm_generic_payload *payload;

	while(true) {
		// wait for read to be triggered from read payload event queue
		wait();

		// ############# COMPLETE THE FOLLOWING SECTION ############# //
		// get the transaction out of the read payload event queue and get
		// information from generic payload
		payload = r_peq.get_next_transaction();
		len = payload->get_data_length();
		ptr = payload->get_data_ptr();
		// ####################### UP TO HERE ####################### //

		if(fill_level < len) {	// not enough data to read
			len = fill_level;	// none or less data will be read
			status = TLM_BURST_ERROR_RESPONSE;
		}
		else
			status = TLM_OK_RESPONSE;

		// ############# COMPLETE THE FOLLOWING SECTION ############# //
		// handle read - same as ex_3
		/* delay = sc_time(int(len*100), SC_NS);
		wait(delay);
	
		
		cout << std::setw(9) << sc_time_stamp() << ": '" << name() << "' " << len << " words have been read: 0x " << hex;
		for(unsigned int i = 0; i<len; i++) {
			cout << std::setw(2) << std::setfill('0') << (int)*(fifo_data+rd_ptr) << " ";
			memcpy((ptr+i), (fifo_data + rd_ptr), 1);
			rd_ptr = (rd_ptr + 1)%fifo_size;
			fill_level--;	
		} */

		
		//read whole thing
		if(rd_ptr+len < fifo_size) memcpy(ptr, (fifo_data+rd_ptr), len);

		//read two parts separately
		else {
			memcpy(ptr, (fifo_data+rd_ptr), (fifo_size - rd_ptr)); 
			memcpy((ptr + (fifo_size - rd_ptr)), fifo_data, (len-(fifo_size - rd_ptr))); 
		} 

		cout << std::setw(9) << sc_time_stamp() << ": '" << name() << "' " << len << " words have been read: 0x " << hex;
		for(unsigned int i=0; i<len; i++) {
			cout << std::setw(2) << std::setfill('0') << (int)(*(ptr+i)) << " ";
		}

		cout << dec;
		cout << " " << endl;
		//update rd ptr and fill level in one go
		rd_ptr = (rd_ptr+len)%fifo_size;
		fill_level = fill_level - len;
		//count = len;



		// ####################### UP TO HERE ####################### //

		if(fifo_size <= 50)
			output_fifo_status();

		// ############# COMPLETE THE FOLLOWING SECTION ############# //
		// prepare backward call, call nb_transport_bw, and evaluate response
		payload->set_response_status(status);
		payload->set_data_length(len);
		phase = BEGIN_RESP;
		delay = SC_ZERO_TIME;
		tlm_resp = fifo2consum_socket->nb_transport_bw(*payload, phase, delay);
		if(tlm_resp != TLM_COMPLETED || phase != END_RESP)
		{
			cout << std::setw(9) << sc_time_stamp() << ": " << name() << "'\tprotocol error! " << "Read request violated " << endl;
exit(1);

		}
		// ####################### UP TO HERE ####################### //
	}
}

void fifo_3::write_fifo() {

	unsigned int len;	// amount of data written
	unsigned char* ptr;	// pointer where to put data

	tlm_sync_enum tlm_resp;
	tlm_response_status status;
	sc_time delay;
	tlm_phase phase;
	tlm_generic_payload *payload;

	while(true) {
		// wait for write to be triggered from write payload event queue
		wait();

		// ############# COMPLETE THE FOLLOWING SECTION ############# //
		// get the transaction out of the write payload event queue and get
		// information from generic payload
		payload = w_peq.get_next_transaction();
		len = payload->get_data_length();
		ptr = payload->get_data_ptr();

		// ####################### UP TO HERE ####################### //

		if(fill_level + (int)len > fifo_size) { // not enough space for all data
			len = fifo_size - fill_level;		// none or less data will be written
			status = TLM_BURST_ERROR_RESPONSE;
		}
		else
			status = TLM_OK_RESPONSE;

		// ############# COMPLETE THE FOLLOWING SECTION ############# //
		// handle write
		/* delay = sc_time(int(len*100), SC_NS);
		wait(delay);
	
		
		cout << std::setw(9) << sc_time_stamp() << ": '" << name() << "' " << len << " words have been written: 0x " << hex;
		for(unsigned int i = 0; i<len; i++) {
			cout << std::setw(2) << std::setfill('0') << (int)*(ptr+i) << " ";
			memcpy((fifo_data + wr_ptr), (ptr+i), 1);
			wr_ptr = (wr_ptr + 1)%fifo_size;
			fill_level++;	
		} 
		*/
	
		//write whole thing
		if(wr_ptr+len < fifo_size) memcpy((fifo_data+wr_ptr), (ptr), len);

		//write two parts separately
		else {
			memcpy((fifo_data+wr_ptr), ptr, (fifo_size - wr_ptr)); 
			memcpy(fifo_data, (ptr+(fifo_size - wr_ptr)), (len-(fifo_size - wr_ptr))); 
		} 


		cout << std::setw(9) << sc_time_stamp() << ": '" << name() << "' " << len << " words have been written: 0x " << hex;
		for(unsigned int i=0; i<len; i++) {
			cout << std::setw(2) << std::setfill('0') << (int)(*(ptr+i)) << " ";
		}

	
		cout << dec;
		cout << " " << endl;
		//update wr ptr and fill level in one go
		wr_ptr = (wr_ptr+len)%fifo_size;
		fill_level = fill_level + len;
		//count = len; 

		// ####################### UP TO HERE ####################### //

		if(fifo_size <= 50)
			output_fifo_status();

		// ############# COMPLETE THE FOLLOWING SECTION ############# //
		// prepare backward call, call nb_transport_bw, and evaluate response
		payload->set_response_status(status);
		payload->set_data_length(len);
		phase = BEGIN_RESP;
		delay = SC_ZERO_TIME;
		tlm_resp = fifo2prod_socket->nb_transport_bw(*payload, phase, delay);
	       	if(tlm_resp != TLM_COMPLETED || phase != END_RESP)
		{
			cout << std::setw(9) << sc_time_stamp() << ": " << name() << "'\tprotocol error! " << "Write request violated " << endl;
exit(1);
		}

		// ####################### UP TO HERE ####################### //
	}
}

// nb_transport_fw, implementation of fw calls from initiators
tlm_sync_enum fifo_3::nb_transport_fw(
		tlm_generic_payload &payload,	// ref to Generic Payload
		tlm_phase &phase,				// ref to phase
		sc_time &delay_time				// ref to delay time
		)
{
	// check whether transaction is initiated correctly
	if(phase != BEGIN_REQ) {
		cout << std::setw(9) << sc_time_stamp() << ": '" << name()
				<< "'\tprotocol error! "
				<< "nb_transport_fw call with phase!=BEGIN_REQ!" << endl;
		exit(1);
	}

	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// determine operation and how much data is involved
	tlm_command cmd = payload.get_command();
	unsigned int len = payload.get_data_length();

	// ####################### UP TO HERE ####################### //

	if(cmd == TLM_WRITE_COMMAND) {
		// increase delay to cycle time multiplied by number of words written
		delay_time += sc_time(len * 100, SC_NS);

		// put transaction into write payload event queue: the do_write()
		// process will be triggered after delay time
		w_peq.notify(payload, delay_time);
		payload.set_response_status(TLM_OK_RESPONSE);
	}

	else if(cmd == TLM_READ_COMMAND) {
		// increase delay to cycle time multiplied by number of words read
		delay_time += sc_time(len * 100, SC_NS);

		// put transaction into read payload event queue: the do_read() process
		// will be triggered after delay time
		r_peq.notify(payload, delay_time);
		payload.set_response_status(TLM_OK_RESPONSE);
	}

	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// finish the first phase of the transaction
	phase = END_REQ;
	return TLM_UPDATED;
	// ####################### UP TO HERE ####################### //
}

// helper function to output content of FIFO
void fifo_3::output_fifo_status() {
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
	cout << dec << endl << endl;
}
