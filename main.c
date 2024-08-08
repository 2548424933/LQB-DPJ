#include "System.h"
u8 Display = 0;
u8 CJ_time[4] = {1,5,30,60};
u8 CJ;
u8 c;
sbit LED1 = P0^0;
u8 Mode=1;
void Timer2_Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x04;			//定时器时钟1T模式
	T2L = 0x20;				//设置定时初始值
	T2H = 0xD1;				//设置定时初始值
	AUXR |= 0x10;			//定时器2开始计时
	IE2 = 0x04;
	EA = 1;
}



//温度读取DS18B20
u8 temp[10];
u16 temp_time = 0;
u8 i = 0;
u16 LED1_time;
bit LED1_flag=0;             //闪烁标志
bit LED1_switch=0;              //S6控制的开关
void Temperature_D1(){
		if(Mode == 2){
			if(i <10 && temp_time>=CJ*100){
			temp[i] = Read_Temperature();
			temp_time = 0;
			i++;
			}
			if(i==10){                                   //当i=10的时候就不检测提取了
				i = 0;
				Display =2;                                    //进入温度采集显示界面，LED1闪烁	
				if(LED1_time>=500){
					LED1_flag=~LED1_flag;
					if(LED1_flag==0){
					DeviceCtrl(0x80,~0x01);
					}else{DeviceCtrl(0x80,0xff);}
					LED1_time=0;
							
				}	
		}
}
}
u8 indexes = 0;
u8 datatemp = 0;
u16 datatemp_time;

void Temperature_D2(){
	if(Mode == 3){
		
		if(datatemp_time>=1000 && indexes<10){
		
		datatemp = temp[indexes];
		indexes++;
		datatemp_time=0;
		}
		if(indexes == 10){
		indexes=0;
		Mode =4;
		} 
	}

}

//键盘读取
u8 key_value;
u8 key_time;
u8 indexes;      //索引

void KBD_Process(){
	if(key_time >= 10){
		key_value = KBD_Read_State();
		key_time =0;
		if(key_value == 4&&Mode == 1){                   //按键 S4 切换 4 个温度采集间隔时间，分别为 1 秒、5 秒、30 秒和 60 秒；
			
			Display=0;  
			CJ= CJ_time[c];
			c++;
			if(c ==4){
			 c=0;
			};
		}
		if(key_value == 5&&Mode == 1){                       //按下按键 S5 ，进入时钟显示界并开始采集温度。
		Display=1;                                 //进入Display1界面并激活Temperature_D1
		Mode = 2;
		}
		Temperature_D1();               //出来后Mode不变2采集完十个数据可以进入温度采集显示界面Display2
		Temperature_D2();				//出来后Mode=4

		if(key_value == 6&&Mode == 2){
		DeviceCtrl(0x80,0xff);    //LED1停止闪烁
		Mode = 3;
		
		}
		if(key_value == 7&&Mode == 4){
			Mode = 1;
			Display=0;
		}

	}
}

//时钟读取
u8 hour,minute,second;
u16 ds1302_time;
bit Smg_Tsf = 0;
void DS1302_Process(){
		second = BCDToDec(Read_Ds1302_Byte(0x81));
		minute = BCDToDec(Read_Ds1302_Byte(0x83));
		hour = BCDToDec(Read_Ds1302_Byte(0x85));
}

u16 SMG_time;
void SMG_Process(){
	if(Display == 0){
			smg_buf[0]= 0x00;
			smg_buf[1] = 0x00;
			smg_buf[2] = 0x00;
			smg_buf[3] = 0x00;	
			smg_buf[4]= 0x00;
			smg_buf[5] = 0x40;
			smg_buf[6] = smg_code[CJ/10];
			smg_buf[7] = smg_code[CJ%10];
	}
	else if(Display == 1){
			smg_buf[0]= smg_code[i/10];
			smg_buf[1] = smg_code[i%10];
			smg_buf[3] = smg_code[minute/10];
			smg_buf[4]= smg_code[minute%10];
			smg_buf[6] = smg_code[second/10];
			smg_buf[7] = smg_code[second%10];
			if(SMG_time>=1000){
			Smg_Tsf  =~ Smg_Tsf ;
			if(Smg_Tsf==0){
			smg_buf[2] = 0x40;
			smg_buf[5] =  0x40;
			}else{
			smg_buf[2] = 0x00;
			smg_buf[5] =  0x00;}
			SMG_time=0;
			}
	}
	else if(Display == 2){
			smg_buf[0]= 0x40;
			smg_buf[1] = smg_code[indexes/10];
			smg_buf[2] = smg_code[indexes%10];
			smg_buf[3] = 0x00;
			smg_buf[4]= 0x00;
			smg_buf[5] = 0x40;
			smg_buf[6] = smg_code[datatemp/10];
			smg_buf[7] = smg_code[datatemp%10];
	}

}


void main(){
	Timer2_Init();
	 Clock_Set(23,59,50);
	while(1){
		SMG_Process();
		KBD_Process();
		DS1302_Process();
		
	}
}

void Timer2_ISR()interrupt 12{
	SMG_Display();
	key_time++;
	temp_time++;
	ds1302_time++;
	LED1_time++;
	SMG_time++;
	datatemp_time++;
}