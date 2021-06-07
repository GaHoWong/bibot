#include <hidef.h>      
#include <MC9S12XS128.h>
#pragma LINK_INFO DERIVATIVE "mc9s12xs128"
          
void chaopin(void);  //��Ƶ����       
void pwm_init(void);

   
  #define delay(n)   \
  { \
     unsigned int i,j; \
    for(i=0;i<n;i++) \
       for(j=0;j<500;j++); \
  } 

        
void main(void) 
{     
             
chaopin(); //��Ƶ������ʼ��    
pwm_init();//��ʼ��PWM�˿�
for(;;) 
  
  {  /*
     //MC33886���Գ���  �����ת
     PWMDTY01=600;  //PP1��IN1 GND-GND �ı������Сʵ�ֵ���
     PWMDTY23=0;     //PP3��IN3 GND-GND
     
      delay(8000);   //��ʱ
   
     //�����ת
  
     PWMDTY23=300; // PP3��IN2 GND-GND �ı������Сʵ�ֵ���
     PWMDTY01=0;   //PP1��IN1 GND-GND  
        
     delay(8000);  //��ʱ
    
 */   
    
    
    
  //BTN7971ģ����Գ��� �����ת
  
     PWMDTY45=1000;     // PP5��IN1 GND-GND �ı������Сʵ�ֵ���
     PWMDTY67=0;       // PP7��IN2 GND-GND 
                                                                  
     delay(10000);  //��ʱ                  
    
    //�����ת
  
     PWMDTY67=500;  // PP7��IN2 GND-GND �ı������Сʵ�ֵ���
     PWMDTY45=0;     //PP5��IN1 GND-GND 
     delay(10000);   // ��ʱ 
    
        
  }

}

  
  
  
 void chaopin(void)//64M
{  
    CLKSEL=0X00;   
    PLLCTL_PLLON=1;   
    SYNR =0xc0 | 0x07;                        
    REFDV=0xc0 | 0x01; 
    POSTDIV=0x00;       
    _asm(nop);          
    _asm(nop);
    _asm(nop);
    _asm(nop);
    while(!(CRGFLG_LOCK==1));   
    CLKSEL_PLLSEL =1;          		        
}

 //---------------PWM��ʼ��-------------
 void pwm_init(void)
 {
    
     PWME=0X00;      //��ֹPWM���
     PWMCTL=0XF0;    //����01 23,45,67 ͨ������
     PWMPRCLK=0X33;  //ClockA=bus/8=8MHz   ���  ClockB=bus/8=8MHz
    // PWMSCLA=0X04;   //PWMSCLA=1  ClockSA=1MHz   ��Ҫ���͵�Ƶ��                        2
     //PWMSCLB=0X04;   //PWMSCLB=1  ClockSB=1MHz   ���üĴ����ķ�Ƶ����
     PWMCLK=0X00;    //0000 1110 1=SA 3=SB 5=A 7=B
     PWMPOL=0XAA;    //PWMͨ��1.3 ��5��7��ʼȫ��Ϊ�ߵ�ƽ��� ,�Ӹߵ������
     PWMCAE=0X00;    //�����
     
     PWMPER45=1000;  //����0.125ms 10KHZ  ��1/A��B��*1000
     PWMPER67=1000;
   
     PWMPER01=2000;  //����0.250ms 4KHZ  ��1/A��B��*2000
     PWMPER23=2000;
     
     //PWMDTY23=1000;  //�������  С��   ����
     //PWMPER23=40000; //����40ms   1/1MHz*40000DTY45=0; 
     PWMDTY01=0;   //  ��ת ��IN1
     PWMDTY23=0;   //  ��ת ��IN2
     PWMDTY45=0;   //  ��ת ��IN1
     PWMDTY67=0;   //  ��ת ��IN2
     PWME_PWME1=1; //����PWM��ͨ�� 1
     PWME_PWME3=1; //����PWM��ͨ�� 3
     PWME_PWME5=1; //����PWM��ͨ�� 5
     PWME_PWME7=1; //����PWM��ͨ�� 7                   
 

 } 


//��л����ִ��Ƽ���֧�ֺ����Σ����ǽ����ϵ�Ŭ����ȡ���ø��á�

//����ףԸ���ǵĿͻ�ȡ�úõĳɼ���

 