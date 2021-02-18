#include "show.h"

unsigned char i;          //计数变量
unsigned char Send_Count; //串口需要发送的数据个数
float Vol;
/**************************************************************************
函数功能：OLED显示
入口参数：无
返回  值：无
**************************************************************************/
void oled_show(void)
{
	//=============第一行=======================//	
					                if(Velocity>60)OLED_ShowString(00,00,"HIGH SPEED");
	                        else 					 OLED_ShowString(00,00,"LOW  SPEED");
		                      OLED_ShowNumber(95,00, Velocity,2,12);
		//=============第二行=======================//	
			                    OLED_ShowString(00,20,"Enc-Walk");
		if( Encoder_Walk<0)	  OLED_ShowString(80,20,"-"),
		                      OLED_ShowNumber(95,20,-Encoder_Walk,3,12);
		else                 	OLED_ShowString(80,20,"+"),
		                      OLED_ShowNumber(95,20, Encoder_Walk,3,12);
		//=============第三行=======================//	
		                      OLED_ShowString(00,10,"Enc-Bala");
		if( Encoder_Balance<0)OLED_ShowString(80,10,"-"),
		                      OLED_ShowNumber(95,10,-Encoder_Balance,3,12);
		else                 	OLED_ShowString(80,10,"+"),
		                      OLED_ShowNumber(95,10, Encoder_Balance,3,12);
  	//=============第四行=======================//		
		                      OLED_ShowString(00,30,"SERVO");
		if(Motor_Turn<0)		  OLED_ShowString(80,30,"-"),
		                      OLED_ShowNumber(95,30,-Motor_Turn,3,12);
		else               		OLED_ShowString(80,30,"+"),
		                      OLED_ShowNumber(95,30,Motor_Turn,3,12);	
		//=============第五行显示电压=======================//
		                      OLED_ShowString(00,40,"Volta");
		                      OLED_ShowString(58,40,".");
		                      OLED_ShowString(80,40,"V");
		                      OLED_ShowNumber(45,40,Voltage/100,2,12);
		                      OLED_ShowNumber(68,40,Voltage%100,2,12);
		 if(Voltage%100<10) 	OLED_ShowNumber(62,40,0,2,12);
		//=============第六行显示角度=======================//
		                      OLED_ShowString(0,50,"Angle");
		if(Pitch<0)	        	OLED_ShowNumber(40,50,Pitch+360,3,12);
		else					        OLED_ShowNumber(40,50,Pitch,3,12);
													OLED_ShowString(70,50,"INIT");
		if(Middle<0)	      	OLED_ShowNumber(103,50,Middle+360,3,12);
		else					        OLED_ShowNumber(103,50,Middle,3,12);
				//=============刷新=======================//
		OLED_Refresh_Gram();	
	}
/**************************************************************************
函数功能：向APP发送数据
入口参数：无
返回  值：无
作    者：平衡小车之家
**************************************************************************/
void APP_Show(void)
{    
	static u8 flag;
	int app_2,app_3,app_4;
	app_4=(Voltage-1110)*2/3;		if(app_4<0)app_4=0;if(app_4>100)app_4=100;   //对电压数据进行处理
	app_3=Encoder_Balance/1.3; if(app_3<0)app_3=-app_3;			                   //对编码器数据就行数据处理便于图形化
	app_2=Encoder_Balance/1.3;  if(app_2<0)app_2=-app_2;
	flag=!flag;
	if(PID_Send==1)//发送PID参数
	{
		printf("{C%d:%d:%d:%d:%d:%d:%d:%d:%d}$",(int)Balance_KP,(int)Balance_KD,(int)Position_KP,(int)Position_KI,0,0,0,0,0);//打印到APP上面	
		PID_Send=0;	
	}	
   else	if(flag==0)// 
   printf("{A%d:%d:%d:%d}$",(u8)app_2,(u8)app_3,app_4,(int)Pitch); //打印到APP上面
	 else
	 printf("{B%d:%d:%d}$",(int)Pitch,(int)Encoder_Balance,(int)Encoder_Walk);//打印到APP上面 显示波形
}
/**************************************************************************
函数功能：虚拟示波器往上位机发送数据 关闭显示屏
入口参数：无
返回  值：无
作    者：平衡小车之家
**************************************************************************/
void DataScope(void)
{   

		DataScope_Get_Channel_Data( Pitch, 1 );          // 角度
		DataScope_Get_Channel_Data(Encoder_Balance, 2 ); // 平衡方向的编码器数据
		DataScope_Get_Channel_Data(Encoder_Walk, 3 );    //  前进方向的编码器数据
  	DataScope_Get_Channel_Data(Motor_Turn, 4 );      //转向数据
//		DataScope_Get_Channel_Data(0, 5 ); //用您要显示的数据替换0就行了
//		DataScope_Get_Channel_Data(0 , 6 );//用您要显示的数据替换0就行了
//		DataScope_Get_Channel_Data(0, 7 );
//		DataScope_Get_Channel_Data( 0, 8 ); 
//		DataScope_Get_Channel_Data(0, 9 );  
//		DataScope_Get_Channel_Data( 0 , 10);
		Send_Count = DataScope_Data_Generate(4);
		for( i = 0 ; i < Send_Count; i++) 
		{
		while((USART1->SR&0X40)==0);  
		USART1->DR = DataScope_OutPut_Buffer[i]; 
		}
}
