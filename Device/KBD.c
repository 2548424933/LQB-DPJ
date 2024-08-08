#include "KBD.h"

u8 Key_IO_Pro(){
	u8 key_io = 0xff;                    //-----------!!----Ҫ��ʼ��Ϊ0xff,��Ȼ���û�г�ʼ��������ֵ��δ����ģ���� key_io �ĳ�ʼֵ����������ģ�����ȷ���������ܵ��´������
	P3=0xf0;P4=0Xff;       //��ɨ��
	if(P44==0) key_io=0x70;
	if(P42==0) key_io=0xb0;
	if(P35==0) key_io=0xd0;
	if(P34==0) key_io=0xe0;

	P3=0x0f;P4=0x00;     //��ɨ��
	if(P33==0) key_io=key_io | 0x07;
	if(P32==0) key_io=key_io | 0x0b;
	if(P31==0) key_io=key_io | 0x0d;
	if(P30==0) key_io=key_io | 0x0e;
	
	return key_io;
}

u8 Trg = 0;
u8 Cont = 0;
void KBD_ThreeLine(){
	u8 kbddata = Key_IO_Pro() ^ 0xff;
	Trg = kbddata & (kbddata ^ Cont);
	Cont = kbddata;
}