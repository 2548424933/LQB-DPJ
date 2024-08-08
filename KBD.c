#include "KBD.h"

u8 vKBD_KeyIO(){
	u8 key_io = 0;
	P3 = 0xf0;P4 = 0xff;
	if(P44 == 0)key_io = 4;
	if(P42 == 0)key_io = 8;
	if(P35 == 0)key_io = 12;
	if(P34 == 0)key_io = 16;
	
	P3 = 0x0f;P4 = 0x00;
	if(P30 == 0)key_io += 3;
	if(P31== 0)key_io  += 2;
	if(P32 == 0)key_io  += 1;
	if(P33 == 0)key_io +=  0;
	
	return key_io;
}

u8 vKBD_Read_State(){
	static u8 key_state = KEY_NO;
	 static u8 key_return = 0;
	 u8 key_val = vKBD_KeyIO();
	switch(key_state){
		case  KEY_NO:
			if(key_val != 0x00) key_state = KEY_DOWN;
			break;
		case KEY_DOWN:
			if(key_val != 0x00){
				key_state = KEY_UP;
				key_return = key_val;
				
			}
		case KEY_UP:
			if(key_val == 0x00){
				key_state = KEY_NO;
				key_return = 0;
			}
	
	}
	return key_return;

}