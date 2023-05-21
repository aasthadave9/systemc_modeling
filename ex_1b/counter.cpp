#include "counter.h"

void counter::count() {
	
	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	if(rst_n.read()== false)  //active low asynchronous reset
		cnt_int = 0;	
	else 
		cnt_int = (cnt_int + 1) % 100;
	
	cnt.write(cnt_int);

	



	// ####################### UP TO HERE ####################### //
}
