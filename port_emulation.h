/*
 * port_emulation.h
 *
 *  Created on: Oct 26, 2023
 *      Author: santino_nastasi
 */

#ifndef PORT_EMULATION_H_
#define PORT_EMULATION_H_


#include <stdint.h>
#include <stdio.h>

void bitSet(char, uint8_t);
void bitClr(char, uint8_t);
void bitToggle(char, uint8_t);
void showReg(char);

#endif /* PORT_EMULATION_H_ */
