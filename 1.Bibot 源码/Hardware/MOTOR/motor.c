#include "motor.h"

void MiniBalance_Motor_Init(void)
{
	RCC->APB2ENR|=1<<2;        //PORTA时钟使能   
  RCC->APB2ENR|=1<<3;        //PORTB时钟使能   

	GPIOA->CRH&=0XFFF00FF0;   //
	GPIOA->CRH|=0X0002B002;   //
	
	GPIOB->CRL&=0X00FFFFFF;   //
	GPIOB->CRL|=0XBB000000;   //
	GPIOB->CRH&=0X000FFF00;   //
	GPIOB->CRH|=0X222000BB;   //
}
void MiniBalance_PWM_Init(void)
{		 					 
  MiniBalance_Motor_Init();  //初始化电机控制所需IO
	RCC->APB2ENR|=1<<11;       //使能TIM1时钟 
  RCC->APB1ENR|=1<<2;        //TIM4时钟使能	
	TIM1->ARR=9999;             //设定计数器自动重装值 
	TIM1->PSC=144;             //预分频器不分频
	TIM1->CCMR2|=6<<12;        //CH4 PWM1模式	
	TIM1->CCMR2|=1<<11;        //CH4预装载使能	   
	TIM1->CCER|=1<<12;         //CH4输出使能	   
  TIM1->BDTR |= 1<<15;       //TIM1必须要这句话才能输出PWM
	TIM1->CR1 = 0x80;           //ARPE使能 
	TIM1->CR1|=0x01;          //使能定时器1 		
  TIM1->CCR4=750;	
	
	TIM4->ARR=7199;            //设定计数器自动重装值 
	TIM4->PSC=0;               //预分频器不分频
  TIM4->CCMR2|=6<<12;        //CH4 PWM1模式	
	TIM4->CCMR2|=6<<4;         //CH3 PWM1模式	
	TIM4->CCMR1|=6<<12;        //CH2 PWM1模式	
	TIM4->CCMR1|=6<<4;         //CH1 PWM1模式	
  TIM4->CCMR2|=1<<11;        //CH4预装载使能	 
	TIM4->CCMR2|=1<<3;         //CH3预装载使能	
	TIM4->CCMR1|=1<<11;        //CH2预装载使能	 
	TIM4->CCMR1|=1<<3;         //CH1预装载使能	
	TIM4->CCER|=1<<12;          //CH4输出使能	   
	TIM4->CCER|=1<<8;          //CH3输出使能	
	TIM4->CCER|=1<<4;          //CH2输出使能	   
	TIM4->CCER|=1<<0;          //CH1输出使能	
	TIM4->CR1=0x80;            //ARPE使能 
	TIM4->CR1|=0x01;           //使能定时器3			
	STBY=0; 
} 

