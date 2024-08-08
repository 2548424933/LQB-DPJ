#include "System.h"
bit Display=1;
u8 key_time; u8 key_io;
u16 adc_val;u16 adc_time;  //--------------????Ϊʲô��u16������{����30��ʱ��ת��λ��û��}����������������ADC_Read_Process()
u8 dac_val=2; bit dac_flag; s16 dac_time;
s16 ne555_time; u32 ne555_val;
bit SMG_flag=0;

sbit LED4 = P0^3;

void Timer2_Init(void)		//1����@12.000MHz
{
	AUXR |= 0x04;			//��ʱ��ʱ��1Tģʽ
	T2L = 0x20;				//���ö�ʱ��ʼֵ
	T2H = 0xD1;				//���ö�ʱ��ʼֵ
	AUXR |= 0x10;			//��ʱ��2��ʼ��ʱ
	IE2 |= 0x04;
	EA = 1;
}

void Counter0_Init(){                 //��ʱ��0������
	TMOD |= 0x05;
	TH0 = 0x00;
	 TL1 = 0x00;
	 TR0 = 1;
}


//ADC��ȡ
void ADC_Read_Process(){
	if(adc_time>=10){	
		adc_val=Read_ADC()*100/51;   //�������һ������������һ����������������������֣���С�����ֽ�����������Ҫ��*100
	}
}


//DAC���
void DAC_Write_Process(){
	if(dac_time>=100){
		if(dac_flag==1){
			dac_val = adc_val;
			//Device_Ctrl(0x80,~0x01);
		}
		else{
			dac_val=2;
			//Device_Ctrl(0x80,0xff);
		}
		Write_DAC(dac_val*51);
	}
}

//NE555
void NE555_Process(){
	if(ne555_time>=1000){
		ne555_time=0;
		ne555_val = (TH0<<8)|TL0;
		TL0=0;
		TH0=0;
	}

}




u8 led_ctrl=0xff;
//LED����
void LED_Process(){
	//if((150<=dac_val&&dac_val<250)  ||  dac_val>=350){P2=0x80;P03=0;P2=0x00;}         //����Ĵ����У��� P2 ����Ϊ 0x80 Ȼ��Ѹ������Ϊ 0x00 ���ܻᵼ���������ӵ� P2 �ĵƶ��ݵر����������Ϊ�� P2 ������Ϊ 0x80 ʱ��P2 �ĵ� 7 λ���� P2.7�����øߵ�ƽ�����ܻ�˲�伤���� P2 ���ӵ����еơ�Ȼ��Ѹ�ٽ� P2 ����Ϊ 0x00��������Щ�ƶ�ʱ���ڴ��ڵ���״̬��������ʱ�伫�̣��������Ȳ��ߡ�
	//if(dac_val<150  ||  (250<=dac_val && dac_val<350)){P2=0x80;P03=1;P2=0x00;} 
	
	if((150<=adc_val&&adc_val<250)  ||  adc_val>=350){led_ctrl&=~0x04;}  
	else if(adc_val<150  ||  (250<=adc_val && adc_val<350)){led_ctrl|=0x04;} 
	
	if((1000<=ne555_val&&dac_val<5000)   ||   10000<=ne555_val){led_ctrl&=~0x08;}
	else if(ne555_val<1000   ||   (5000<=ne555_val&&dac_val<10000)){led_ctrl|=0x08;}


Device_Ctrl(0x80,led_ctrl);
}





//����ܲ�������
void SMG_Process(){
	if(SMG_flag==0){
		if(Display == 0){
		amg_buf[0] =0x71;
		amg_buf[1] =0x00;
		amg_buf[2] = 0x00;
		amg_buf[3] = smg_code[ne555_val/10000];
		amg_buf[4] = smg_code[ne555_val/1000%10];
		amg_buf[5] = smg_code[ne555_val/100%10];
		amg_buf[6] = smg_code[ne555_val/10%10];
		amg_buf[7] = smg_code[ne555_val%10];
		}		
		if(Display == 1){
		amg_buf[0] =0x3e;
		amg_buf[1] =0x00;
		amg_buf[2] = 0x00;
		amg_buf[3] = smg_code[dac_flag];
		amg_buf[4] = 0x00;
		amg_buf[5] = smg_code[adc_val/100] | 0x80;
		amg_buf[6] = smg_code[adc_val/10%10];
		amg_buf[7] = smg_code[adc_val%10];
		}
	}
	else{
		amg_buf[0] = 0x00;
		amg_buf[1] =0x00;
		amg_buf[2] = 0x00;
		amg_buf[3] = 0x00;
		amg_buf[4] = 0x00;
		amg_buf[5] = 0x00;
		amg_buf[6] = 0x00;
		amg_buf[7] = 0x00;
	}

}


void main(){
	System_Init();
	Timer2_Init();
	Counter0_Init();
	
	while(1){
		SMG_Process();
		ADC_Read_Process();  
		DAC_Write_Process();
		NE555_Process();
		
		if(key_time>=5){
			key_time = 0;
			KBD_ThreeLine();
			if(Trg==0x81) {SMG_flag=~SMG_flag;}      //s7
			if(Trg==0x82) {}                                              //s6
			if(Trg==0x84) {dac_flag=~dac_flag;}         //s5
			if(Trg==0x88) {Display=~Display;}            //s4
		}
		
		
		
		
		
		
		
		
		
	}

}



void Timer2_ISR() interrupt 12{
	SMG_Display();
	key_time++;
	ne555_time++;
	adc_time++;
	dac_time++;
	LED_Process();
}