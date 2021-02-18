#include "motor.h"

void MiniBalance_Motor_Init(void)
{
	RCC->APB2ENR|=1<<2;        //PORTAʱ��ʹ��   
  RCC->APB2ENR|=1<<3;        //PORTBʱ��ʹ��   

	GPIOA->CRH&=0XFFF00FF0;   //
	GPIOA->CRH|=0X0002B002;   //
	
	GPIOB->CRL&=0X00FFFFFF;   //
	GPIOB->CRL|=0XBB000000;   //
	GPIOB->CRH&=0X000FFF00;   //
	GPIOB->CRH|=0X222000BB;   //
}
void MiniBalance_PWM_Init(void)
{		 					 
  MiniBalance_Motor_Init();  //��ʼ�������������IO
	RCC->APB2ENR|=1<<11;       //ʹ��TIM1ʱ�� 
  RCC->APB1ENR|=1<<2;        //TIM4ʱ��ʹ��	
	TIM1->ARR=9999;             //�趨�������Զ���װֵ 
	TIM1->PSC=144;             //Ԥ��Ƶ������Ƶ
	TIM1->CCMR2|=6<<12;        //CH4 PWM1ģʽ	
	TIM1->CCMR2|=1<<11;        //CH4Ԥװ��ʹ��	   
	TIM1->CCER|=1<<12;         //CH4���ʹ��	   
  TIM1->BDTR |= 1<<15;       //TIM1����Ҫ��仰�������PWM
	TIM1->CR1 = 0x80;           //ARPEʹ�� 
	TIM1->CR1|=0x01;          //ʹ�ܶ�ʱ��1 		
  TIM1->CCR4=750;	
	
	TIM4->ARR=7199;            //�趨�������Զ���װֵ 
	TIM4->PSC=0;               //Ԥ��Ƶ������Ƶ
  TIM4->CCMR2|=6<<12;        //CH4 PWM1ģʽ	
	TIM4->CCMR2|=6<<4;         //CH3 PWM1ģʽ	
	TIM4->CCMR1|=6<<12;        //CH2 PWM1ģʽ	
	TIM4->CCMR1|=6<<4;         //CH1 PWM1ģʽ	
  TIM4->CCMR2|=1<<11;        //CH4Ԥװ��ʹ��	 
	TIM4->CCMR2|=1<<3;         //CH3Ԥװ��ʹ��	
	TIM4->CCMR1|=1<<11;        //CH2Ԥװ��ʹ��	 
	TIM4->CCMR1|=1<<3;         //CH1Ԥװ��ʹ��	
	TIM4->CCER|=1<<12;          //CH4���ʹ��	   
	TIM4->CCER|=1<<8;          //CH3���ʹ��	
	TIM4->CCER|=1<<4;          //CH2���ʹ��	   
	TIM4->CCER|=1<<0;          //CH1���ʹ��	
	TIM4->CR1=0x80;            //ARPEʹ�� 
	TIM4->CR1|=0x01;           //ʹ�ܶ�ʱ��3			
	STBY=0; 
} 

