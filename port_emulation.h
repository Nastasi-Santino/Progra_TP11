#ifndef PORT_EMULATION_H_
#define PORT_EMULATION_H_


#include <stdint.h>
#include <stdio.h>

int bitSet(char, uint8_t);
int bitClr(char, uint8_t);
int bitToggle(char, uint8_t);
int bitGet(char, uint8_t);
int maskOn(char, uint16_t);
int showReg(char);

#endif /* PORT_EMULATION_H_ */
