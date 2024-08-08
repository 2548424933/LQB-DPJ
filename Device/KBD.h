#ifndef __KBD_H_
#define __KBD_H_
#include "System.h"



u8 Read_KeyIO();
u8 KBD_Read_State();


extern unsigned char Trg;
extern unsigned char Cont;
u8 KBD_Read_KeyIO_ThreeLine();
void KBD_Read_ThreeLine();

#endif