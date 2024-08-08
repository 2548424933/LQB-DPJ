#include "System.h"
u8 temperature;
u8 Mode = 1;
u16 WorkTime = 0;
u16 Surplus_Work_Time;
bit Display;
u16 temperature_time;
void Timer2_Init(void)		//100微秒@12.000MHz
{
	AUXR &= 0xFB;			//定时器时钟12T模式
	T2L = 0x9C;				//设置定时初始值
	T2H = 0xFF;				//设置定时初始值
	AUXR |= 0x10;			//定时器2开始计时
	IE2 = 0x04;
	EA = 1;
}

//PWM占空控制               高电平占的个数叫占空比
u8 pwm_time;
u8 pwm_duty;
u8 pwm_flag = 1;           //pwm开关型号
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


//键盘读取
u8 key_value;
u16 BTN_Time;
void BTN_Process(){
	if(BTN_Time >= 10){                   //100不行？,是的，与之前不一样，之前的按键与功能是一起放在这个键盘检测里面，之前读取的key_value值是BTN_Read_State的值（随时变化），而这里的值需要等时间更新才会从新读取BTN的值。速度不快的话key_value可能以相同的值执行两次
		key_value = BTN_Read_State();       //其实我感觉这样不行，BTN读取方法都没用了
		BTN_Time=0;
	}
}

//时间倒计时
u16 countdown;

void CountDown_t(){

	if(Surplus_Work_Time==0){
		pwm_flag = 0;                                 //PWN停止信号
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
	if(temperature_time >= 100){           //100毫秒读取一次温度
		temperature = Read_Temperature();
		temperature_time =0；
	}
	SMG_Process();
	BTN_Process();    //key_value
	CountDown_t(); //时间倒计时
	if(key_value == 4){                                               //模式切换
		++Mode;
		if(Mode == 4) Mode = 1;
	}
	if(key_value == 5){
		if(WorkTime > 120) WorkTime = 0;            //限定0 1 2 分钟
		WorkTime = WorkTime + 60;
		Surplus_Work_Time = WorkTime;
	}
	if(key_value == 6){                                            //停止按键       倒计时清零触发 work_flag的PWM停止标志位
		Surplus_Work_Time =0;                              //会触发 倒计时函数 的PWM停止信号
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