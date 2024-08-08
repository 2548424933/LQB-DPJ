#include "System.h"
u8 key_time;
u8 Display = 0;
u8 adc_val;
u16 adc_time;

u8 pwm_time;
u8 pwm_duty;

u8 twinkling_flag;
u16 twinkling_time;
u8 i = 0;
bit twinkling;

u8 kbd_six_count=0;

u8 a;

void Timer2_Init(void)		//1����@12.000MHz
{
	AUXR &= 0xFB;			//��ʱ��ʱ��12Tģʽ
	T2L = 0x18;				//���ö�ʱ��ʼֵ
	T2H = 0xFC;				//���ö�ʱ��ʼֵ
	AUXR |= 0x10;			//��ʱ��2��ʼ��ʱ
//----------�ǵ�Ҫ����������------------------
	IE2 |= 0x04;                                                     //����ʱ��2�ж�
	EA = 1;															 //�������ж�
}

//ADC��ȡ
void ADC_Read_Process(){
	if(adc_time >=500){
		adc_time =0;
		adc_val = Read_ADC(0x43)/51;         //��ȡPCF8591 �趨Ϊ0~5v
	}

}

u16 LED_Interval;
u16 LED_Intervaltime[4];
u8 LED_Mode=1;
u8 ledshift;
u8 led_ctrl = 0xff;
bit LED_flag=1;
//LED��״̬
void LED_Process(){
	if(LED_flag==1){
		if(LED_Mode==1){
			if(LED_Interval>=LED_Intervaltime[i]){
				led_ctrl =  ~(0x01<<ledshift);
				ledshift++;
				if(ledshift==8) ledshift=0;
				LED_Interval=0;
			}
		}
		if(LED_Mode==2){
			if(LED_Interval>=LED_Intervaltime[i]){
				led_ctrl = ~(0x80 >>ledshift);
				ledshift++;
				if(ledshift==8) ledshift=0;
				LED_Interval=0;
			}
		}
		if(LED_Mode==3){
			if(LED_Interval>=LED_Intervaltime[i]){
				led_ctrl = ~((0x01<<ledshift) | (0x80>>ledshift));
				ledshift++;
				if(ledshift==4) ledshift=0;
				LED_Interval=0;
			}
		}
		if(LED_Mode==4){
			if(LED_Interval>=LED_Intervaltime[i]){
				led_ctrl = ~((0x08>>ledshift) | (0x10<<ledshift));
				ledshift++;
				if(ledshift==4) ledshift=0;
				LED_Interval=0;
			}
		}
	
	}
	Device_Ctrl(0x80,led_ctrl);
}

//PWM����
void PWM_Process(u8 adc_val){
	if(0<=adc_val && adc_val<=1) pwm_duty = 20;
	if(1<adc_val && adc_val<=2) pwm_duty = 40;
	if(2<adc_val && adc_val<=3) pwm_duty = 60;
	if(3<adc_val && adc_val<=5) pwm_duty = 80;
	pwm_time = pwm_time%10 +1;
	if(pwm_time <= (pwm_duty/10)){
		Device_Ctrl(0x80,led_ctrl);
	}
	else{
		Device_Ctrl(0x80,0xff);
	}
}

//��ȡEEPROM      ��һ�ο���ʱ��
void RRead_EEPROM(){
	if(Read_EEPROM(0x05) != 0xff && Read_EEPROM(0x06) != 0xff){							//�������û�����ݾͲ���ȡ��
		LED_Intervaltime[0] = (Read_EEPROM(0x01)<<8) | Read_EEPROM(0x00);
		LED_Intervaltime[1] = (Read_EEPROM(0x03)<<8) | Read_EEPROM(0x02);
		LED_Intervaltime[2] = (Read_EEPROM(0x05)<<8) | Read_EEPROM(0x04);
		LED_Intervaltime[3] = (Read_EEPROM(0x07)<<8) | Read_EEPROM(0x06);
	}
	else{
		LED_Intervaltime[0] = 400;
		LED_Intervaltime[1] = 400;
		LED_Intervaltime[2] = 400;
		LED_Intervaltime[3] = 400;
	}
}



//����ܺ���
u8 key_value;
void SMG_Process(){
	if(Display==0){
	amg_buf[0] = smg_code[LED_Intervaltime[0]/1000];
	amg_buf[1] = smg_code[LED_Intervaltime[0]/100%10];
	amg_buf[2] = smg_code[LED_Intervaltime[0]/10%10];
	amg_buf[3] = smg_code[LED_Intervaltime[0]%10];
	amg_buf[4] =0x00;
	amg_buf[5] = 0x00;
	amg_buf[6] =0x00;
	amg_buf[7] = smg_code[a];
	}
	
	if(Display==1){
	if(twinkling_flag==1){                                  //��˸
		if(twinkling_time>=800){twinkling =~ twinkling;twinkling_time=0;}
		if(twinkling==0){
		amg_buf[0] = smg_code[0];
		amg_buf[1] = smg_code[LED_Mode];
		amg_buf[2] = smg_code[0];
		}
		if(twinkling==1){
		amg_buf[0] =0x00;
		amg_buf[1] =0x00;
		amg_buf[2] = 0x00;
		}
	}else{
	amg_buf[0] = smg_code[0];
	amg_buf[1] = smg_code[LED_Mode];
	amg_buf[2] = smg_code[0];
	}

	amg_buf[3] = 0x00;
	
	
		if(twinkling_flag==2){                                  //��˸
		if(twinkling_time>=800){twinkling =~ twinkling;twinkling_time=0;}
		if(twinkling==0){
		amg_buf[4] =smg_code[LED_Intervaltime[i]/1000];
		amg_buf[5] = smg_code[LED_Intervaltime[i]/100%10];
		amg_buf[6] = smg_code[LED_Intervaltime[i]/10%10];
		amg_buf[7] = smg_code[LED_Intervaltime[i]%10];
		}
		if(twinkling==1){
		amg_buf[4] = 0x00;
		amg_buf[5] =0x00;
		amg_buf[6] =0x00;
		amg_buf[7] = 0x00;
		}
	}else{
	amg_buf[4] =smg_code[LED_Intervaltime[i]/1000];
	amg_buf[5] = smg_code[LED_Intervaltime[i]/100%10];
	amg_buf[6] = smg_code[LED_Intervaltime[i]/10%10];
	amg_buf[7] = smg_code[LED_Intervaltime[i]%10];
	}
	
	}
	
	if(Display==2){
	amg_buf[0] = 0x00;
	amg_buf[1] = 0x00;
	amg_buf[2] = 0x00;
	amg_buf[3] = 0x00;
	amg_buf[4] = 0x00;
	amg_buf[5] = 0x00;
	amg_buf[6] = smg_code[0];
	amg_buf[7] = smg_code[5];
	}
	
}

u8 key_io;

void main(){
	System_Init();
	Timer2_Init();
	RRead_EEPROM();
	//Write_EEPROM(0x01,0x90>>8);
	while(1){
	
	SMG_Process();
	ADC_Read_Process();
		if(key_time>=50){
			key_time=0;
			KBD_ThreeLine();
			if(Trg_KBD==0x81){			//7      LED������ֹͣ����
				LED_flag =~ LED_flag;
			}
			if(Trg_KBD==0x82){             //6      ���ð���
				kbd_six_count++;
				if(kbd_six_count==1){Display=1;}
				if(kbd_six_count==2){twinkling_flag=1;}
				if(kbd_six_count==3){twinkling_flag=2;}
				if(kbd_six_count==4){
				twinkling_flag=0;kbd_six_count=0;Display=0;						//�˳�����ģʽʱҪ����EEPROM
				Write_EEPROM(0x00,LED_Intervaltime[0]);
				Write_EEPROM(0x01,LED_Intervaltime[0]>>8);
				Write_EEPROM(0x02,LED_Intervaltime[1]);
				Write_EEPROM(0x03,LED_Intervaltime[1]>>8);
				Write_EEPROM(0x04,LED_Intervaltime[2]);
				Write_EEPROM(0x05,LED_Intervaltime[2]>>8);
				Write_EEPROM(0x06,LED_Intervaltime[3]);
				Write_EEPROM(0x07,LED_Intervaltime[3]>>8);				
				}
			}
			if(twinkling_flag==1){
				if(Trg_KBD==0x84){	
					LED_Mode++;
					i++;                               //�����LED_Intervaltime[i] ,��֤ÿһ��Mode��Ӧһ��ʱ��
					ledshift=0;
					Device_Ctrl(0x80,0xff);
					if(LED_Mode==5) {LED_Mode=1;i=0; }          
				}
				if(Trg_KBD==0x88){
					if(LED_Mode>0){
						LED_Mode--;
						i--;
						ledshift=0;
						Device_Ctrl(0x80,0xff);
					}
				}
			}
			if(twinkling_flag==2){
				if(Trg_KBD==0x84){	
					LED_Intervaltime[i] += 100;
					if(LED_Intervaltime[i]==1500) LED_Intervaltime[i]=400;
				}
				if(Trg_KBD==0x88){
					if(LED_Intervaltime[i]>400){
						LED_Intervaltime[i] -= 100;			
					}
				}
			}
			
			if(Cont_KBD==0x88){
				if(kbd_six_count==0)Display=2;
			}
			if(Cont_KBD==0){
				if(kbd_six_count==0)Display=0;
			}
		}
		
		
		
		
	}
}


void Timer2_ISR()interrupt 12{
	SMG_Display();
	key_time++;
	adc_time++;
	LED_Interval++;
	pwm_time++;
	LED_Process();
	PWM_Process(adc_val);
	twinkling_time++;
	
	
	
	
}