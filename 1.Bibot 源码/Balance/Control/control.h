#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
#define PI 3.14159265
#define POTENTIOMETER  2618
#define DIFFERENCE 100
#define SERVO_MID 690
extern	int Balance_Pwm,Velocity_Pwm,Turn_Pwm;
int EXTI15_10_IRQHandler(void);
void Set_Pwm(int motor_a,int motor_b,int servo);
void Kinematic_Analysis(float Vx,float Vy,float Vz);
void Kinematic_Analysis2(float Vx,float Vy,float Vz);
void Key(void);
void Xianfu_Pwm(int amplitude_motor,int amplitude_turn);
void Xianfu_Velocity(int amplitude_A,int amplitude_B,int amplitude_C);
u8 Turn_Off(float angle, int voltage);
u16 myabs(long int a);
float Balance_Control(float Angle,float Gyro);
float Position_Control(int encoder);
int balance(float angle,float gyro);
int velocity(int encoder);
int Walk_Control (int Encoder,int Target);
void Get_RC(u8 mode);
#endif
