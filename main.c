#include "port_emulation.h"

/*
 *		LOGICA GENERAL DEL PROGRAMA
 * 			Este breve programa simula el uso de un registro de 8 bits para controlar
 * 		el estado de 8 LEDs. Para ello utiliza la libreria port_emulation, escrita previamente, y
 * 		usa algunas funciones de la misma a traves de algunos comandos del usuario.
 * */

#define OPTION(x) (x == 't' || x == 'c' || x == 's' || x == 'q') 	// macro para ver si lo que ingresa el usuario es un comando valido

int main (void){

	int c;

	printf("Bienvenido al simulador del grupo 3!\n\n"															//
			"Este programa simula 8 LEDs conectados a un registro de 8 bits\n"									//
			"1 simula un LED prendido y 0 un LED apagado.\n\n"													//
			"Comandos disponibles:\n"																			//
			"1) Ingresar un numero entre 0 y 7: prende el LED correspondiente al numero\n"						//
			"2) Ingresar t: cambia el estado de todos los LEDs, prende los apagados y apaga los prendidos\n"	// MENU
			"3) Ingresar s: prende todos los LEDs\n"															//
			"4) Ingresar c: apaga todos los LEDs\n"																//
			"5) Ingresar q: termina el programa\n"																//
			"Presione Enter para comenzar!\n");																	//
																												//
	while(getchar() != '\n');																					//

	do{
		showReg('a');												// Mostramos el estado actual de los LEDs, que
		putchar('\n');												// estan conectados al registro a. Pedimos al usuario que
		printf("Ingrese un comando: ");								// ingrese un comando.
		c = getchar();												//
		if(OPTION(c) && getchar() == '\n'){							// Verificamos que sea un comando (solo 1 letra, minuscula).
			switch(c){												//
				case 't':											// Si el comando es t, llamamos a maskToggle y le pasamos
					maskToggle('a', 0b11111111);					// una mascara para indicar que cambie el estado de todos
					break;											// los bits, toggleando el estado de todos los LEDs.
				case 's':											// Si el comando es s, llamamos a maskOn y le pasamos una
					maskOn('a', 0b11111111);						// mascara para indicar que prenda todos los bits,
					break;											// prendiendo todos los LEDs.
				case 'c':											// Si el comando es c, llamamos a maskOff y le pasamos una
					maskOff('a', 0b11111111);						// mascara para indicar que apago todos los bits.
					break;											//
				case 'q':											// Si el comando es q no hace nada, luego lo va a detectar
					break;											// y va a salir del loop.
				default:											// Como antes de entrar al switch verificamos que ocurra
					printf("No deberia llegar aca\n");				// uno de los casos, nunca llegara al default.
			}														//
		} else if((c >= '0' && c <= '7') && getchar() == '\n'){		// Si es un numero del 0 al 7, llamamos a bitSet y le
			bitSet('a', c - '0');									// pasamos el numero que dio el usuario pasandolo antes
		}else {														// de ascii al int correcto. Se prendera ese bit.
			printf("Error introduzca un comando correcto\n");		// Si no es ninguno de los anteriores casos, indicamos
			while(getchar() != '\n');								// que introduzca un comando valido y con el while
		}															// borramos el buffer.
	}while(c != 'q');												//

	return 0;
}
