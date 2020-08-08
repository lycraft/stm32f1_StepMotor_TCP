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
#include "usart.h"
#include "timer.h"

int sa=0,sb=0,sc=0,sd=0;
uint8_t modetemp = 6,mode = 4;//modetemp是指示位置 指向选中的mode，==============mode0-4
uint8_t flag = 1;


void measure1()//mode1屏幕不刷新的东西
{
		OLED_Clear();
	
		OLED_ShowString(0,2,"PWM1:");
		OLED_ShowString(41,2,"100");
		OLED_ShowString(73,2,"hz");
	
		OLED_ShowString(0,4,"PWM2:");
		OLED_ShowString(41,4,"1.1k");
		OLED_ShowString(73,4,"hz");
	 

	
}

void meau()//主界面
{
		OLED_Clear();
		OLED_DrawBMP(96,0,128,2,BMP2);	
		OLED_ShowString(0,6,"Meau");
		OLED_ShowString(40,0,"PWM");
		OLED_ShowString(40,2,"USART");
		OLED_ShowString(40,4,"others");
		OLED_ShowString(72,6,"Lycraft");
}

void voltage1()//ADC界面不刷新的
{
		OLED_Clear();
		

}
int main(void)
 {	
		vu8 key=0;
	 	u16 len;	
		u16 times=0;
	  u16 t; 
	 
		delay_init();	    	 //延时函数初始化	  
		NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级 	
		OLED_Init();			//初始化OLED  
//		KEY_Init();          //初始化与按键连接的硬件接口
	 	uart_init(115200);	 //串口初始化为115200
		EXTIX_Init();
		TIM3_PWM_Init(1000 - 1,72 - 1);	 //不分频。PWM频率=72000000/900=80Khz------new 72000000/72 = 1000000/1000 = 1khz(1ms)计数到1000就是1ms
	 
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
			
			delay_ms(10);	 
											 
			TIM_SetCompare2(TIM3,500);	//占空比为50% 500/1000 = 50%
			
			
		}	
//========================================================================电压电流测量ADC
		if(mode % 4 == 2)
		{
			if(flag == 1)//只执行一次的
			{
				voltage1();
				flag--;
			}
			if(USART_RX_STA&0x8000)
			{					   
				len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
				printf("\r\n您发送的消息为:\r\n\r\n");
				for(t=0;t<len;t++)
				{
					USART_SendData(USART1, USART_RX_BUF[t]);//向串口1发送数据
					while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
				}
				printf("\r\n\r\n");//插入换行
				USART_RX_STA=0;
			}else
			{
				times++;
				if(times%5000==0)
				{
					printf("\r\n战舰STM32开发板 串口实验\r\n");
					printf("正点原子@ALIENTEK\r\n\r\n");
				}
				if(times%200==0)printf("请输入数据,以回车键结束\n");  
				delay_ms(10);   
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

