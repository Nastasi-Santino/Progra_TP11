/*
 * port_emulation.c
 *
 *  Created on: Oct 25, 2023
 *      Author: Grupo 4
 */
#include "port_emulation.h"

#define REGISTRO_EXISTENTE(x) ((x) == 'a' || (x) == 'A' || (x) == 'b' || (x) == 'B' || (x) == 'd' || (x) == 'D')
#define ERROR 1
#define OK 0

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
static int validar(char, uint8_t);

static int validar(char p, uint8_t bit){

	if(REGISTRO_EXISTENTE(p)){
		if(bit < 16){
			return OK;
		} else {
			return ERROR;
		}
	} else {
		return ERROR;
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

int showReg(char p){

	if(validar(p, 0)){
		return ERROR;
	}

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
	return OK;
}

int bitSet(char p, uint8_t bit){

	if(validar(p, bit)){
		return ERROR;
	}

	int8_t * port;

	port = portSelector(p, &bit);

	uint8_t mascara = 1;
	int i;

	for(i = 0; i < bit;i++){
		mascara <<= 1;
	}

	 *port = *port | mascara;

	 return OK;
}

int bitClr(char p, uint8_t bit){

	if(validar(p, bit)){
		return ERROR;
	}

	int8_t * port;

	port = portSelector(p, &bit);

	uint8_t mascara = 1;
	int i;

	for(i = 0; i < bit;i++){
		mascara <<= 1;
	}

	mascara = ~mascara;

	*port = *port & mascara;

	return OK;
}

int bitToggle(char p, uint8_t bit){

	if(validar(p, bit)){
		return ERROR;
	}

	int8_t * port;

	port = portSelector(p, &bit);

	uint8_t mascara = 1;
	int i;

	for(i = 0; i < bit;i++){
		mascara <<= 1;
	}

	*port = *port ^ mascara;

	return 0;
}

int bitGet(char p, uint8_t bit){

	if(validar(p, bit)){
		return ERROR;
	}

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

	return OK;
}







