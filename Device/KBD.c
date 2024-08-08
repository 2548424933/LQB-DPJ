#include "KBD.h"
#define KEY_NO 0
#define KEY_DOWN 1
#define KEY_UP 2


u8 KBD_Read_State(){
	static u8 key_state = KEY_NO;
	u8 key_val = 0;
	P3 = 0x0f;
	switch(key_state){
		case KEY_NO:
		if(P3 != 0x0f){
			key_state = KEY_DOWN;
		}
		break;
		case KEY_DOWN:
		if(P3 != 0x0f){
			key_state = KEY_UP;
			if(P3 == 0x07) key_val = 4;
			if(P3 == 0x0b) key_val = 5;
			if(P3 == 0x0d) key_val = 6;
			if(P3 == 0x0e) key_val = 7;
		}else{key_state=KEY_NO;}
		break;
		case KEY_UP:
		if(P3 == 0x0f){
			key_state = KEY_NO;
		}
		break;
	}
	return key_val;
	
	
}
