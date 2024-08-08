#include "device.h"

void DeviceCtrl(u8 data2,u8 data1){
	P0 = data1;
	P2 = P2 & 0x1f | data2;
	P2 = P2 & 0x1f;
}



