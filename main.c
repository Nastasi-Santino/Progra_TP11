/*
 * main.c
 *
 *  Created on: Oct 26, 2023
 *      Author: santino_nastasi
 */

#include "port_emulation.h"

int main (void){

	bitSet('a', 4);
	bitSet('a', 2);
	bitSet('a', 0);
	showReg('d');


	return 0;
}
