#include "Cpu.h"
#include "IoModule.h"
#include "reporting.h"

using namespace sc_core;
using namespace tlm;

void Cpu::processor_thread(void) {	

		// ############# COMPLETE THE FOLLOWING SECTION ############# //
		// read new packet descriptor
	
	int data_len = sizeof(m_packet_descriptor);
	
	//loopback packet to random output queue
	int outq_addr_array[4] = {0x20000000, 0x30000000, 0x40000000, 0x50000000};
	int outq_addr = outq_addr_array[rand() % 4];
		
	while (true) {
		wait(packetReceived_interrupt.value_changed_event());
		
		while(packetReceived_interrupt.read()) {
			startTransaction(TLM_READ_COMMAND, 0x10000000, 
			(unsigned char *) &m_packet_descriptor, data_len);

		// ####################### UP TO HERE ####################### //

		// ############# COMPLETE THE FOLLOWING SECTION ############# //
		// Forward the packet descriptor to an arbitrary port
			startTransaction(TLM_WRITE_COMMAND, outq_addr, 
			(unsigned char *) &m_packet_descriptor, data_len);
		}
	}

		// ####################### UP TO HERE ####################### //
	
}

// ############# COMPLETE THE FOLLOWING SECTION ############# //
// startTransaction
void Cpu::startTransaction(tlm_command command, soc_address_t address,
			unsigned char *data, unsigned int dataSize) {
	do {
		tlm_generic_payload trans;
		tlm_sync_enum result;
		tlm_response_status status;
		sc_time delay;
		tlm_phase phase;
	
		trans.set_command(command);
		trans.set_data_ptr(data);
		trans.set_data_length(dataSize);
		trans.set_address(address);

		phase = BEGIN_REQ;
		delay = SC_ZERO_TIME;
		
		result = initiator_socket->nb_transport_fw(trans, phase, delay);

		//wait till response from bus (target) arrives
		wait(transactionFinished_event);
	
		if(result != TLM_UPDATED || phase != END_REQ) {
				cout << std::setw(9) << sc_time_stamp() << ": '" << name() << "'\tprotocol error! " << "Packet descriptor request not completed appropriately" << endl;
				exit(1);	
		}
		status = trans.get_response_status();
		if(status == TLM_INCOMPLETE_RESPONSE) continue; //start fresh transaction
			
		if(status == TLM_OK_RESPONSE) { 
			//break out, move on to writing to output queue
			break;
		}
	} while(true);
				
}

// ####################### UP TO HERE ####################### //

// nb_transport_bw: implementation of the backward path callback
tlm_sync_enum Cpu::nb_transport_bw(tlm_generic_payload& transaction,
		tlm_phase& phase, sc_time& delay_time) {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	if(phase != BEGIN_RESP) {
		cout << std::setw(9) << sc_time_stamp() << ": '" << name() << "'\tprotocol error! " << "Response could not be set up" << endl;
	}
	
	delay_time += sc_time(CLK_CYCLE_BUS);	
	transactionFinished_event.notify(delay_time);
	phase = END_RESP;
	return TLM_COMPLETED;


	// ####################### UP TO HERE ####################### //
}

unsigned int Cpu::instances = 0;
