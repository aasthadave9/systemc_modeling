#include "Cpu.h"
#include "IoModule.h"
#include "reporting.h"

using namespace sc_core;
using namespace tlm;

void Cpu::processor_thread(void) {	

		// ############# COMPLETE THE FOLLOWING SECTION ############# //
		// read new packet descriptor
	
	unsigned int data_len = sizeof(m_packet_descriptor);
	unsigned int header_len = sizeof(sc_time)+sizeof(uint64_t)+IpPacket::MINIMAL_IP_HEADER_LENGTH;
	soc_address_t baseAddr = m_packet_descriptor.baseAddress;
	int outq_addr_array[4] = {0x20000000, 0x30000000, 0x40000000, 0x50000000};
	unsigned int nexthop;
				
	while (true) {
		wait(packetReceived_interrupt.value_changed_event());
		
		while(packetReceived_interrupt.read()) {
			MEASURE_TRANSFER_TIME(
			startTransaction(TLM_READ_COMMAND, 0x10000000, 
			(unsigned char *) &m_packet_descriptor, data_len);
			startTransaction(TLM_READ_COMMAND, (0x00000000 + baseAddr), 
			(unsigned char *) &m_packet_header, header_len);)
	
		
			if(verifyHeaderIntegrity(m_packet_header)) {
				if(use_accelerator) {
					MEASURE_PROCESSING_TIME(
					wait(CPU_VERIFY_HEADER_CYCLES*CLK_CYCLE_CPU);)
					
					m_lookup_request.destAddress = m_packet_header.getDestAddress();
					m_lookup_request.processorId = m_id;

					// write the lookup input to acc
					MEASURE_TRANSFER_TIME(
					startTransaction(TLM_WRITE_COMMAND, 0x60000000,
					(unsigned char *) &m_lookup_request, sizeof(m_lookup_request)); )
					
					// continue processing header
					MEASURE_PROCESSING_TIME(
					decrementTTL(m_packet_header);
					wait(CPU_DECREMENT_TTL_CYCLES*CLK_CYCLE_CPU);
					updateChecksum(m_packet_header);
					wait(CPU_UPDATE_CHECKSUM_CYCLES*CLK_CYCLE_CPU);)
					
					// wait till acc returns lookup result, then read result
					if(!lookupReady_interrupt) {
						wait(lookupReady_interrupt.value_changed_event());
					}
					MEASURE_TRANSFER_TIME(
					startTransaction(TLM_READ_COMMAND, 0x60000000,
					(unsigned char *) &nexthop, sizeof(unsigned int)); )									
				}
				
				else {
					// no accelerator, continue processing header & nexthop
					MEASURE_PROCESSING_TIME(
					decrementTTL(m_packet_header);
					wait(CPU_DECREMENT_TTL_CYCLES*CLK_CYCLE_CPU);
					updateChecksum(m_packet_header);
					wait(CPU_UPDATE_CHECKSUM_CYCLES*CLK_CYCLE_CPU);
					nexthop = makeNHLookup(m_packet_header);	
					wait(CPU_IP_LOOKUP_CYCLES*CLK_CYCLE_CPU);)
				}
				
				// write header to mem and pkt descr to outq from nexthop calc
				MEASURE_TRANSFER_TIME(
				startTransaction(TLM_WRITE_COMMAND, (0x00000000+baseAddr), 
				(unsigned char *) &m_packet_header, header_len); 
				startTransaction(TLM_WRITE_COMMAND, outq_addr_array[nexthop],
				(unsigned char *) &m_packet_descriptor, data_len);)
			}

			else { //write packet descriptor to discard queue
			        MEASURE_PROCESSING_TIME(
			       	wait(CPU_VERIFY_HEADER_CYCLES*CLK_CYCLE_CPU); ) 
								
				MEASURE_TRANSFER_TIME(
			        startTransaction(TLM_WRITE_COMMAND, 0x10000000, 
				(unsigned char *) &m_packet_descriptor, data_len);)
			}	
			
		}
	}

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
				cout << std::setw(9) << sc_time_stamp() << ": '" << name() << "'\tprotocol error! " << "Packet descriptor/header request not completed appropriately" << endl;
				exit(1);	
		}
		status = trans.get_response_status();
		if(status == TLM_INCOMPLETE_RESPONSE) continue; //start fresh transaction
			
		if(status == TLM_OK_RESPONSE) { 
			//break out, move on to writing operation
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

