#include "Device.h"

void vSystem_Init(){
//���������̵�����ʼ�� �����Ҳ��պ�
	P0 = 0;         
	P2 = 0xa0;
	P2 = 0;
}



void vDeviceCtrl(u8 p2data,u8 p0data){
	P0 = p0data;
	P2 = P2 & 0x1f | p2data;
	P2 = P2 & 0x1f;
}