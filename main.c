#include "System.h"
sbit relay = P0^4;
sbit buzzer = P0^6;
sbit LED1 = P0^0;
sbit LED2 = P0^1;
sbit LED3 = P0^7;
bit buzzer_flag,relay_flag,buzzer_beep;
u8 display;//显示的界面

void SMG_Process(void);

//ds1302时钟读取
u8 hour,minute,second;
void DS1302_Process(void){
	minute = BCDToDec(Read_Ds1302_Byte(0x83));
	hour = BCDToDec(Read_Ds1302_Byte(0x85));
}

//电位器 Rb2读取
u8 ch1;       //湿度值
u8 ch2;     //湿度阀值
u8 ch3;     //湿度阀值判断
u8 Read_ADC_Process(){
		u8 a;
		//EA = 0;
		a = Read_ADC(0x43)/2.57f;
		//EA = 1;
		return a;                 //ch1
}

//湿度值判断条件
void Pan_Process(bit buzzer_flag,bit relay_flag){
		if(buzzer_flag == 0 && relay_flag == 0){          ///           
			P2 = 0xa0;buzzer=0;relay=0;P2=0x00;
		}
		if(buzzer_flag == 0 && relay_flag == 1){              
			P2 = 0xa0;buzzer=0;relay=1;P2=0x00;
		}
		if(buzzer_flag == 1 && relay_flag == 0){ 
			P2 = 0xa0;buzzer=1;relay=0;P2=0x00;
		}
		if(buzzer_flag == 1 && relay_flag == 1){ 
			P2 = 0xa0;buzzer=1;relay=1;P2=0x00;
		}
}

//手动状态
u8 key_value;
void Hand_State(){
	//ch2 = Read_EEPROM(0x00);
	relay_flag = 0 ;buzzer_flag=0;
	P2 = 0x80; P0 = 0Xff;LED2 = 0; LED1 = 1;P2 = 0x00;            //LED2亮
	while(SYS){
		SMG_Process();
		DS1302_Process();                     //时钟
		    
		ch1 = Read_ADC_Process();		         //湿度ch1
		Pan_Process(buzzer_flag,relay_flag);          //湿度值判断          如果.....就不会判断（里面有判断条件）
		
		key_value = BTN_Read_State();
		if(key_value == 7) SYS =! SYS;
		if(key_value == 6){
			buzzer_beep =~ buzzer_beep;
		};//开关蜂鸣器提醒
//		if(key_value == 5) {relay_flag = 0;}     //继电器开
//		if(key_value == 4) {relay_flag = 1;}    //继电器关

			if(ch1 > ch2&&buzzer_beep == 0 ){                                  //根据湿度数据自动打开蜂鸣器
				buzzer_flag= 1;
			}else{
				buzzer_flag = 0;
			}	

		}
		
	
}

//自动状态
void AUTO_State(){
	//ch2 = Read_EEPROM(0x00);
	relay_flag = 0 ;buzzer_flag=0;
	P2 = 0x80; P0 = 0Xff;LED2 = 1; LED1 = 0;P2 = 0x00;       //LED1亮
	while(SYS == 0){
	SMG_Process();
	DS1302_Process();                     //时钟
	ch1 = Read_ADC_Process();		         //湿度ch1
	Pan_Process(buzzer_flag,relay_flag);          //湿度值判断          如果.....就不会判断（里面有判断条件）
	
	key_value = BTN_Read_State();
	if(key_value == 7){
		Write_EEROM(0x00,ch2);  
		SYS =! SYS;
		
		display = 0;
		        
	}
	if(key_value == 6){       
	
			if(display == 0){          //进入湿度调节界面   键5键4可以调节
				display = 1;
			}else if(display == 1){      
				display = 0;
				Write_EEROM(0x00,ch2);			//退出湿度调节界面,并保存到EEPROM中
			}
	};
	if(display == 1){
				if(key_value == 5){
				ch2 += 5;
				};
				if(key_value == 4){
				ch2 -=5;
				};
			}
		
	 if(ch1 > ch2 ){                               //根据湿度数据自动控制打开或关闭灌溉设备
				relay_flag = 1;
			}else{
				relay_flag = 0;
			}
}
	
}

void SMG_Process(){
	if(display == 0){                          //自动状态下的S6键切换界面
			smg_buf[0] =smg_code[hour/10];
			smg_buf[1] = smg_code[hour%10];
			smg_buf[2] = smg_code[ch2/100] ;              //0x40
			smg_buf[3] = smg_code[ch2/10%10];        //minute
			smg_buf[4] = smg_code[ch2%10];
			smg_buf[5] = 0x00;
			smg_buf[6] = smg_code[ch1/10%10];
			smg_buf[7] =  smg_code[ch1%10];
	}else{
			smg_buf[0] = smg_code[key_value];
			smg_buf[1] =  smg_code[SYS];
			smg_buf[2] = 0x00;
			smg_buf[3] = 0x00;
			smg_buf[4] = 0x00;
			smg_buf[5] =smg_code[ch2/100];
			smg_buf[6] =  smg_code[ch2/10%10];
			smg_buf[7] =  smg_code[ch2%10];
	}

	
	
}

void main(){
	System_Init();
	Timer2_Init();
	Clock_Set(8,30,20);
	//printf("1");
	ch2 = Read_EEPROM(0x00);          //开机时读取一次rom的值,ch1湿度阈值------------0x00就行--------
	while(1){
		AUTO_State();
		Hand_State();
	}
}



void Timer2() interrupt 12{
	SMG_Display();

}





//#include "System.h"
//sbit relay = P0^4;
//sbit buzzer = P0^6;

//u8 display;  // 显示的界面
//void SMG_Process(void);

// ds1302 时钟读取
//u8 hour, minute, second;
//void DS1302_Process(void) {
//    minute = BCDToDec(Read_Ds1302_Byte(0x83));
//    hour = BCDToDec(Read_Ds1302_Byte(0x85));
//}

// 电位器 Rb2 读取
//u8 ch1;   // 湿度值
//u8 ch2;   // 湿度阀值
//u8 Read_ADC_Process() {
//    u8 a;
//    EA = 0;
//    a = Read_ADC(0x43) / 2.57f;
//    EA = 1;
//    return a; // ch1
//}

// 湿度值判断条件
//void Pan_Process(u8 ch1, u8 ch2) {
//    if (ch1 > ch2) {
//        // 蜂鸣器响
//        DeviceCtrl(0x80, 0x7f);
//    }
//}

// 手动状态
//u8 key_value;
//bit buzzer_state = 0;

//void Hand_State() {
//    while (SYS) {
//        SMG_Process();
//        DS1302_Process(); // 时钟
//        EA = 0;
//        DeviceCtrl(0x80,0xfd); // LED2 亮
//        EA = 1;

//        ch1 = Read_ADC_Process(); // 湿度 ch1
//        Pan_Process(ch1, ch2); // 湿度值判断

//        key_value = BTN_Read_State();
//        if (key_value == 7) {
//            Write_EEROM(0x00, ch2); // 切换到自动模式前保存湿度阀值
//            SYS = !SYS; // 切换工作状态
//        }
//        if (key_value == 6) buzzer_state = !buzzer_state; // 开关蜂鸣器提醒
//        if (key_value == 5) { P2 = 0xa0; relay = 0; buzzer = 0; P2 = 0x00; } // 继电器开
//        if (key_value == 4) { P2 = 0xa0; relay = 1; buzzer = 0; P2 = 0x00; } // 继电器关
//    }
//}

// 自动状态
//void AUTO_State() {
//    while (SYS == 0) {
//        SMG_Process();
//        DS1302_Process(); // 时钟
//        EA = 0;
//       DeviceCtrl(0x80,0xfe); // LED1 亮
//        EA = 1;

//        ch1 = Read_ADC_Process(); // 湿度 ch1
//        Pan_Process(ch1, ch2); // 湿度值判断

//        key_value = BTN_Read_State();
//        if (key_value == 7) {
//            Write_EEROM(0x00, ch2); // 保存 ch2 到 EEPROM
//            SYS = !SYS;
//            display = 0;
//        }
//        if (key_value == 6) { // 进入或退出湿度调节界面
//            if (display == 0) {
//                display = 1;
//            } else if (display == 1) {
//                display = 0;
//                Write_EEROM(0x00, ch2); // 保存湿度阀值到 EEPROM
//            }
//        }
//        if (display == 1) { // 湿度阀值调整界面
//            if (key_value == 5) {
//                ch2 += 1; // 增加湿度阀值
//            }
//            if (key_value == 4) {
//                ch2 -= 1; // 减少湿度阀值
//            }
//        }
//    }
//}

//void SMG_Process() {
//    if (display == 0) { // 自动状态下的 S6 键切换界面
//        smg_buf[0] = smg_code[hour / 10];
//        smg_buf[1] = smg_code[hour % 10];
//        smg_buf[2] = smg_code[ch2 / 100];
//        smg_buf[3] = smg_code[ch2 / 10 % 10];
//        smg_buf[4] = smg_code[ch2 % 10];
//        smg_buf[5] = 0x00;
//        smg_buf[6] = smg_code[ch1 / 10 % 10];
//        smg_buf[7] = smg_code[ch1 % 10];
//    } else {
//        smg_buf[0] = smg_code[key_value];
//        smg_buf[1] = smg_code[SYS];
//        smg_buf[2] = 0x00;
//        smg_buf[3] = 0x00;
//        smg_buf[4] = 0x00;
//        smg_buf[5] = smg_code[ch2 / 100];
//        smg_buf[6] = smg_code[ch2 / 10 % 10];
//        smg_buf[7] = smg_code[ch2 % 10];
//    }
//}

//void main() {
//    System_Init();
//    Timer2_Init();
//    Clock_Set(8, 30, 20);
//    
//	if (Read_EEPROM(0x10) == 128) { // 假设 128 是默认值或错误值
//    Write_EEROM(0x10, 50); // 设置一个默认值，例如 50
//}
//	ch2 = Read_EEPROM(0x00); // 开机时读取一次 EEPROM 的值，ch1 湿度阈值
//    while (1) {
//        if (SYS == 0) {
//            AUTO_State();
//        } else {
//            Hand_State();
//        }
//    }
//}

//void Timer2() interrupt 12 {
//    SMG_Display();
//}

