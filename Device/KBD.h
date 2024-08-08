#ifndef __KBD_H_
#define __KBD_H_
#include "System.h"

extern unsigned char Trg_KBD;
extern unsigned char Cont_KBD;

u8 KBD_KeyIO();
void KBD_ThreeLine();

#endif