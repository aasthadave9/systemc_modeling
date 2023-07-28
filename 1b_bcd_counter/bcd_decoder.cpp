#include "bcd_decoder.h"

void bcd_decoder::decode() {
	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	//modulo with 10 to get units position
	//divide with 10 to get tens position
	
	char units = (char)(val.read() % 10); 
	char tens = (char)(val.read() / 10);
	
	lo.write(units);
	hi.write(tens);

	// ####################### UP TO HERE ####################### //
}


