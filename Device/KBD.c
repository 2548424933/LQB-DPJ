#include "KBD.h"

u8 KBD_KeyIO(){
	u8 key_io = 0xff;
	P4=0xff;P3=0xf0;
	if(P44==0) key_io=0x70;
	if(P42==0) key_io=0xb0;
	if(P35==0) key_io=0xd0;
	if(P34==0) key_io=0xe0;
	
	P4=0x00;P3=0x0f;
	if(P33==0) key_io=key_io|0x07;
	if(P32==0) key_io=key_io|0x0b;
	if(P31==0) key_io=key_io|0x0d;
	if(P30==0) key_io=key_io|0x0e;
	
	return key_io;
}


//unsigned char Trg_KBD;
//unsigned char Cont_KBD;
//void KBD_ThreeLine(){
//	unsigned char ReadData = KBD_KeyIO() ^ 0xff;
//	Trg_KBD = ReadData & (ReadData ^ Cont_KBD);
//	Cont_KBD = ReadData;
//}

unsigned char Trg_KBD;
unsigned char Cont_KBD;
void KBD_ThreeLine(){
	unsigned char ReadData = KBD_KeyIO() ^ 0xff;
	Trg_KBD = ReadData & (ReadData ^ Cont_KBD);
	Cont_KBD = ReadData;

}