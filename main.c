#include "System.h"
u8 KBD_Time;
u8 Key_Value;
u8 Display = 0;
u8 second; u8 minute; u8 hour;
u8 alarm_second =0; u8 alarm_minute = 0; u8 alarm_hour = 0;
u8 count_seven,count_six,count_five,count_four;
u8 save_hour,save_minute,save_second; 
u8 seven_next,six_next;
void Timer2_Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0xFB;			//定时器时钟12T模式
	T2L = 0x18;				//设置定时初始值
	T2H = 0xFC;				//设置定时初始值
	AUXR |= 0x10;			//定时器2开始计时
	IE2 |= 0x04;
	EA =  1;
}

//温度读取
u32 temperature;
u8 temperature_time;
void Read_Temperature(){
	if(temperature_time >= 100){
		float a;	
		a = Read_Temp();
		temperature = a*10000;
		temperature_time = 0;
	}
}


//时钟读取
void Read_Clock(){
	second = BCDToDec (Read_Ds1302_Byte ( 0x81));
	minute = BCDToDec (Read_Ds1302_Byte ( 0x83));
	hour = BCDToDec (Read_Ds1302_Byte ( 0x85));
}

//闹钟警告
u8 alarm_time_interval;           
u16 alarm_time_total;
bit alarm_flag;
bit alarm_twinkling_flag;
void Alarm_Warning(){
	if(second==0){
		alarm_flag=1;
		P2=0Xa0;P0=0x50;P2=0;
	}else{P2=0Xa0;P0=0x00;P2=0;}
	
//	if(alarm_flag==1){
//			if(alarm_time_interval>=200){
//			alarm_twinkling_flag =~ alarm_twinkling_flag;
//			alarm_time_interval=0;
//			} 
//			if(alarm_twinkling_flag==0){
//				P2=0X80;P0=~0X01;P2=0;
//			}
//			if(alarm_twinkling_flag==1){
//				P2=0X80;P0=0XFF;P2=0;
//			}
//	}
//	
//	if(alarm_time_total>=5000){
//		alarm_flag=0;
//		P2=0X80;P0=0xff;P2=0;
//		alarm_time_total=0;
//	}

}

bit twinkling_flag;
bit DisplayOne_First;
u16 twinkling_time;
void SMG_Process(){
	if(Display == 0){         //时钟
		amg_buf[0] = smg_code[hour/10];
		amg_buf[1] = smg_code[hour%10];
		amg_buf[2] = 0x40;
		amg_buf[3] = smg_code[minute/10];
		amg_buf[4] = smg_code[minute%10];
		amg_buf[5] = 0x40;
		amg_buf[6] = smg_code[second/10];
		amg_buf[7] = smg_code[second%10];
	}
	if(Display == 1){		//时钟设置
		if(count_seven==1){
			if(twinkling_time>=1000){twinkling_flag=~twinkling_flag;twinkling_time=0;}
			if(twinkling_flag==0){
				amg_buf[0] = smg_code[save_hour/10];
				amg_buf[1] = smg_code[save_hour%10];
			}
			if(twinkling_flag==1){
				amg_buf[0] = 0x00;
				amg_buf[1] = 0x00;
			}
		}else{                                                                          //闪烁信号结束数码管要显示
			amg_buf[0] = smg_code[save_hour/10];
			amg_buf[1] = smg_code[save_hour%10];
		}
		amg_buf[2] = 0x40;
		if(count_seven==2){
			if(twinkling_time>=1000){twinkling_flag=~twinkling_flag;twinkling_time=0;}
			if(twinkling_flag==0){
				amg_buf[3] = smg_code[save_minute/10];
				amg_buf[4] = smg_code[save_minute%10];
			}
			if(twinkling_flag==1){
				amg_buf[3] = 0x00;
				amg_buf[4] = 0x00;
			}
		}else{                                                                        
			amg_buf[3] = smg_code[save_minute/10];
			amg_buf[4] = smg_code[save_minute%10];
		}
		amg_buf[5] = 0x40;
			if(count_seven==3){
			if(twinkling_time>=1000){twinkling_flag=~twinkling_flag;twinkling_time=0;}
			if(twinkling_flag==0){
				amg_buf[6] = smg_code[save_second/10];
				amg_buf[7] = smg_code[save_second%10];
			}
			if(twinkling_flag==1){
				amg_buf[6] = 0x00;
				amg_buf[7] = 0x00;
			}
		}else{                                                                          
			amg_buf[6] = smg_code[save_second/10];
			amg_buf[7] = smg_code[save_second%10];
		}
	}
	
		if(Display == 2){		//闹钟设置
			if(count_six==1){
			if(twinkling_time>=1000){twinkling_flag=~twinkling_flag;twinkling_time=0;}
			if(twinkling_flag==0){
				amg_buf[0] = smg_code[alarm_hour/10];
				amg_buf[1] = smg_code[alarm_hour%10];
			}
			if(twinkling_flag==1){
				amg_buf[0] = 0x00;
				amg_buf[1] = 0x00;
			}
		}else{                                                                          //闪烁信号结束数码管要显示
			amg_buf[0] = smg_code[alarm_hour/10];
			amg_buf[1] = smg_code[alarm_hour%10];
		}
		amg_buf[2] = 0x40;
		if(count_six==2){
			if(twinkling_time>=1000){twinkling_flag=~twinkling_flag;twinkling_time=0;}
			if(twinkling_flag==0){
				amg_buf[3] = smg_code[alarm_minute/10];
				amg_buf[4] = smg_code[alarm_minute%10];
			}
			if(twinkling_flag==1){
				amg_buf[3] = 0x00;
				amg_buf[4] = 0x00;
			}
		}else{                                                                        
			amg_buf[3] = smg_code[alarm_minute/10];
			amg_buf[4] = smg_code[alarm_minute%10];
		}
		amg_buf[5] = 0x40;
			if(count_six==3){
			if(twinkling_time>=1000){twinkling_flag=~twinkling_flag;twinkling_time=0;}
			if(twinkling_flag==0){
				amg_buf[6] = smg_code[alarm_second/10];
				amg_buf[7] = smg_code[alarm_second%10];
			}
			if(twinkling_flag==1){
				amg_buf[6] = 0x00;
				amg_buf[7] = 0x00;
			}
		}else{                                                                          
			amg_buf[6] = smg_code[alarm_second/10];
			amg_buf[7] = smg_code[alarm_second%10];
		}
	}
	if(Display == 3){		 //温度
		amg_buf[0] = 0x00;
		amg_buf[1] = 0x00;
		amg_buf[2] = 0x00;
		amg_buf[3] = 0x00;
		amg_buf[4] = 0x00;
		amg_buf[5] = smg_code[temperature/100000];
		amg_buf[6] = smg_code[temperature/10000%10];
		amg_buf[7] = 0x39;
	}


}



void main(){
	Timer2_Init();
	System_Init();            //设备初始化
	hour=23; minute=59;second=55;
	alarm_hour=0;alarm_minute=0;alarm_second=0;
	Clock_Set(hour,minute,second);//时钟初始化
	
	while(1){
	SMG_Process();
	Read_Clock();						//时钟读取
	Read_Temperature();          //温度读取
	
		if(KBD_Time >= 10){
		KBD_Read_ThreeLine();
		KBD_Time = 0;
		if(Trg == 0x81){                   //时钟设置
			Key_Value = 7;
			count_seven++;
			seven_next++;
			if(count_seven==1){                               //从时钟模式第一次进来要保存时间，防止时间走失
				save_hour=hour; save_minute=minute;save_second=second;
				Display=1;
				seven_next=1;

			}	
			if(count_seven==2){seven_next=2;}             //save_hour=select_time是为了保存第一个更改后的数据
			if(count_seven==3){seven_next=3;}
			if(count_seven==4){
			count_seven=0;
			Display=0;
			Clock_Set(save_hour,save_minute,save_second);
			seven_next=0;
			}
			
		}
		if(Trg == 0x82){                                //6
			count_six++;
			six_next++;
			if(count_six==1){                               //从时钟模式第一次进来要保存时间，防止时间走失
				Display=2;
				six_next=1;
			}	
			if(count_six==2){six_next=2;}             //save_hour=select_time是为了保存第一个更改后的数据
			if(count_six==3){six_next=3;}
			if(count_six==4){
			count_six=0;
			Display=0;
			six_next=0;
			}
		}                            
		if(Trg == 0x84){														                        //5
			Key_Value = 5;
			//时钟模式的按键加
			if(seven_next==1){
				if(save_hour<23) save_hour++;
			}
			if(seven_next==2){
				if(save_minute<59) save_minute++;
			} 
			if(seven_next==3){
				if(save_second<59) save_second++;
			}
			//闹钟模式的按键加
			if(six_next==1){
			if(alarm_hour<23) alarm_hour++;
			}
			if(six_next==2){
				if(alarm_minute<59) alarm_minute++;
			} 
			if(six_next==3){
				if(alarm_second<59) alarm_second++;
			}
		}							
		if(Trg == 0x88){														                          //4
			Key_Value = 6;
			//时钟模式的按键减
			if(seven_next==1){
				if(save_hour>0) save_hour--;
			}
			if(seven_next>=0){
				if(save_minute>0) save_minute--;
			} 
			if(seven_next>=0){
				if(save_second>0) save_second--;
			}
			//闹钟模式的按键加
			if(six_next==1){
			if(alarm_hour>0) alarm_hour--;
			}
			if(six_next==2){
				if(alarm_minute>0) alarm_minute--;
			} 
			if(six_next==3){
				if(alarm_second>0) alarm_second--;
			}			
		}
		//按键4松开放下显示不一样的数据
		if(Trg==0 & Cont==0){        
			if(Display==3){
				Display = 0;
				}
		}
		if(Trg!=0){                     //按下任意键闹钟的警告下的LED1会关闭
		alarm_flag=0;
		P2=0X80;P0=0xff;P2=0;
		}
		Alarm_Warning();
		
		}
	

	}

}




void Timer2_ISR() interrupt 12{
		SMG_Display();
		KBD_Time++;
		temperature_time++;
		twinkling_time++;
		alarm_time_interval++;
		alarm_time_total++;
//			//闹钟警告          指示灯L1
//		Alarm_Warning();
}