#include "BTN.h"

#define KEY_NO 0
#define KEY_DOWN 1
#define KEY_UP 2

//u8 BTN_Read_State(){
//	static u8 key_state = 0;
//	static u8 key_val = 0;
//	P3 = P3&0x0f;
//	switch(key_state){
//		case KEY_NO:
//		if(P3 != 0x0f) key_state = KEY_DOWN;
//		break;
//		
//		case KEY_DOWN:
//		if(P3 != 0x0f){
//			if(P3 == 0x0e) key_val = 7;
//			if(P3 == 0x0d) key_val = 6;
//			if(P3 == 0x0b) key_val = 5;
//			if(P3 == 0x07) key_val = 4;
//			key_state = KEY_UP;
//			}
//		else key_state = KEY_NO;
//		break;
//	
//		case KEY_UP:
//		if(P3 == 0x0f) {
//		key_state = KEY_NO;
//		key_val = 0;
//		}
//		else key_val = 0;
//		break;
//	}
//	return key_val;
//}

u8 BTN_Read_State(){
	static u8 key_state = 0;  //按键状态
	u8 key_io = P3 & 0X0f;   //P3 IO口的值
	u8 key_val = 0;   //返回键值
	switch(key_state){
		case KEY_NO:
			if(key_io != 0x0f) key_state = KEY_DOWN;
			break;
		case KEY_DOWN:
			if(key_io != 0x0f){
				if(key_io==0x0e) key_val = 7;
				if(key_io==0x0d) key_val = 6;
				if(key_io==0x0b) key_val = 5;
				if(key_io==0x07) key_val = 4;
				key_state = KEY_UP;
			}else{
				key_state = KEY_NO;
			}
			break;
		case KEY_UP:
			if(key_io == 0x0f){
			key_state = KEY_NO;
			key_val = 0;
			break;
			}
			
			
	}
	return key_val;
}
