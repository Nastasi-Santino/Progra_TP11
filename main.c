include "port_emulation.h"

int main (void){

	showReg('d');
	putchar('\n');
	maskOn('d', 255);
	showReg('d');


	return 0;
}
