#include "BTN.h"
#define KEY_NO 0
#define KEY_UP 1
#define KEY_DOWN 2

u8 SYS = 0;

u8 BTN_Read_State(){
	static u8 key_state = KEY_NO;
	u8 key_io = P3 & 0x0f;
	u8 key_val;             //static u8 key_val;
	switch(key_state){
		case KEY_NO:
		if(key_io) key_state = KEY_UP;
		case KEY_DOWN:
		if(key_io){
			if(key_io == 0x0e){               //按键7切换状态      0为自动
//				if(SYS == 0) {
//					SYS = 1;
//				}else	SYS = 0;		

//				SYS = !SYS;      //按位取反
				key_val = 7;
			}  ;              
			if(key_io == 0x0d) key_val = 6;
			if(key_io == 0x0b) key_val = 5;
			if(key_io == 0x07) key_val = 4;
			key_state = KEY_UP;
			
		}else{
			key_state = KEY_NO;
		}
		case KEY_UP:
		if(key_io ==  0x0f){
		key_state = KEY_NO;
		key_val = 0;
		}
	}
	return key_val;
}