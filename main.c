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
	putchar('\n');
	bitToggle('b', 1);
	bitToggle('a', 2);
	showReg('d');
	putchar('\n');
	bitGet('a', 2);
	bitGet('d', 12);



	return 0;
}
