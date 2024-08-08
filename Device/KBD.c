#include "KBD.h"
//矩阵按键（三行按键法）
unsigned char Trg_KBD;
unsigned char Cont_KBD;
u8 KBD_Read_KeyIO_ThreeLine(){
		u8 key_io = 0xff;
		P3=0xf0;P4=0xff;
		if(P44==0) key_io=0x70;
		if(P42==0) key_io=0xb0;
		if(P35==0) key_io=0xd0;
		if(P34==0) key_io=0xe0;

		P3=0x0f;P4=0x00;
		if(P33==0) key_io=key_io | 0x07;
		if(P32==0) key_io=key_io | 0x0b;
		if(P31==0) key_io=key_io | 0x0d;
		if(P30==0) key_io=key_io | 0x0e;
		return key_io;
}


unsigned char Trg;
unsigned char Cont;
void KBD_Read_ThreeLine(){
	unsigned char ReadData = KBD_Read_KeyIO_ThreeLine()^0xff;                    
	Trg = ReadData&(ReadData^Cont);
	Cont = ReadData;
}

