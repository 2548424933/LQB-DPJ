#include "System.h"



//独立按键  
 //光敏电阻    
u8 key_value =0;
u8 count_time = 0;
void BTN_Process(){
		if(count_time == 5){
			vBTN_Read_State();     
			count_time = 0;
			
		} 
}

//矩阵按键
//u8 key_value =0;
//u8 count_time = 0;
//void KBD_Process(){
//		if(count_time == 10){
//			key_value =  vKBD_Read_State();          
//			count_time = 0;
//			
//		} 
//}



u8 ch1 = 0;
u8 count_time2;
void vADC_Process(){
	if(count_time2 == 10){
		vRead_ADC(0x41);
		ch1 = vRead_ADC(0x41);
		if(ch1 < 125){
			vDeviceCtrl(0x80,0xfe);
		}else{
			vDeviceCtrl(0x80,0xff);
		}
		count_time2 = 0;

	}
		
}
 

//数码管
void vSMG_Process(){
	smg_buf[0] = 0x00;
	smg_buf[1] = smg_code[0]|0x80;
	smg_buf[2] = smg_code[5];
	smg_buf[3] = smg_code[0];
	
	if(m == 1){
	n = total;
	}
	if(m == 0){
	n = money;
	total = 0;
	}
	smg_buf[4] = smg_code[n/1000];
	smg_buf[5] = smg_code[n/100%10]|0x80 ;
	smg_buf[6] = smg_code[n/10%10];
	smg_buf[7] = smg_code[n%10];
//	smg_buf[5] = smg_code[ch1/100];
//	smg_buf[6] = smg_code[ch1/10%10];
//	smg_buf[7] = smg_code[ch1%10];
}

u16 count_time3;
void main(){
	vSystem_Init();
	Timer2_Init();                        //--------定时器要加
	
	while(1){
		BTN_Process();
		vSMG_Process();
		vADC_Process();
		if(m == 1){
			if(count_time3 >= 100){
				total += 10;
				money = total*0.5;
				count_time3 = 0;
			}
		}
		
		
	}
}



//中断服务函数
void Timer2() interrupt 12{
		count_time++;
		count_time2++;
		count_time3++;
		vSMG_Display();
}