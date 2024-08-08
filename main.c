#include "System.h"
sbit relay = P0^4;
sbit buzzer = P0^6;
sbit LED1 = P0^0;
sbit LED2 = P0^1;
sbit LED3 = P0^7;
bit buzzer_flag,relay_flag,buzzer_beep;
u8 display;//��ʾ�Ľ���

void SMG_Process(void);

//ds1302ʱ�Ӷ�ȡ
u8 hour,minute,second;
void DS1302_Process(void){
	minute = BCDToDec(Read_Ds1302_Byte(0x83));
	hour = BCDToDec(Read_Ds1302_Byte(0x85));
}

//��λ�� Rb2��ȡ
u8 ch1;       //ʪ��ֵ
u8 ch2;     //ʪ�ȷ�ֵ
u8 ch3;     //ʪ�ȷ�ֵ�ж�
u8 Read_ADC_Process(){
		u8 a;
		//EA = 0;
		a = Read_ADC(0x43)/2.57f;
		//EA = 1;
		return a;                 //ch1
}

//ʪ��ֵ�ж�����
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

//�ֶ�״̬
u8 key_value;
void Hand_State(){
	//ch2 = Read_EEPROM(0x00);
	relay_flag = 0 ;buzzer_flag=0;
	P2 = 0x80; P0 = 0Xff;LED2 = 0; LED1 = 1;P2 = 0x00;            //LED2��
	while(SYS){
		SMG_Process();
		DS1302_Process();                     //ʱ��
		    
		ch1 = Read_ADC_Process();		         //ʪ��ch1
		Pan_Process(buzzer_flag,relay_flag);          //ʪ��ֵ�ж�          ���.....�Ͳ����жϣ��������ж�������
		
		key_value = BTN_Read_State();
		if(key_value == 7) SYS =! SYS;
		if(key_value == 6){
			buzzer_beep =~ buzzer_beep;
		};//���ط���������
//		if(key_value == 5) {relay_flag = 0;}     //�̵�����
//		if(key_value == 4) {relay_flag = 1;}    //�̵�����

			if(ch1 > ch2&&buzzer_beep == 0 ){                                  //����ʪ�������Զ��򿪷�����
				buzzer_flag= 1;
			}else{
				buzzer_flag = 0;
			}	

		}
		
	
}

//�Զ�״̬
void AUTO_State(){
	//ch2 = Read_EEPROM(0x00);
	relay_flag = 0 ;buzzer_flag=0;
	P2 = 0x80; P0 = 0Xff;LED2 = 1; LED1 = 0;P2 = 0x00;       //LED1��
	while(SYS == 0){
	SMG_Process();
	DS1302_Process();                     //ʱ��
	ch1 = Read_ADC_Process();		         //ʪ��ch1
	Pan_Process(buzzer_flag,relay_flag);          //ʪ��ֵ�ж�          ���.....�Ͳ����жϣ��������ж�������
	
	key_value = BTN_Read_State();
	if(key_value == 7){
		Write_EEROM(0x00,ch2);  
		SYS =! SYS;
		
		display = 0;
		        
	}
	if(key_value == 6){       
	
			if(display == 0){          //����ʪ�ȵ��ڽ���   ��5��4���Ե���
				display = 1;
			}else if(display == 1){      
				display = 0;
				Write_EEROM(0x00,ch2);			//�˳�ʪ�ȵ��ڽ���,�����浽EEPROM��
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
		
	 if(ch1 > ch2 ){                               //����ʪ�������Զ����ƴ򿪻�رչ���豸
				relay_flag = 1;
			}else{
				relay_flag = 0;
			}
}
	
}

void SMG_Process(){
	if(display == 0){                          //�Զ�״̬�µ�S6���л�����
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
	ch2 = Read_EEPROM(0x00);          //����ʱ��ȡһ��rom��ֵ,ch1ʪ����ֵ------------0x00����--------
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

//u8 display;  // ��ʾ�Ľ���
//void SMG_Process(void);

// ds1302 ʱ�Ӷ�ȡ
//u8 hour, minute, second;
//void DS1302_Process(void) {
//    minute = BCDToDec(Read_Ds1302_Byte(0x83));
//    hour = BCDToDec(Read_Ds1302_Byte(0x85));
//}

// ��λ�� Rb2 ��ȡ
//u8 ch1;   // ʪ��ֵ
//u8 ch2;   // ʪ�ȷ�ֵ
//u8 Read_ADC_Process() {
//    u8 a;
//    EA = 0;
//    a = Read_ADC(0x43) / 2.57f;
//    EA = 1;
//    return a; // ch1
//}

// ʪ��ֵ�ж�����
//void Pan_Process(u8 ch1, u8 ch2) {
//    if (ch1 > ch2) {
//        // ��������
//        DeviceCtrl(0x80, 0x7f);
//    }
//}

// �ֶ�״̬
//u8 key_value;
//bit buzzer_state = 0;

//void Hand_State() {
//    while (SYS) {
//        SMG_Process();
//        DS1302_Process(); // ʱ��
//        EA = 0;
//        DeviceCtrl(0x80,0xfd); // LED2 ��
//        EA = 1;

//        ch1 = Read_ADC_Process(); // ʪ�� ch1
//        Pan_Process(ch1, ch2); // ʪ��ֵ�ж�

//        key_value = BTN_Read_State();
//        if (key_value == 7) {
//            Write_EEROM(0x00, ch2); // �л����Զ�ģʽǰ����ʪ�ȷ�ֵ
//            SYS = !SYS; // �л�����״̬
//        }
//        if (key_value == 6) buzzer_state = !buzzer_state; // ���ط���������
//        if (key_value == 5) { P2 = 0xa0; relay = 0; buzzer = 0; P2 = 0x00; } // �̵�����
//        if (key_value == 4) { P2 = 0xa0; relay = 1; buzzer = 0; P2 = 0x00; } // �̵�����
//    }
//}

// �Զ�״̬
//void AUTO_State() {
//    while (SYS == 0) {
//        SMG_Process();
//        DS1302_Process(); // ʱ��
//        EA = 0;
//       DeviceCtrl(0x80,0xfe); // LED1 ��
//        EA = 1;

//        ch1 = Read_ADC_Process(); // ʪ�� ch1
//        Pan_Process(ch1, ch2); // ʪ��ֵ�ж�

//        key_value = BTN_Read_State();
//        if (key_value == 7) {
//            Write_EEROM(0x00, ch2); // ���� ch2 �� EEPROM
//            SYS = !SYS;
//            display = 0;
//        }
//        if (key_value == 6) { // ������˳�ʪ�ȵ��ڽ���
//            if (display == 0) {
//                display = 1;
//            } else if (display == 1) {
//                display = 0;
//                Write_EEROM(0x00, ch2); // ����ʪ�ȷ�ֵ�� EEPROM
//            }
//        }
//        if (display == 1) { // ʪ�ȷ�ֵ��������
//            if (key_value == 5) {
//                ch2 += 1; // ����ʪ�ȷ�ֵ
//            }
//            if (key_value == 4) {
//                ch2 -= 1; // ����ʪ�ȷ�ֵ
//            }
//        }
//    }
//}

//void SMG_Process() {
//    if (display == 0) { // �Զ�״̬�µ� S6 ���л�����
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
//	if (Read_EEPROM(0x10) == 128) { // ���� 128 ��Ĭ��ֵ�����ֵ
//    Write_EEROM(0x10, 50); // ����һ��Ĭ��ֵ������ 50
//}
//	ch2 = Read_EEPROM(0x00); // ����ʱ��ȡһ�� EEPROM ��ֵ��ch1 ʪ����ֵ
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

