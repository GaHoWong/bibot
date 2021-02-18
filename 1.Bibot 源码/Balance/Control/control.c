#include "control.h"	
#include "filter.h"	


u8 Flag_Target;     //相关标志位
u8 temp1;                                               //临时变量
float Voltage_Count,Voltage_All;  //电压采样相关变量
/**************************************************************************
函数功能：所有的控制代码都在这里面
         5ms定时中断由MPU6050的INT引脚触发
         严格保证采样和数据处理的时间同步				 
**************************************************************************/
int EXTI15_10_IRQHandler(void) 
{    
	 if(INT==0)		
	{     
				EXTI->PR=1<<12;                                                      //清除LINE5上的中断标志位  		
			 Flag_Target=!Flag_Target;
		   if(delay_flag==1)
			 {
				 if(++delay_50==10)	 delay_50=0,delay_flag=0;                     //给主函数提供50ms的精准延时
			 }
		  if(Flag_Target==1)                                                  //5ms读取一次陀螺仪和加速度计的值
			{ 
					Read_DMP();                                                           //===更新姿态		
				  Voltage_All+=Get_battery_volt();                                    //多次采样累积
			    if(++Voltage_Count==100) Voltage=Voltage_All/100,Voltage_All=0,Voltage_Count=0;//求平均值 获取电池电压	 
			  	Key();//扫描按键变化	
			return 0;	                                               
			}
		  Encoder_Balance=Read_Encoder(3);			//===10ms控制一次，Balance为了保证M法测速的时间基准，首先读取编码器数据
		  Encoder_Walk=Read_Encoder(2);     
	  	Read_DMP();                                       //===更新姿态	
  		Led_Flash(100);                                   //===LED闪烁;常规模式 1s改变一次指示灯的状态		
      Adjust=Get_Adc(Adjust_Ch);	
      Middle=(Adjust-POTENTIOMETER)/392+3-Encoder_Balance/35;		//+Motor_Turn/50
		 	Get_RC(Flag_Direction);	

		  Motor_Walk=Walk_Control(Encoder_Walk,Target_Walk),           //直立
			Motor_Balance=Balance_Control(Pitch,gyro[1])+Position_Control(Encoder_Balance);   //动量  //

		  Xianfu_Pwm(7000,110);                                 //===PWM限幅
			if(Turn_Off(Pitch,Voltage)==0)     
		  Set_Pwm(-Motor_Balance,Motor_Walk,Motor_Turn);                   //===赋值给PWM寄存器  		 
			
 }
	 return 0;	 
} 

/**************************************************************************
函数功能：倾角PD控制
入口参数：角度
返回  值：倾角控制PWM
作    者：平衡小车之家
**************************************************************************/
float Balance_Control(float Angle,float Gyro)
{  
   float Bias;
	 int balance;
	 Bias=Angle-Middle;  //===求出平衡的角度中值 和机械等重心分布相关
	 balance=Balance_KP*Bias+Gyro*Balance_KD;   //===计算平衡控制的电机PWM  PD控制   kp是P系数 kd是D系数 
	 return balance;
}
/**************************************************************************
函数功能：位置PD控制 
入口参数：编码器
返回  值：位置控制PWM
作    者：平衡小车之家
**************************************************************************/
float Position_Control(int encoder)
{  
    static float Velocity,Encoder_Least,Movement,Encoder;
	  static float Encoder_Integral;  
    //=============速度PI控制器=======================//	
		Encoder_Least=encoder;       //速度滤波  
		Encoder *= 0.7;		                                                //===一阶低通滤波器       
		Encoder += Encoder_Least*0.3;	                                    //===一阶低通滤波器    
 		Encoder_Integral +=Encoder;                                       //===积分出位移 
		Encoder_Integral +=Movement;                                      //===接收遥控器数据，控制前进后退
		if(Encoder_Integral>1500)  	Encoder_Integral=1500;               //===积分限幅
		if(Encoder_Integral<-1500)	Encoder_Integral=-1500;              //===积分限幅	
	  if(Flag_Stop)   Encoder_Integral=0; //===电机关闭后清除积分
		Velocity=Encoder*Position_KP+Encoder_Integral*Position_KI;        //===速度控制	
	  if(Flag_Stop)   Velocity=0;      //===电机关闭后清除积分
	  return Velocity;
}

/**************************************************************************
函数功能：前进后退速度控制
入口参数：目标值 测量值
返回  值：控制PWM
作    者：平衡小车之家
**************************************************************************/
int Walk_Control (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //计算偏差
	 Pwm+=15*(Bias-Last_bias)+15*Bias;   //增量式PI控制器
	 if(Pwm>7200)Pwm=7200;
	 if(Pwm<-7200)Pwm=-7200;
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}


/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：PWM
返回  值：无
**************************************************************************/
void Set_Pwm(int motor_a,int motor_b,int servo)
{  
	     float Turn;
	    ///////////飞轮系统///////
    	if(motor_a>0)			AIN1=7200,AIN2=7200-motor_a;  
			else 	            AIN2=7200,AIN1=7200+motor_a;
		  ///////////前进后退控制/////
		  if(motor_b>0)			BIN1=1,BIN2=0;
			else 	            BIN2=1,BIN1=0;
	    PWM=myabs(motor_b);
	    /////////  舵机控制   ////////
	    Turn=gyro[2]/50;
	    SERVO=SERVO_MID+servo-Turn;
}

/**************************************************************************
函数功能：限制PWM赋值 
入口参数：幅值
返回  值：无
************************************************************************/
void Xianfu_Pwm(int amplitude_motor,int amplitude_turn)
{	
    if(Motor_Balance<-amplitude_motor) Motor_Balance=-amplitude_motor;	
		if(Motor_Balance>amplitude_motor)  Motor_Balance=amplitude_motor;	
	  if(Motor_Walk<-amplitude_motor) Motor_Walk=-amplitude_motor;	
		if(Motor_Walk>amplitude_motor)  Motor_Walk=amplitude_motor;		
	  if(Motor_Turn<-amplitude_turn)  Motor_Turn=-amplitude_turn;	
		if(Motor_Turn>amplitude_turn)  Motor_Turn=amplitude_turn;		

}
/**************************************************************************
函数功能：按键修改小车运行状态 
入口参数：无
返回  值：无
**************************************************************************/
void Key(void)
{	
	u8 tmp,tmp2;
	tmp=click_N_Double(50); 
	if(tmp==1)Flag_Stop=!Flag_Stop;//单击控制小车的启停
	if(tmp==2)Flag_Show=!Flag_Show;//双击控制小车的显示状态                 
}

/**************************************************************************
函数功能：异常关闭电机
入口参数：电压
返回  值：1：异常  0：正常
**************************************************************************/
u8 Turn_Off(float angle, int voltage)
{
	    u8 temp;
      if(Flag_Stop==1||Pitch<(Middle-30)||Pitch>(Middle+30)||voltage<1100||Roll>15||Roll<-15)//
			{	 
				Flag_Stop=1;				
				temp=1;                                            
				STBY=0; 
				PWM=0;
				BIN1=0;
				BIN2=0;
		    SERVO=SERVO_MID;
      }
			else
			{
				STBY=1; 
				temp=0;			
			}
      return temp;			
}

/**************************************************************************
函数功能：绝对值函数
入口参数：long int
返回  值：unsigned int
**************************************************************************/
u16 myabs(long int a)
{ 		   
	  long int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}

void Get_RC(u8 mode)
{
		float step_servo=0.8;  //设置转向度控制步进值。
		float step_motor=0.2;  //设置速度控制步进值。
	  float Max_Velocity;
		switch(mode)   //方向控制
		{
		case 1:  Target_Walk-=step_motor;       Motor_Turn=Motor_Turn/1.01;   break;
		case 2:  Target_Walk-=step_motor;       Motor_Turn-=step_servo;   break;
		case 3:  Target_Walk=Target_Walk/1.03;  Motor_Turn-=step_servo;      break;
		case 4:  Target_Walk+=step_motor;       Motor_Turn-=step_servo;              break;
		case 5:  Target_Walk+=step_motor;       Motor_Turn=Motor_Turn/1.01;              break;
		case 6:  Target_Walk+=step_motor;       Motor_Turn+=step_servo;              break;
		case 7:  Target_Walk=Target_Walk/1.03;  Motor_Turn+=step_servo;                  break;
		case 8:  Target_Walk-=step_motor;       Motor_Turn+=step_servo;              break; 
		default: Target_Walk=Target_Walk/1.02;	Motor_Turn=Motor_Turn/1.01;	  break;	 
		}
		Max_Velocity=Velocity-myabs(Motor_Turn);
		if(Max_Velocity<14)Max_Velocity=14;
		if(Target_Walk<-Max_Velocity) Target_Walk=-Max_Velocity;	   //速度控制限幅
		if(Target_Walk>Max_Velocity/2)  Target_Walk=Max_Velocity/2;	     
		if(Motor_Turn<-110) Motor_Turn=-110;	   //转向控制限赋
		if(Motor_Turn>110)  Motor_Turn=110;	 	
}


