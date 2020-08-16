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
#include "tcp.h"
#include "esp8266.h"

uint8_t PulForward = 0, DirForward = 0, PulBack = 0, DirBack = 0;
u8 RevData = 0;

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
//////////////////////////////////////////////////////////////////////////////////*	 
//初始化变量 
//	 
//////////////////////////////////////////////////////////////////////////////////  
		vu8 key=0;
	 	uint8_t res;
	  char str[100]={0};
		
//////////////////////////////////////////////////////////////////////////////////	 
//初始化函数	  
//////////////////////////////////////////////////////////////////////////////////  	 
		delay_init();	    	 //延时函数初始化	  
		NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级 	
		OLED_Init();			//初始化OLED  
//		KEY_Init();          //初始化与按键连接的硬件接口
	 	uart_init(115200);	 //串口初始化为115200
	  ESP8266_Init(115200);
		EXTIX_Init();
		TIM3_PWM_Init(1000 - 1,72 - 1);	 //new 72000000/72 = 1000000/1000 = 1khz(1ms)计数到1000就是1ms
		LED_Init();

//////////////////////////////////////////////////////////////////////////////////	 
//只执行一次的函数  
////////////////////////////////////////////////////////////////////////////////// 		
		measure1();
	 
		OLED_ShowString(0,0, "configing_0");	
    ESP8266_AT_Test();
		OLED_ShowString(0,0, "configing_1");	 
		printf("正在配置ESP8266\r\n");
    ESP8266_Net_Mode_Choose(STA);
		OLED_ShowString(0,0, "configing_2");
    while(!ESP8266_JoinAP(User_ESP8266_SSID, User_ESP8266_PWD));
		OLED_ShowString(0,0, "configing_3");
    ESP8266_Enable_MultipleId ( DISABLE );
		OLED_ShowString(0,0, "configing_4");
    while(!ESP8266_Link_Server(enumTCP, User_ESP8266_TCPServer_IP, User_ESP8266_TCPServer_PORT, Single_ID_0));
		OLED_ShowString(0,0, "configing_5");
    while(!ESP8266_UnvarnishSend());
		OLED_ShowString(0,0, "configing_6");
		printf("\r\n配置完成");
		OLED_ShowString(0,0, "config is ok!");	
		
//////////////////////////////////////////////////////////////////////////////////	 
//一直循环的函数 
////////////////////////////////////////////////////////////////////////////////// 			
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
      		if(RevData == 'a')
					{
						OLED_ShowString(49,4,"a");
						sprintf (str,"a电机方向向左设置完成\r\n" );//格式化发送字符串到TCP服务器
						ESP8266_SendString ( ENABLE, str, 0, Single_ID_0 );
						DirForward = 1;
						RevData = 0;
					}
					else if(RevData == 'b')
					{
						OLED_ShowString(49,4,"b");
						sprintf (str,"b电机方向向右设置完成\r\n" );//格式化发送字符串到TCP服务器
						ESP8266_SendString ( ENABLE, str, 0, Single_ID_0 );
						DirBack = 1;
						RevData = 0;
					}
					else if(RevData == 'c')
					{
						OLED_ShowString(49,4,"c");
						sprintf (str,"c电机启动\r\n" );//格式化发送字符串到TCP服务器
						ESP8266_SendString ( ENABLE, str, 0, Single_ID_0 );
						PulForward = 1;
						RevData = 0;
					}
					else if(RevData == 'd')
					{
						OLED_ShowString(49,4,"d");
						sprintf (str,"d电机停止\r\n" );//格式化发送字符串到TCP服务器
						ESP8266_SendString ( ENABLE, str, 0, Single_ID_0 );
						PulBack = 1;
						RevData = 0;
					}
					else
					{
						if(RevData != 0)
						{
							OLED_ShowString(49,4,"?");
							sprintf (str,"不是正确指令\r\n" );//格式化发送字符串到TCP服务器
							ESP8266_SendString ( ENABLE, str, 0, Single_ID_0 );
							RevData = 0;
						}
					}
						  
				delay_ms(10);
			
        if(TcpClosedFlag) //判断是否失去连接
        {
            ESP8266_ExitUnvarnishSend(); //退出透传模式
            do
            {
                res = ESP8266_Get_LinkStatus();     //获取连接状态
            }   
            while(!res);

            if(res == 4)                     //确认失去连接，重连
            {
                
                
                while (!ESP8266_JoinAP(User_ESP8266_SSID, User_ESP8266_PWD ) );
                while (!ESP8266_Link_Server(enumTCP, User_ESP8266_TCPServer_IP, User_ESP8266_TCPServer_PORT, Single_ID_0 ) );        
            } 
            while(!ESP8266_UnvarnishSend());                    
        }
				

	}	  
}

