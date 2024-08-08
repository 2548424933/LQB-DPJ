#include "KBD.h"

#define KEY_NO 0
#define KEY_DOWN 1
#define KEY_UP 2

u8 KBD_KeyIO(){
	u8 key_io = 0;
	P3 = 0xf0; P4 = 0xff;              //0xe0
	if(P44 == 0) key_io = 4;
	if(P42 == 0) key_io = 8;
	if(P35 == 0) key_io = 12;
	//if(P34 == 0) key_io = 16;           
	
	P3 = 0x1f; P4 = 0x00;          //0x1f是屏蔽掉最后一列，还有上面那一个步骤
	if(P33 == 0) key_io += 0;
	if(P32 == 0) key_io += 1;
	if(P31 == 0) key_io += 2;
	if(P30 == 0) key_io += 3;
	
	return key_io;
}


u8 KBD_Read_State(){
	static u8 key_state;
	u8 key_io = 0;
	u8 key_return = 0;           //初始化一下等于0比较好
	key_io = KBD_KeyIO();
	switch(key_state){                    //case执行完要加break，不然会继续执行下一关break
	case KEY_NO:
	if(key_io){
		key_state = KEY_DOWN;
		
	}break;
	
	case KEY_DOWN:
	if(key_io){
		key_state = KEY_UP;
		key_return = key_io;
	}else{key_state = KEY_NO;}
	break;
	case KEY_UP:
	if(key_io == 0){
		key_state = KEY_NO;
	}
	break;
}
	return key_return;              //注意放在switch外面
}
