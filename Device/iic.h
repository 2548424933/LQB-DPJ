#ifndef _IIC_H
#define _IIC_H
#include "System.h"

//void IIC_Start(void); 
//void IIC_Stop(void);  
//bit IIC_WaitAck(void);  
//void IIC_SendAck(bit ackbit); 
//void IIC_SendByte(unsigned char byt); 
//unsigned char IIC_RecByte(void); 

u8 Read_ADC(u8 ctrl_byte);
void Write_EEPROM(u8 add,u8 dat);
u8 Read_EEPROM(u8 add);

#endif