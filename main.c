#include "port_emulation.h"

#define OPTION(x) (x == 't' || x == 'c' || x == 's' || x == 'q')

int main (void){

	int c;

	do{
		showReg('a');
		putchar('\n');
		c = getchar();
		if(OPTION(c) && getchar() == '\n'){
			switch(c){
				case 't':
					maskToggle('a', 0b11111111);
					break;
				case 's':
					maskOn('a', 0b11111111);
					break;
				case 'c':
					maskOff('a', 0b11111111);
					break;
				case 'q':
					break;
				default:
					printf("No deberia llegar aca\n");
			}
		} else if((c >= '0' && c <= '7') && getchar() == '\n'){
			bitSet('a', c - '0');
		}else {
			printf("Error introduzca un comando correcto\n");
			while(getchar() != '\n');
		}
	}while(c != 'q');

	return 0;
}
