#include "port_emulation.h"


#define REGISTRO_8BITS(x) ((x) == 'a' || (x) == 'A' || (x) == 'b' || (x) == 'B')
#define REGISTRO_16BITS(x) ((x) == 'd' || (x) == 'D')
#define REGISTRO_EXISTENTE(x) ((REGISTRO_8BITS(x)) || ((REGISTRO_16BITS(x))))
#define ERROR 1
#define OK 0

typedef struct {
	uint8_t a;
	uint8_t b;
}registro_t;

typedef union {
	registro_t ab;
	uint16_t d;
}registroD_t;

static registroD_t ports;

static uint8_t * portSelector(char, uint8_t *);
static uint8_t bitSelectorMask(uint8_t);
static int validar(char, uint8_t);
static int validarMascara(char, uint16_t);

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

static int validarMascara(char p, uint16_t mask){

	if(REGISTRO_8BITS(p) && (mask > 255)){
		return ERROR;
	} else {
		return OK;
	}

}

static uint8_t * portSelector(char p, uint8_t * b){

	uint8_t * port;

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

static uint8_t bitSelectorMask(uint8_t bit){

	uint8_t mask = 1;

	int i;
	for(i = 0; i < bit;i++){
		mask <<= 1;
	}
	return mask;
}

int showReg(char p){

	if(validar(p, 0)){
		return ERROR;
	}

	uint8_t * reg = portSelector(p, NULL);

	uint8_t temp;

	uint8_t bits[8];
	int i;

	if(REGISTRO_16BITS(p)){
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

	uint8_t * port = portSelector(p, &bit);

	uint8_t mascara = bitSelectorMask(bit);
	 *port = *port | mascara;

	 return OK;
}

int bitClr(char p, uint8_t bit){

	if(validar(p, bit)){
		return ERROR;
	}

	uint8_t * port = portSelector(p, &bit);

	uint8_t mascara = ~bitSelectorMask(bit);

	*port = *port & mascara;

	return OK;
}

int bitToggle(char p, uint8_t bit){

	if(validar(p, bit)){
		return ERROR;
	}

	uint8_t * port = portSelector(p, &bit);

	uint8_t mascara = bitSelectorMask(bit);

	*port = *port ^ mascara;

	return 0;
}

int bitGet(char p, uint8_t bit){

	if(validar(p, bit)){
		return ERROR;
	}

	uint8_t * port = portSelector(p, &bit);
	int8_t on_off;

	uint8_t mascara = bitSelectorMask(bit);

	on_off = *port & mascara;

	printf("%d\n", on_off ? 1 : 0);

	return OK;
}

int maskOn(char p, uint16_t mask){

	if(validar(p, 0)){
		return ERROR;
	}

	if(validarMascara(p, mask)){
		return ERROR;
	}

	uint8_t * port = portSelector(p, NULL);

	if(REGISTRO_16BITS(p)){
		if(mask <= 255){
			maskOn('b', mask);
		} else {
			maskOn('a', mask%255);
			maskOn('b', mask - 256);
		}
	} else {
		*port = *port | mask;
	}

	return OK;
}







