#include "port_emulation.h"

/*
 * 	 	LOGICA DE LA LIBRERIA
 * 	 		Esta libreria emula un microprocesador que tiene dos registros de 8 bits y un registro de 16 bits
 * 		que es la union de los dos registos anteriores (como el Motorola 68HC11).
 *			Para simular los registros usa una estructura y una union.
 *			Ademas tiene varias funciones, algunas que funcionan bit a bit y otras que funcionan modificando
 *		el registro entero. Para hacer estas funciones decidimos usar mascaras dinamicas.
 *
 * */


#define REGISTRO_8BITS(x) ((x) == 'a' || (x) == 'A' || (x) == 'b' || (x) == 'B') // Macro para ver si es registro de 8 bits.
#define REGISTRO_16BITS(x) ((x) == 'd' || (x) == 'D')	// Macro para ver si es registro de 16 bits.
#define REGISTRO_EXISTENTE(x) ((REGISTRO_8BITS(x)) || ((REGISTRO_16BITS(x)))) // Macro para ver si es cuaqluier registro.
#define ERROR 1 // Macros para manejo
#define OK 0    // de errores.

typedef struct {	// Creamos una structura que contiene 2 int de 8 bits, esto es
	uint8_t b; 		// el puerto a y el puerto b.
	uint8_t a;		//
}registro_t;		//

typedef union {		// Creamos una union que tiene 16 bits, en ella esta una estructura con 2 puertos y
	registro_t ab;	// un int de 16 bits. Asi, el int de 16 bits no es mas que "juntar" los dos puertos.
	uint16_t d;		// Es decir, el puerto d.
}registroD_t;		//

static registroD_t ports; // crea el los puertos como una union.

// Esta funcion valida que un registro exista y que el numero de bit tenga sentido.
static int validar(char, uint8_t);

// Esta funcion valida que una mascara sea de tamaño acorde al puerto.
static int validarMascara(char, uint16_t);

// Esta funcion selecciona un registro y devuelve un puntero al mismo.
static uint8_t * portSelector(char, uint8_t *);

// Esta funcion arma una mascara para seleccionar un bit.
static uint8_t bitSelectorMask(uint8_t);

// Esta funcion valida que un registro exista y que el numero de bit tenga sentido.
static int validar(char p, uint8_t bit){

	if(REGISTRO_EXISTENTE(p)){			// Primero verifica que el registro existe .
		if(bit < 16){					// luego si es un registro de 16 bits
			if(REGISTRO_16BITS(p)){		//  y el bit esta entre 0-7 devuelve que no hay error.
				return OK;				//
			} else{						// si es un registro de 8 bits y el bit esta entre 0-7
				if(bit < 8){			// devulve que no hay error.
					return OK;			//
				} else{					//
					return ERROR;		//
				}						//
			}							// Cualquier otro caso devuelve error.
		} else {						//
			return ERROR;				//
		}								//
	} else {							//
		return ERROR;					//
	}
}

// Esta funcion valida que una mascara sea de tamaño acorde al puerto.
static int validarMascara(char p, uint16_t mask){
												//
	if(REGISTRO_8BITS(p) && (mask > 255)){		// Solo si el registro es a o b y la mascara es de
		return ERROR;							// mas de 8 bits va a devolver error.
	} else {									// No puede tener mas de 16 bits porque es tipo uint16
		return OK;								//
	}											//
}												//

// Esta funcion selecciona un registro y devuelve un puntero al mismo.
static uint8_t * portSelector(char p, uint8_t * b){
												//
	uint8_t * port;								// Esta funcion recibe un char indicando el puerto, y devuelve un
												// puntero al puerto.
		switch(p){								//
			case 'a':							//
			case 'A':							//
				port = &(ports.ab.a);			// Si es el puerto a, guarda en port la posicion de memoria del puerto a.
				break;							//
			case 'b':							//
			case 'B':							//
				port = &(ports.ab.b);			// Si es el puerto b, guarda en port la posicion de memoria del puerto a.
				break;							//
			case 'd':							//
			case 'D':							//
				if(b != NULL){					// Si es el puerto d, se ajusta dependiendo el bit indicado. Si el bit
					if(*b > 7){					// indicado esta en a, guarda la posicion de memoria del puerto a y ajusta
						port = &ports.ab.a;		// el bit para que coincida.
						*b -= 8;				//
					} else {					// Si el bit indicado esta en b, guarda la posicion de memoria del puerto b
						port = &ports.ab.b;		// y no hace falta ajustar el bit porque ya coincide.
					}							//
				} else{							//
					return NULL; 				// Ademas verifica que el puntero al bit que recibe no sea el NULL ya que
				}								// esta funcion puede ser llamada sin indicar ningun bit, en cuyo caso
				break;							// si se llama con 'd' devuevle NULL.
			default:							//
				return NULL;					// Como el char que recibe siempre es verificado antes de ser pasado a
		}										// esta funcion, nunca llega al default.
												//
		return port;							// Devuelve el puntero indicado.
}												//

// Esta funcion arma una mascara para seleccionar un bit.
static uint8_t bitSelectorMask(uint8_t bit){
									//
	uint8_t mask = 0b00000001;		// Crea una mascara que selecciona el bit 0 (00000001).
									//
	int i;							// Y luego shiftea la mascara hacia la izquierda tantas veces como el numero de bit
	for(i = 0; i < bit;i++){		// quedando asi una mascara con todos 0 menos en la posicion del bit que recibio
		mask <<= 1;					// donde queda un 1. Asi "separa un bit"
	}								//
	return mask;					// Devuelve la mascara.
}									//

// Esta funcion recibe un puerto e imprime en pantalla el registro entero.
int showReg(char p){
												//
	if(validar(p, 0)){							// Valida que el registro exista. Y pasa un 0 en bit puesto que en
		return ERROR;							// este caso no hay que validar ningun bit.
	}											//
												//
	uint8_t * reg = portSelector(p, NULL);		// Selecciona el registro indicado.
												//
	uint8_t temp;								// Crea una variable temporal para hacer la conversion de base 10 a base 2
	uint8_t bits[8];							// y un array para ir guardando los bits durante la conversion.
												//
	int i;										//
	if(REGISTRO_16BITS(p)){						// De forma recursiva si es el registro a imprimir es el d, imprime el a
		showReg('a');							// y luego el b, dejando impreso el d.
		showReg('b');							//
	} else {									// Si es el registro a o b, hace un peque algoritmo para pasa un numero en
		temp = *reg;							// base 10 a un numero en base 2. Lo que hace es ir dividiendo por 2 y
		for(i = 0; i < 8; i++){					// guardando los restos. De esta forma se guardan los bits pero en forma
			bits[i] = temp % 2;					// invertida.
			temp /= 2;							//
		}										//
		for(i = 7; i >= 0; i--){				// Por ello para imprimir el numero, se itera el array desde el final hasta
			printf("%d", bits[i]);				// el comienzo, invirtiendo los bits para que quede en base 2.
		}										//
	}											//
	return OK;									// Devuelve que no hubo error.
}												//

// Esta funcion recibe un puerto y un numero de bit, y cambia su estado a 1.
int bitSet(char p, uint8_t bit){
												//
	if(validar(p, bit)){						// Valida que el registro exista y que el numero de bit sea
		return ERROR;							// coherente.
	}											//
												//
	uint8_t * port = portSelector(p, &bit);		// Selecciona el registro indicado.
												//
	uint8_t mascara = bitSelectorMask(bit);		// Arma una mascara con todos 0 menos en la posicion del bit.
	 *port |= mascara;							// Con un or bitwise con la mascara el bit este prendido o apagado, terminara
	 	 	 	 	 	 	 	 	 	 	 	// prendido.
	 return OK;									// Devuelve que no hubo error.
}												//

// Esta funcion recibe un puerto y un numero de bit, y cambia su estado a 0.
int bitClr(char p, uint8_t bit){
												//
	if(validar(p, bit)){						// Valida que el registro exista y que el numero de bit sea
		return ERROR;							// coherente.
	}											//
												//
	uint8_t * port = portSelector(p, &bit);		// Selecciona el registro indicado.
												//
	uint8_t mascara = ~bitSelectorMask(bit);	// Arma una mascara con todos 1 menos en la posicion del bit.
	*port &= mascara;							// Con un and bitwise con la mascara el bit este prendido o apagado, terminara
												// apagado.
	return OK;									// Devuelve que no hubo error.
}												//

// Esta funcion recibe un puerto y un numero de bit, y cambia su estado al opuesto.
int bitToggle(char p, uint8_t bit){
												//
	if(validar(p, bit)){						// Valida que el registro exista y que el numero de bit sea
		return ERROR;							// coherente.
	}											//
												//
	uint8_t * port = portSelector(p, &bit);		// Selecciona el registro indicado.
												//
	uint8_t mascara = bitSelectorMask(bit);		// Arma una mascara con todos 0 menos en la posicion del bit.
	*port ^= mascara;							// Con un xor bitwise con la mascara si el bit esta prendido, se apaga; Si
												// esta apagado, se prende. Cambiando el estado al opuesto.
	return OK;									// Devuelve que no hubo error.
}												//

// Esta funcion recibe un puerto y un numero de bit, e imprime su valor.
int bitGet(char p, uint8_t bit){
												//
	if(validar(p, bit)){						// Valida que el registro exista y que el numero de bit sea
		return ERROR;							// coherente.
	}											//
												//
	uint8_t * port = portSelector(p, &bit);		// Selecciona el registro indicado.
	int8_t on_off;								// Crea una variable que va a ser la que indique el estado del bit.
												//
	uint8_t mascara = bitSelectorMask(bit);		// Arma una mascara con todos 0 menos en la posicion del bit.
												//
	on_off = *port & mascara;					// Solo si el bit indicado esta prendido, el and bitwise va a devolver
												// el valor que represente ese bit, que es distinto de 0.
	printf("%d\n", on_off ? 1 : 0);				// Si el bit esta apagado, el and va a devolver 0.
												// Imprime 1 si es un valor distinto de 0, sino 0;
	return OK;									// Devuelve que no hubo error.
}												//

// Esta funcion recibe un puerto y una mascara, y prende los bits que indique la mascara.
int maskOn(char p, uint16_t mask){
													//
	if(validar(p, 0)){								// Valida que el registro exista. Y pasa un 0 en bit puesto que en
		return ERROR;								// este caso no hay que validar ningun bit.
	}												//
													//
	if(validarMascara(p, mask)){					// Valida que el tamaño de la mascara sea acorde al puerto.
		return ERROR;								//
	}												//
													//
	if(REGISTRO_16BITS(p)){							// Si es un puerto de 16 bits, hace un puntero que apunte al puerto d
		uint16_t * port = &ports.d;					// es decir el int de 16 bits de la union. Y luego hace un or bitwise
		*port |= mask;								// con la mascara recibida, prendiendo los bits que indica la mascara.
	} else {										// Si es un puerto de 8 bits, selecciona el puerto como en las
		uint8_t * port = portSelector(p, NULL);		// otras funciones y hace el or bitwise con la mascara, consiguiendo
		*port |= mask;								// el mismo resultado.
	}												//
													//
	return OK;										// Devuelve que no hubo error.
}													//

// Esta funcion recibe un puerto y una mascara, y apaga los bits que indique la mascara.
int maskOff(char p, uint16_t mask){
													//
	if(validar(p, 0)){								// Valida que el registro exista. Y pasa un 0 en bit puesto que en
		return ERROR;								// este caso no hay que validar ningun bit.
	}												//
													//
	if(validarMascara(p, mask)){					// Valida que el tamaño de la mascara sea acorde al puerto.
		return ERROR;								//
	}												//
													//
	if(REGISTRO_16BITS(p)){							// Si es un puerto de 16 bits, hace un puntero que apunte al puerto d
		uint16_t * port = &ports.d;					// es decir el int de 16 bits de la union. Y luego hace un and bitwise
		*port &= ~mask;								// con la mascara recibida negada, apangando los bits que indica la mascara.
	} else {										// Si es un puerto de 8 bits, selecciona el puerto como en las
		uint8_t * port = portSelector(p, NULL);		// otras funciones y hace el and bitwise con la mascara negada,
		*port &= ~mask;								// consiguiendo el mismo resultado.
	}												//
													//
	return OK;										// Devuelve que no hubo error.
}													//

// Esta funcion recibe un puerto y una mascara, e invierte el estado de los bits que indique la mascara.
int maskToggle(char p, uint16_t mask){
													//
	if(validar(p, 0)){								// Valida que el registro exista. Y pasa un 0 en bit puesto que en
		return ERROR;								// este caso no hay que validar ningun bit.
	}												//
													//
	if(validarMascara(p, mask)){					// Valida que el tamaño de la mascara sea acorde al puerto.
		return ERROR;								//
	}												//
													//
	if(REGISTRO_16BITS(p)){							// Si es un puerto de 16 bits, hace un puntero que apunte al puerto d
		uint16_t * port = &ports.d;					// es decir el int de 16 bits de la union. Y luego hace un xor bitwise
		*port ^= mask;								// con la mascara recibida, invirtiendo los bits que indica la mascara.
	} else {										// Si es un puerto de 8 bits, selecciona el puerto como en las
		uint8_t * port = portSelector(p, NULL);		// tras funciones y hace el xor bitwise con la mascara, consiguiendo
		*port ^= mask;								// el mismo resultado.
	}												//
													//
	return OK;										// Devuelve que no hubo error.
}													//
