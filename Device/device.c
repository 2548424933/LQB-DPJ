#include "device.h"

void Device_Ctrl(u8 data2,u8 data0){
	P0 = data0;
	P2 = P2&0x1f | data2;
	P2 = P2&0x1f;
}

void System_Init(){
	Device_Ctrl(0x80,0xff);
	Device_Ctrl(0xa0,0xaf);
}



void Delay5ms(void)	//@12.000MHz
{
	unsigned char data i, j;

	i = 59;
	j = 90;
	do
	{
		while (--j);
	} while (--i);
}
