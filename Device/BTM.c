#include "BTM.h"

//#define	KEY_NO 0
//#define  KEY_DOWN 1
//#define  KEY_UP 2 

//总量 
u16 total = 0;
//总价
u16 money =0;
//传递
u16 n;

u8 m;

void vBTN_Read_State(){
	static u8 key_state = 1;  //按键状态
	u8 key_io = P3 & 0X0f;   //P3 IO口的值
	static u8 key_val = 0;   //返回键值
	switch(key_state){
		case KEY_NO:
			if(key_io != 0x0f) key_state = KEY_DOWN;
			break;
		case KEY_DOWN:
			if(key_io != 0x0f){
				if(key_io==0x0e) {                   //7 继电器开
						vDeviceCtrl(0xa0,0x10);
						m = 1;
				}
				if(key_io==0x0d) {
						vDeviceCtrl(0xa0,0x00);
						m = 0;
				}
//				if(key_io==0x0b) key_val = 5;
//				if(key_io==0x07) key_val = 4;
				key_state = KEY_UP;
			}else{
				key_state = KEY_NO;
			}
			break;
		case KEY_UP:
			if(key_io == 0x0f){
			key_state = KEY_NO;
			break;
			}
				
	}
}

