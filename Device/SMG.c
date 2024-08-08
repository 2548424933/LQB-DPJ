#include "SMG.h"
//0    1    2    3    4    5    6    7    8    9 
u8 smg_code[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
u8 smg_buf[8];

void vSMG_Display(void){
	static int i;
	vDeviceCtrl(0xc0,0);
	vDeviceCtrl(0xe0,~smg_buf[i]);
	vDeviceCtrl(0xc0,0x01<<i);
	i++;
	if(i == 8) i=0;
}