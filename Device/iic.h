#ifndef _IIC_H
#define _IIC_H

#include "System.h"
//void IIC_Start(void); 
//void IIC_Stop(void);  
//bit IIC_WaitAck(void);  
//void IIC_SendAck(bit ackbit); 
//void IIC_SendByte(unsigned char byt); 
//unsigned char IIC_RecByte(void); 


void vWrite_EEPROM(u8 add,u8 dat);
u8 ucRead_EEPROM(u8 add);
u8 vRead_ADC(u8 ctrl_byte);
void vWrite_DAC(u8 ctrl_byte,u8 dat);



u8 Read_ADC(u8 ctrl_byte);
void Write_EEROM(u8 add,u8 dat);
u8 Read_EEPROM(u8 add);
#endif