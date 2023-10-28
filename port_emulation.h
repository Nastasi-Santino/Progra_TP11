#ifndef PORT_EMULATION_H_
#define PORT_EMULATION_H_

#include <stdint.h>
#include <stdio.h>

// Esta funcion recibe un puerto y un numero de bit, y cambia su estado a 1.
int bitSet(char, uint8_t);

// Esta funcion recibe un puerto y un numero de bit, y cambia su estado a 0.
int bitClr(char, uint8_t);

// Esta funcion recibe un puerto y un numero de bit, y cambia su estado al opuesto.
int bitToggle(char, uint8_t);

// Esta funcion recibe un puerto y un numero de bit, e imprime su valor.
int bitGet(char, uint8_t);

// Esta funcion recibe un puerto y una mascara, y prende los bits que indique la mascara.
int maskOn(char, uint16_t);

// Esta funcion recibe un puerto y una mascara, y apaga los bits que indique la mascara.
int maskOff(char, uint16_t);

// Esta funcion recibe un puerto y una mascara, e invierte el estado de los bits que indique la mascara.
int maskToggle(char, uint16_t);

// Esta funcion recibe un puerto e imprime en pantalla el registro entero.
int showReg(char);

#endif /* PORT_EMULATION_H_ */
