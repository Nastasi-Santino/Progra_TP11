/*
 * port_emulation.c
 *
 *  Created on: Oct 25, 2023
 *      Author: Grupo 4
 */
#include <stdint.h>
#include "port_emulation.h"
/*
typedef struct {
	uint8_t b0 :1;
	uint8_t b1 :1;
	uint8_t b2 :1;
	uint8_t b3 :1;
	uint8_t b4 :1;
	uint8_t b5 :1;
	uint8_t b6 :1;
	uint8_t b7 :1;
}registro_bits_t;
*/

typedef struct {
	int8_t a;
	int8_t b;
}registro_t;

typedef union {
	registro_t ab;
	int16_t d;
}registroD_t;

static registroD_t ports;

static int8_t * portSelector(char, uint8_t *);

void showReg(char p){

	int8_t * reg;

	reg = portSelector(p, NULL);

	int8_t temp;

	int8_t bits[8];
	int i;

	if(p == 'd' || p == 'D'){

	} else {
		temp = *reg;
		for(i = 0; i < 8; i++){
			bits[i] = temp % 2;
			temp /= 2;
		}
		for(i = 7; i >= 0; i--){
			printf("%d", bits[i]);
		}
		putchar('\n');
	}

}

static int8_t * portSelector(char p, uint8_t * b){

	int8_t * port;

		switch(p){
			case 'a':
			case 'A':
				port = &(ports.ab.a);
				break;
			case 'b':
			case 'B':
				port = &(ports.ab.b);
				break;
			case 'd':
			case 'D':
				port = &(ports.ab.b);
				*b -= 8;
				break;
		}

		return port;
}

void bitSet(char p, uint8_t bit){

	int8_t * port;

	port = portSelector(p, &bit);

	uint8_t mascara = 0x0000001b;
	int i;

	for(i = 0; i < bit;i++){
		mascara <<= 1;
	}

	 *port = *port || mascara;
}

void bitClr(char p, uint8_t bit){

	int8_t * port;

	port = portSelector(p, &bit);

	uint8_t mascara = 0x0000001b;
	int i;

	for(i = 0; i < bit;i++){
		mascara <<= 1;
	}

	mascara = ~mascara;

	*port = *port || mascara;
}
