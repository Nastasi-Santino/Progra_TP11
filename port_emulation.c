/*
 * port_emulation.c
 *
 *  Created on: Oct 25, 2023
 *      Author: Grupo 4
 */
#include "port_emulation.h"

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
		showReg('a');
		showReg('b');
	} else {
		temp = *reg;
		for(i = 0; i < 8; i++){
			bits[i] = temp % 2;
			temp /= 2;
		}
		for(i = 7; i >= 0; i--){
			printf("%d", bits[i]);
		}
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
				if(b != NULL){
					if(*b > 7){
						port = &ports.ab.a;
						*b -= 8;
					} else {
						port = &ports.ab.b;
					}
				}
				break;
		}

		return port;
}

void bitSet(char p, uint8_t bit){

	int8_t * port;

	port = portSelector(p, &bit);

	uint8_t mascara = 1;
	int i;

	for(i = 0; i < bit;i++){
		mascara <<= 1;
	}

	 *port = *port | mascara;
}

void bitClr(char p, uint8_t bit){

	int8_t * port;

	port = portSelector(p, &bit);

	uint8_t mascara = 1;
	int i;

	for(i = 0; i < bit;i++){
		mascara <<= 1;
	}

	mascara = ~mascara;

	*port = *port & mascara;
}

void bitToggle(char p, uint8_t bit){

	int8_t * port;

	port = portSelector(p, &bit);

	uint8_t mascara = 1;
	int i;

	for(i = 0; i < bit;i++){
		mascara <<= 1;
	}

	*port = *port ^ mascara;
}

void bitGet(char p, uint8_t bit){

	int8_t * port;
	int8_t on_off;

	port = portSelector(p, &bit);

	uint8_t mascara = 1;
	int i;

	for(i = 0; i < bit;i++){
		mascara <<= 1;
	}

	on_off = *port & mascara;

	printf("%d\n", on_off ? 1 : 0);
}







