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
	
	P3 = 0x1f; P4 = 0x00;          //0x1f�����ε����һ�У�����������һ������
	if(P33 == 0) key_io += 0;
	if(P32 == 0) key_io += 1;
	if(P31 == 0) key_io += 2;
	if(P30 == 0) key_io += 3;
	
	return key_io;
}


u8 KBD_Read_State(){
	static u8 key_state;
	u8 key_io = 0;
	u8 key_return = 0;           //��ʼ��һ�µ���0�ȽϺ�
	key_io = KBD_KeyIO();
	switch(key_state){                    //caseִ����Ҫ��break����Ȼ�����ִ����һ��break
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
	return key_return;              //ע�����switch����
}
