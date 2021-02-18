#include "control.h"	
#include "filter.h"	


u8 Flag_Target;     //��ر�־λ
u8 temp1;                                               //��ʱ����
float Voltage_Count,Voltage_All;  //��ѹ������ر���
/**************************************************************************
�������ܣ����еĿ��ƴ��붼��������
         5ms��ʱ�ж���MPU6050��INT���Ŵ���
         �ϸ�֤���������ݴ����ʱ��ͬ��				 
**************************************************************************/
int EXTI15_10_IRQHandler(void) 
{    
	 if(INT==0)		
	{     
				EXTI->PR=1<<12;                                                      //���LINE5�ϵ��жϱ�־λ  		
			 Flag_Target=!Flag_Target;
		   if(delay_flag==1)
			 {
				 if(++delay_50==10)	 delay_50=0,delay_flag=0;                     //���������ṩ50ms�ľ�׼��ʱ
			 }
		  if(Flag_Target==1)                                                  //5ms��ȡһ�������Ǻͼ��ٶȼƵ�ֵ
			{ 
					Read_DMP();                                                           //===������̬		
				  Voltage_All+=Get_battery_volt();                                    //��β����ۻ�
			    if(++Voltage_Count==100) Voltage=Voltage_All/100,Voltage_All=0,Voltage_Count=0;//��ƽ��ֵ ��ȡ��ص�ѹ	 
			  	Key();//ɨ�谴���仯	
			return 0;	                                               
			}
		  Encoder_Balance=Read_Encoder(3);			//===10ms����һ�Σ�BalanceΪ�˱�֤M�����ٵ�ʱ���׼�����ȶ�ȡ����������
		  Encoder_Walk=Read_Encoder(2);     
	  	Read_DMP();                                       //===������̬	
  		Led_Flash(100);                                   //===LED��˸;����ģʽ 1s�ı�һ��ָʾ�Ƶ�״̬		
      Adjust=Get_Adc(Adjust_Ch);	
      Middle=(Adjust-POTENTIOMETER)/392+3-Encoder_Balance/35;		//+Motor_Turn/50
		 	Get_RC(Flag_Direction);	

		  Motor_Walk=Walk_Control(Encoder_Walk,Target_Walk),           //ֱ��
			Motor_Balance=Balance_Control(Pitch,gyro[1])+Position_Control(Encoder_Balance);   //����  //

		  Xianfu_Pwm(7000,110);                                 //===PWM�޷�
			if(Turn_Off(Pitch,Voltage)==0)     
		  Set_Pwm(-Motor_Balance,Motor_Walk,Motor_Turn);                   //===��ֵ��PWM�Ĵ���  		 
			
 }
	 return 0;	 
} 

/**************************************************************************
�������ܣ����PD����
��ڲ������Ƕ�
����  ֵ����ǿ���PWM
��    �ߣ�ƽ��С��֮��
**************************************************************************/
float Balance_Control(float Angle,float Gyro)
{  
   float Bias;
	 int balance;
	 Bias=Angle-Middle;  //===���ƽ��ĽǶ���ֵ �ͻ�е�����ķֲ����
	 balance=Balance_KP*Bias+Gyro*Balance_KD;   //===����ƽ����Ƶĵ��PWM  PD����   kp��Pϵ�� kd��Dϵ�� 
	 return balance;
}
/**************************************************************************
�������ܣ�λ��PD���� 
��ڲ�����������
����  ֵ��λ�ÿ���PWM
��    �ߣ�ƽ��С��֮��
**************************************************************************/
float Position_Control(int encoder)
{  
    static float Velocity,Encoder_Least,Movement,Encoder;
	  static float Encoder_Integral;  
    //=============�ٶ�PI������=======================//	
		Encoder_Least=encoder;       //�ٶ��˲�  
		Encoder *= 0.7;		                                                //===һ�׵�ͨ�˲���       
		Encoder += Encoder_Least*0.3;	                                    //===һ�׵�ͨ�˲���    
 		Encoder_Integral +=Encoder;                                       //===���ֳ�λ�� 
		Encoder_Integral +=Movement;                                      //===����ң�������ݣ�����ǰ������
		if(Encoder_Integral>1500)  	Encoder_Integral=1500;               //===�����޷�
		if(Encoder_Integral<-1500)	Encoder_Integral=-1500;              //===�����޷�	
	  if(Flag_Stop)   Encoder_Integral=0; //===����رպ��������
		Velocity=Encoder*Position_KP+Encoder_Integral*Position_KI;        //===�ٶȿ���	
	  if(Flag_Stop)   Velocity=0;      //===����رպ��������
	  return Velocity;
}

/**************************************************************************
�������ܣ�ǰ�������ٶȿ���
��ڲ�����Ŀ��ֵ ����ֵ
����  ֵ������PWM
��    �ߣ�ƽ��С��֮��
**************************************************************************/
int Walk_Control (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //����ƫ��
	 Pwm+=15*(Bias-Last_bias)+15*Bias;   //����ʽPI������
	 if(Pwm>7200)Pwm=7200;
	 if(Pwm<-7200)Pwm=-7200;
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}


/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ�����PWM
����  ֵ����
**************************************************************************/
void Set_Pwm(int motor_a,int motor_b,int servo)
{  
	     float Turn;
	    ///////////����ϵͳ///////
    	if(motor_a>0)			AIN1=7200,AIN2=7200-motor_a;  
			else 	            AIN2=7200,AIN1=7200+motor_a;
		  ///////////ǰ�����˿���/////
		  if(motor_b>0)			BIN1=1,BIN2=0;
			else 	            BIN2=1,BIN1=0;
	    PWM=myabs(motor_b);
	    /////////  �������   ////////
	    Turn=gyro[2]/50;
	    SERVO=SERVO_MID+servo-Turn;
}

/**************************************************************************
�������ܣ�����PWM��ֵ 
��ڲ�������ֵ
����  ֵ����
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
�������ܣ������޸�С������״̬ 
��ڲ�������
����  ֵ����
**************************************************************************/
void Key(void)
{	
	u8 tmp,tmp2;
	tmp=click_N_Double(50); 
	if(tmp==1)Flag_Stop=!Flag_Stop;//��������С������ͣ
	if(tmp==2)Flag_Show=!Flag_Show;//˫������С������ʾ״̬                 
}

/**************************************************************************
�������ܣ��쳣�رյ��
��ڲ�������ѹ
����  ֵ��1���쳣  0������
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
�������ܣ�����ֵ����
��ڲ�����long int
����  ֵ��unsigned int
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
		float step_servo=0.8;  //����ת��ȿ��Ʋ���ֵ��
		float step_motor=0.2;  //�����ٶȿ��Ʋ���ֵ��
	  float Max_Velocity;
		switch(mode)   //�������
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
		if(Target_Walk<-Max_Velocity) Target_Walk=-Max_Velocity;	   //�ٶȿ����޷�
		if(Target_Walk>Max_Velocity/2)  Target_Walk=Max_Velocity/2;	     
		if(Motor_Turn<-110) Motor_Turn=-110;	   //ת������޸�
		if(Motor_Turn>110)  Motor_Turn=110;	 	
}


