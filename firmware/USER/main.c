//////////////////////////////////////////////////////////////////////////////////	 
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  功能描述   : OLED 4接口演示例程(51系列)
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              D0   接PA8（SCL）
//              D1   接PB15（SDA）
//              RES  接PB14
//              DC   接PB13
//              CS   接PA3               
//              ----------------------------------------------------------------
//******************************************************************************/
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "key.h"
#include "exti.h" 

int sa=0,sb=0,sc=0,sd=0;
int wendu,shidu;
uint8_t modetemp = 6,mode = 4;//modetemp是指示位置 指向选中的mode，==============mode0-4
uint8_t flag = 1;


void measure1()//mode1屏幕不刷新的东西
{
		OLED_Clear();
	
		OLED_ShowString(0,2,"PWM1:");
	
		OLED_ShowString(0,4,"PWM2:");

	 
	 	OLED_ShowCHinese(0,6,0);
		OLED_ShowCHinese(18,6,1);
		OLED_ShowCHinese(36,6,2);//的
		OLED_ShowCHinese(54,6,3);//迷
		OLED_ShowCHinese(72,6,4);//你
		OLED_ShowCHinese(90,6,5);//屏
		OLED_ShowCHinese(108,6,6);//幕
	
}

void meau()//主界面
{
		OLED_Clear();
		OLED_DrawBMP(96,0,128,2,BMP2);	
		OLED_ShowString(0,6,"Meau");
		OLED_ShowString(40,0,"PWM");
		OLED_ShowString(40,2,"ADC");
		OLED_ShowString(40,4,"others");
		OLED_ShowString(72,6,"Lycraft");
}

void voltage1()//ADC界面不刷新的
{
		OLED_Clear();
		OLED_ShowCHinese(0,2,10);//电
		OLED_ShowCHinese(18,2,11);//压
		OLED_ShowCHinese(0,4,10);//电
		OLED_ShowCHinese(18,4,12);//流
		
		OLED_ShowChar(37,2,':');
		OLED_ShowChar(37,4,':');
		OLED_ShowChar(71,2,'.');
		OLED_ShowChar(71,4,'.');
		OLED_ShowChar(112,2,'V');
		OLED_ShowChar(112,4,'A');	
}
int main(void)
 {	
		vu8 key=0;
	 
		delay_init();	    	 //延时函数初始化	  
		NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级 	
		OLED_Init();			//初始化OLED  
//		KEY_Init();          //初始化与按键连接的硬件接口
		EXTIX_Init();
		
	 
	while(1) 
	{		
		if(modetemp == 3||modetemp == 9)//======箭头标志位
		{
			modetemp = 6;
		}
//========================================================================Meau
		if(mode % 4 == 0)
		{
			if(flag == 1)//只执行一次的
			{
				meau();	
				flag--;				
			}
			OLED_ShowChar(32,(modetemp % 3) * 2,'>');
			OLED_ShowChar(32,(modetemp - 1) % 3 * 2,' ');
			OLED_ShowChar(32,(modetemp + 1) % 3 * 2,' ');
		}
//========================================================================PWM
		if(mode % 4 == 1)
		{
			if(flag == 1)//只执行一次的
			{
				measure1();
				flag--;
			}
			
			
			
		}	
//========================================================================电压电流测量ADC
		if(mode % 4 == 2)
		{
			if(flag == 1)//只执行一次的
			{
				voltage1();
				flag--;
			}

			
			
		}	
//=========================================================================others
			if(mode % 4 == 3)
		{
			if(flag == 1)//只执行一次的
			{
				OLED_Clear();
				flag--;				
			}
			OLED_DrawBMP(0,0,128,8,BMP1);
			delay_ms(100);
		
		}
	}	  
}

