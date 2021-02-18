#include "sys.h"

u8 Flag_Stop=1,Flag_Show=0,Flag_Direction;         //ֹͣ��־λ�� ��ʾ��־λ Ĭ��ֹͣ ��ʾ��
float Encoder_Balance,Encoder_Walk,Target_Walk;          //���������������            
float Motor_Balance,Motor_Walk,Motor_Turn,Velocity=70;      //���PWM����
int Voltage;                              //��ص�ѹ������صı���                       
u8 delay_50,delay_flag,PID_Send;          //��ʱ��ر���
float Pitch,Roll,Yaw;   //����ǶȺ�XYZ��Ŀ���ٶ�
float	Balance_KP=1800,Balance_KD=8,Position_KP=100,Position_KI=2;  //λ�ÿ���PID����
float Adjust,Middle;
int main(void)
{ 
	Stm32_Clock_Init(9);            //=====ϵͳʱ������
	delay_init(72);                 //=====��ʱ��ʼ��
	JTAG_Set(JTAG_SWD_DISABLE);     //=====�ر�JTAG�ӿ�
	JTAG_Set(SWD_ENABLE);           //=====��SWD�ӿ� �������������SWD�ӿڵ���
	MiniBalance_PWM_Init();         //=====��ʼ��PWM 10KHZ�������������
	LED_Init();                     //=====��ʼ���� LED ���ӵ�Ӳ���ӿ�
	KEY_Init();                     //=====������ʼ��
	OLED_Init();                    //=====OLED��ʼ��
	uart_init(72,128000);           //=====��ʼ������1
	uart2_init(36,9600);            //=====����3��ʼ��
	Encoder_Init_TIM2();            //=====�������ӿ�
	Encoder_Init_TIM3();            //=====��ʼ��������C
	Adc_Init();                     //=====adc��ʼ��
	delay_ms(500);                  //=====��ʱ
	IIC_Init();                     //=====IIC��ʼ��
  MPU6050_initialize();           //=====MPU6050��ʼ��	
  DMP_Init();                     //=====��ʼ��DMP     
  EXTI_Init();                    //=====MPU6050 5ms��ʱ�жϳ�ʼ��
	while(1)
		{	
	    if(Flag_Show==0)          //ʹ��MiniBalance APP��OLED��ʾ��
			{
				APP_Show();	
 				oled_show();          //===��ʾ����
	    }
    	else                    //ʹ��MiniBalance ��λ�� ��λ��ʹ�õ�ʱ����Ҫ�ϸ��ʱ�򣬹ʴ�ʱ�ر�app��ز��ֺ�OLED��ʾ��
  		{
				DataScope();          //����MiniBalance��λ��
			}
			delay_flag=1;	
			delay_50=0;
			while(delay_flag);	     //ͨ��MPU6050��INT�ж�ʵ�ֵ�50ms��׼��ʱ				
		} 
}
