#include "bluetooth.h"
#include "ev3api.h"
#include <stdio.h>

static FILE *bt;

void bluetooth_w()
{
	bt = ev3_serial_open_file(EV3_SERIAL_BT);
	
	while(1){
		uint8_t c = fgetc(bt);
		fputc(c, bt);
		if(c == '1'){
			break;
		}
	}
}