#include "System.h"
u8 Display = 0;
u8 Maxtem = 35;
u8 Mintem = 25;   
u8 tem_mod;
u16 led_tem= 0;
sbit Relay = P0^4; sbit Buzzer = P0^6;
bit led_flag = 0;
void Timer2_Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x04;			//定时器时钟1T模式
	T2L = 0x20;				//设置定时初始值
	T2H = 0xD1;				//设置定时初始值
	AUXR |= 0x10;			//定时器2开始计时

    IE2 |= 0X04;
	EA = 1;
}

//键盘读取
u8 key_value;
u16 kbd_time;
u8 temperature;
u8 key_count = 0;
u8 a;
u8 set_temp[8];
void KBD_Process(){
	temperature = Read_Temperature();
	
	if(kbd_time >= 10){
		a = KBD_Read_State();
		//处于三个状态下
		if(temperature < Mintem) {
			tem_mod =0;
			P2 = 0xa0;Relay=0;Buzzer=0;P2 = 0x00;      
			if(led_tem >= 800){
				led_flag =~ led_flag;
				if(led_flag == 1){P2 = 0x80;P0=~0x01;P2 = 0;}
				if(led_flag == 0){P2 = 0x80;P0=0xff;P2 = 0;}
				led_tem =0;
			}
		}
		else if(Mintem < temperature < Maxtem){
			tem_mod=1;
			P2 = 0xa0;Relay=0;Buzzer=0;P2 = 0x00;
			if(led_tem >= 400){
				led_flag =~ led_flag;
				if(led_flag == 1){P2 = 0x80;P0=~0x01;P2 = 0;}
				if(led_flag == 0){P2 = 0x80;P0=0xff;P2 = 0;}
				led_tem =0;
		}
		}
		else if(Maxtem < temperature){
			tem_mod =2;
			P2 = 0xa0;Relay=1;Buzzer=0;P2 = 0x00;
			if(led_tem >= 200){
				led_flag =~ led_flag;
				if(led_flag == 1){P2 = 0x80;P0=~0x01;P2 = 0;}
				if(led_flag == 0){P2 = 0x80;P0=0xff;P2 = 0;}
				led_tem =0;
		}
		}
		if(Display == 0){
				//u8 smg_buf[8] = {0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f};
				smg_buf[0]=0x40;
				smg_buf[1]=smg_code[tem_mod];
				smg_buf[2]=smg_code[Maxtem/10];     //0x40;
				smg_buf[3]=smg_code[Maxtem%10];    //0x00;	
				smg_buf[4]=smg_code[Mintem/10];
				smg_buf[5]=smg_code[Mintem%10]; 
				smg_buf[6]=smg_code[temperature/10];      
				smg_buf[7]=smg_code[temperature%10];	
				if(a == 8)Display =! Display;				
				}
		else{      
				   if(key_count == 2)key_count =5;
				   if(key_count == 7)key_count =0;
				   //u8 smg_buf[8] = {0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f};
				  set_buf[0] = 0x40;
				  set_buf[3] = smg_code[key_count];
				  set_buf[4] = 0x00;
				  set_buf[5] =  0x40;	
					switch(a){                                                                               //设置模式
					case 4: key_count++;set_buf[key_count]=smg_code[9];set_temp[key_count]=9;break;
					case 5: key_count++;set_buf[key_count]=smg_code[6];set_temp[key_count]=6;break;
					case 6: key_count++;set_buf[key_count]=smg_code[3];set_temp[key_count]=3;break;
					case 7: key_count++;set_buf[key_count]=smg_code[0];set_temp[key_count]=0;break;
					case 8: Display =! Display;
								 Maxtem = set_temp[1]*10 + set_temp[2];
								 Mintem = set_temp[6]*10 + set_temp[7];
								if(Maxtem<Mintem ){
									DeviceCtrl(0x80,0xfd);
								}else{
									DeviceCtrl(0x80,0xff);
								}
								break;
					case 9: key_count++;set_buf[key_count]=smg_code[7];set_temp[key_count]=7;break;
					case 10: key_count++;set_buf[key_count]=smg_code[4];set_temp[key_count]=4;break;
					case 11: key_count++;set_buf[key_count]=smg_code[1];set_temp[key_count]=1;break;
					case 12: {set_buf[1] = smg_code[1];set_buf[2] = smg_code[1];set_buf[6] = smg_code[1];set_buf[7] = smg_code[1];break;}
					case 13: key_count++;set_buf[key_count]=smg_code[8];set_temp[key_count]=8;break;
					case 14:key_count++;set_buf[key_count]=smg_code[5];set_temp[key_count]=5;break;
					case 15: key_count++;set_buf[key_count]=smg_code[2];set_temp[key_count]=2;break;
						}
				}
kbd_time = 0;
}

}


int main(){	
	Timer2_Init();
	
	while(1){

	KBD_Process();

}

}
void Timer2_ISR()interrupt 12{
	SMG_Display(Display);
	kbd_time++;
	led_tem++;
}