#include <hidef.h>      
#include <MC9S12XS128.h>
#pragma LINK_INFO DERIVATIVE "mc9s12xs128"
          
void chaopin(void);  //超频程序       
void pwm_init(void);

   
  #define delay(n)   \
  { \
     unsigned int i,j; \
    for(i=0;i<n;i++) \
       for(j=0;j<500;j++); \
  } 

        
void main(void) 
{     
             
chaopin(); //超频函数初始化    
pwm_init();//初始化PWM端口
for(;;) 
  
  {  /*
     //MC33886测试程序  电机正转
     PWMDTY01=600;  //PP1接IN1 GND-GND 改变参数大小实现调速
     PWMDTY23=0;     //PP3接IN3 GND-GND
     
      delay(8000);   //延时
   
     //电机反转
  
     PWMDTY23=300; // PP3接IN2 GND-GND 改变参数大小实现调速
     PWMDTY01=0;   //PP1接IN1 GND-GND  
        
     delay(8000);  //延时
    
 */   
    
    
    
  //BTN7971模块测试程序 电机正转
  
     PWMDTY45=1000;     // PP5接IN1 GND-GND 改变参数大小实现调速
     PWMDTY67=0;       // PP7接IN2 GND-GND 
                                                                  
     delay(10000);  //延时                  
    
    //电机反转
  
     PWMDTY67=500;  // PP7接IN2 GND-GND 改变参数大小实现调速
     PWMDTY45=0;     //PP5接IN1 GND-GND 
     delay(10000);   // 延时 
    
        
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

 //---------------PWM初始化-------------
 void pwm_init(void)
 {
    
     PWME=0X00;      //禁止PWM输出
     PWMCTL=0XF0;    //设置01 23,45,67 通道连级
     PWMPRCLK=0X33;  //ClockA=bus/8=8MHz   电机  ClockB=bus/8=8MHz
    // PWMSCLA=0X04;   //PWMSCLA=1  ClockSA=1MHz   需要更低的频率                        2
     //PWMSCLB=0X04;   //PWMSCLB=1  ClockSB=1MHz   设置寄存器的分频参数
     PWMCLK=0X00;    //0000 1110 1=SA 3=SB 5=A 7=B
     PWMPOL=0XAA;    //PWM通道1.3 、5、7起始全设为高电平输出 ,从高到低输出
     PWMCAE=0X00;    //左对齐
     
     PWMPER45=1000;  //周期0.125ms 10KHZ  （1/A或B）*1000
     PWMPER67=1000;
   
     PWMPER01=2000;  //周期0.250ms 4KHZ  （1/A或B）*2000
     PWMPER23=2000;
     
     //PWMDTY23=1000;  //舵机回正  小左   大右
     //PWMPER23=40000; //周期40ms   1/1MHz*40000DTY45=0; 
     PWMDTY01=0;   //  正转 接IN1
     PWMDTY23=0;   //  反转 接IN2
     PWMDTY45=0;   //  反转 接IN1
     PWMDTY67=0;   //  正转 接IN2
     PWME_PWME1=1; //启动PWM输通道 1
     PWME_PWME3=1; //启动PWM输通道 3
     PWME_PWME5=1; //启动PWM输通道 5
     PWME_PWME7=1; //启动PWM输通道 7                   
 

 } 


//感谢你对手创科技的支持和信任，我们将不断的努力争取做得更好。

//衷心祝愿我们的客户取得好的成绩！

 