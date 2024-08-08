#include "System.h"
u8 temperature;
u8 Mode = 1;
u16 WorkTime = 0;
u16 Surplus_Work_Time;
bit Display;
u16 temperature_time;
void Timer2_Init(void)		//100΢��@12.000MHz
{
	AUXR &= 0xFB;			//��ʱ��ʱ��12Tģʽ
	T2L = 0x9C;				//���ö�ʱ��ʼֵ
	T2H = 0xFF;				//���ö�ʱ��ʼֵ
	AUXR |= 0x10;			//��ʱ��2��ʼ��ʱ
	IE2 = 0x04;
	EA = 1;
}

//PWMռ�տ���               �ߵ�ƽռ�ĸ�����ռ�ձ�
u8 pwm_time;
u8 pwm_duty;
u8 pwm_flag = 1;           //pwm�����ͺ�
void PWM_Process(u8 Mode){
	if(pwm_flag == 1){
		if(Mode == 1) pwm_duty = 20;
		if(Mode == 2) pwm_duty = 30;
		if(Mode == 3) pwm_duty = 70;
		pwm_time = pwm_time%10 +1;
		if(pwm_time < (pwm_duty/10)){
			DeviceCtrl(0x80,~0x01);
			P34 =1;
		}else{
			DeviceCtrl(0x80,0xff);
			P34=0;
		}
	}
	if(pwm_flag == 0){
		pwm_time = 0;
		pwm_duty = 0;
	}
	
}


//���̶�ȡ
u8 key_value;
u16 BTN_Time;
void BTN_Process(){
	if(BTN_Time >= 10){                   //100���У�,�ǵģ���֮ǰ��һ����֮ǰ�İ����빦����һ�����������̼�����棬֮ǰ��ȡ��key_valueֵ��BTN_Read_State��ֵ����ʱ�仯�����������ֵ��Ҫ��ʱ����²Ż���¶�ȡBTN��ֵ���ٶȲ���Ļ�key_value��������ͬ��ִֵ������
		key_value = BTN_Read_State();       //��ʵ�Ҹо��������У�BTN��ȡ������û����
		BTN_Time=0;
	}
}

//ʱ�䵹��ʱ
u16 countdown;

void CountDown_t(){

	if(Surplus_Work_Time==0){
		pwm_flag = 0;                                 //PWNֹͣ�ź�
	}
	if(Surplus_Work_Time!=0){
		pwm_flag = 1;
		if(countdown>=10000 ){
		Surplus_Work_Time = Surplus_Work_Time -1;
		countdown=0;
		}
	}
		
			       
}

int i = 0;
void SMG_Process(){
	if(Display ==0){
			smg_buf[0] = 0x40;
			smg_buf[1] = smg_code[Mode];
			smg_buf[2] = 0x40; 
			smg_buf[3] =smg_code[0];
			smg_buf[4] = smg_code[Surplus_Work_Time/1000];
			smg_buf[5] = smg_code[Surplus_Work_Time/100%10];
			smg_buf[6] = smg_code[Surplus_Work_Time/10%10];
			smg_buf[7] = smg_code[Surplus_Work_Time%10];
	}
	if(Display == 1){
			smg_buf[0] = 0x40;
			smg_buf[1] = smg_code[Mode];
			smg_buf[2] = 0x40;
			smg_buf[3] = 0x00;
			smg_buf[4] = 0x00;
			smg_buf[5] = smg_code[temperature /10%10];
			smg_buf[6] = smg_code[temperature %10];
			smg_buf[7] = 0x39;
	}

}


void main(){
	Timer2_Init();

	while(1){
	if(temperature_time >= 100){           //100�����ȡһ���¶�
		temperature = Read_Temperature();
		temperature_time =0��
	}
	SMG_Process();
	BTN_Process();    //key_value
	CountDown_t(); //ʱ�䵹��ʱ
	if(key_value == 4){                                               //ģʽ�л�
		++Mode;
		if(Mode == 4) Mode = 1;
	}
	if(key_value == 5){
		if(WorkTime > 120) WorkTime = 0;            //�޶�0 1 2 ����
		WorkTime = WorkTime + 60;
		Surplus_Work_Time = WorkTime;
	}
	if(key_value == 6){                                            //ֹͣ����       ����ʱ���㴥�� work_flag��PWMֹͣ��־λ
		Surplus_Work_Time =0;                              //�ᴥ�� ����ʱ���� ��PWMֹͣ�ź�
		WorkTime = 0;
		//pwm_flag = 0; 
	}
	if(key_value == 7){
		Display =~ Display;
	}
	
	
	}
}


void Timer2_ISR()interrupt 12{
	SMG_Display();
	BTN_Time++;
	pwm_time++;
	PWM_Process(Mode);
	countdown++;
	temperature_time++;
}