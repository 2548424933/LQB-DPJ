#include "device.h"

void Device_Ctrl(data2,data1){
	P0 = data1;
	P2 = P2 & 0X1F| data2;
	P2 = P2&0X1F;
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
