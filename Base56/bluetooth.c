#include "bluetooth.h"
#include "ev3api.h"
#include <stdio.h>

FILE *bt;

int bluetooth_w()
{	
	volatile uint8_t c = fgetc(bt);
	fputc(c, bt);
	if(c == '1'){
		return 1;
	}
	return 0;
}