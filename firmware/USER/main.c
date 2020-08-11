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
#include "led.h"

uint8_t PulForward = 0, DirForward = 0, PulBack = 0, DirBack = 0;


void measure1()//mode1屏幕不刷新的东西
{
		OLED_Clear();
	
		OLED_ShowString(0,2,"PWM:");
		OLED_ShowString(41,2,"----");
		OLED_ShowString(73,2,"hz");
	
		OLED_ShowString(0,4,"DIR+:");
		OLED_ShowString(49,4,"x");
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
		TIM3_PWM_Init(1000 - 1,72 - 1);	 //new 72000000/72 = 1000000/1000 = 1khz(1ms)计数到1000就是1ms
		LED_Init();
	 
		measure1();
	while(1) 
	{		
		
//========================================================================按键步进电机控制方向速度
			if(PulForward == 1)
			{
				delay_ms(10);	 
				TIM_SetCompare2(TIM3,500);	//占空比为50% 500/1000 = 50%	//PB5接驱动PUL引脚输出PWM波！
				OLED_ShowString(41,2,"1000");
				PulForward--;
			}				
			if(PulBack == 1)
			{
				delay_ms(10);
				TIM_SetCompare2(TIM3,1000);	//停止输出pwm波，该引脚变为低电平
				OLED_ShowString(41,2,"----");
//				TIM_PrescalerConfig(TIM3,5000,TIM_PSCReloadMode_Immediate);//修改psr以修改占空比
				PulBack--;
			}				
			if(DirForward == 1)
			{
				delay_ms(10);
			  GPIO_SetBits(GPIOB,GPIO_Pin_6); //拉高引脚PB6	
//				GPIOB->BRR=GPIO_Pin_6;
				OLED_ShowString(49,4,"+");
				DirForward--;
			}
			if(DirBack == 1)
			{
				delay_ms(10);
				GPIO_ResetBits(GPIOB,GPIO_Pin_6);//拉低引脚PB6
//				GPIOB->BSRR=GPIO_Pin_6;
				OLED_ShowString(49,4,"-");
				DirBack--;
			}
//========================================================================串口	
			if(USART_RX_STA&0x8000)//收到信息时
			{					   
				len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
				printf("\r\n您发送的消息为:\r\n\r\n");
				for(t=0;t<len;t++)//打印出接收的消息
				{
					USART_SendData(USART1, USART_RX_BUF[t]);//向串口1发送数据
					while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
				}
				printf("\r\n\r\n");//插入换行
				USART_RX_STA=0;
			}else
			{
				times++;
				if(times%200==0)printf("请输入数据,以回车键结束\n");  
				delay_ms(10);   
			}
	

	}	  
}

