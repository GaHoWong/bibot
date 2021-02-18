#ifndef __MOTOR_H
#define __MOTOR_H
#include <sys.h>	 
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
#define AIN1   TIM4->CCR3  
#define AIN2   TIM4->CCR4 

#define STBY  PAout(12)
#define PWM   TIM4->CCR1 
#define SERVO TIM1->CCR4
#define BIN2   PBout(13)
#define BIN1   PBout(14)
void MiniBalance_PWM_Init(void);
void MiniBalance_Motor_Init(void);
#endif
