#include "Device.h"



void DeviceCtrl(u8 p2data,u8 p0data){
	P0 = p0data;
	P2 = P2 & 0x1f |  p2data;
	P2 &= 0x1f;
}

void System_Init(){
	DeviceCtrl(0xa0,0x00);
	DeviceCtrl(0x80,0xff);
}




