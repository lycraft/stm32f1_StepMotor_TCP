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

uint8_t PulForward = 0, DirForward = 0, PulBack = 0, DirBack = 0, AutoMove = 0, AT_Move = 0;
uint8_t DirFlag = 1;//方向标志位
uint8_t AutoMode = 0;//自动模式标志位 0为手动模式，1为自动模式
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
		u16 ClkTime = 0;
		
//////////////////////////////////////////////////////////////////////////////////	 
//初始化函数	  
//////////////////////////////////////////////////////////////////////////////////  	 
		delay_init();	    	 //延时函数初始化	  
		NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级 	
		OLED_Init();			//初始化OLED  
//		KEY_Init();          //初始化与按键连接的硬件接口
	 	uart_init(115200);	 //串口初始化为115200
	  ESP8266_Init(115200);
		EXTIX_Init();//按键
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
		
//========================================================================这里的代码均是符合条件只执行一次
			if(PulForward == 1)//电机设置启动
			{
				delay_ms(10);	 
				TIM_SetCompare2(TIM3,500);	//占空比为50% 500/1000 = 50%	//PB5接驱动PUL引脚输出PWM波！
				OLED_ShowString(41,2,"1000");
				PulForward--;
			}				
			if(PulBack == 1)//电机设置停止
			{
				delay_ms(10);
				TIM_SetCompare2(TIM3,1000);	//停止输出pwm波，该引脚变为低电平
				OLED_ShowString(41,2,"----");
//				TIM_PrescalerConfig(TIM3,5000,TIM_PSCReloadMode_Immediate);//修改psr以修改占空比
				PulBack--;
			}				
			if(DirForward == 1)//电机设置正转
			{
				delay_ms(10);
			  GPIO_SetBits(GPIOB,GPIO_Pin_6); //拉高引脚PB6	
//				GPIOB->BRR=GPIO_Pin_6;
				OLED_ShowString(49,4,"+");
				DirFlag = 1;
				DirForward--;
			}
			if(DirBack == 1)//电机设置反转
			{
				delay_ms(10);
				GPIO_ResetBits(GPIOB,GPIO_Pin_6);//拉低引脚PB6
//				GPIOB->BSRR=GPIO_Pin_6;
				OLED_ShowString(49,4,"-");
				DirFlag = 0;
				DirBack--;
			}
			if(AutoMove == 1)//电机设定自带巡航
			{
				delay_ms(10);
				AutoMode = 1;//进入自动巡航模式
				OLED_ShowString(49,4,"auto");
				AutoMove--;//使得此程序只执行一次
			}
			if(AT_Move == 1)//电机设定手动巡航
			{
				delay_ms(10);
				AutoMode = 0;//停止自动巡航模式
				OLED_ShowString(49,4,"auto");
				AT_Move--;//使得此程序只执行一次
			}
//========================================================================串口接收指令	 
			if(RevData == 'a' && AutoMode == 0)//电机方向正转而且当前为手动模式
			{
				OLED_ShowString(49,4,"a");
				sprintf (str,"a_Motor_DIR_Forward\r\n" );//格式化发送字符串到TCP服务器
				ESP8266_SendString ( ENABLE, str, 0, Single_ID_0 );
				DirForward = 1;
				RevData = 0;
			}
			else if(RevData == 'b' && AutoMode == 0)//电机方向反转
			{
				OLED_ShowString(49,4,"b");
				sprintf (str,"b_Motor_DIR_Back\r\n" );//格式化发送字符串到TCP服务器
				ESP8266_SendString ( ENABLE, str, 0, Single_ID_0 );
				DirBack = 1;
				RevData = 0;
			}
			else if(RevData == 'c' && AutoMode == 0)//电机启动
			{
				OLED_ShowString(49,4,"c");
				sprintf (str,"c_Motor_Go\r\n" );//格式化发送字符串到TCP服务器
				ESP8266_SendString ( ENABLE, str, 0, Single_ID_0 );
				PulForward = 1;
				RevData = 0;
			}
			else if(RevData == 'd' && AutoMode == 0)//电机停止
			{
				OLED_ShowString(49,4,"d");
				sprintf (str,"d_Motor_Stop\r\n" );//格式化发送字符串到TCP服务器
				ESP8266_SendString ( ENABLE, str, 0, Single_ID_0 );
				PulBack = 1;
				RevData = 0;
			}
			else if(RevData == 'e' && AutoMode == 0)//电机进入自动巡航模式
			{
				OLED_ShowString(49,4,"e");
				sprintf (str,"e_Auto\r\n" );//格式化发送字符串到TCP服务器
				ESP8266_SendString ( ENABLE, str, 0, Single_ID_0 );
				PulBack = 1;//电机停止工作
				DirForward = 1;//电机设为正转
				AutoMove = 1;
				RevData = 0;
			}
			else if(RevData == 'f' && AutoMode == 1)//电机进入手动巡航模式
			{
				OLED_ShowString(49,4,"f");
				sprintf (str,"f_ATcontol\r\n" );//格式化发送字符串到TCP服务器
				ESP8266_SendString ( ENABLE, str, 0, Single_ID_0 );
				AT_Move = 1;
				PulBack = 1;
				RevData = 0;
			}
			else//不是正确的字符
			{
				if(RevData != 0)
				{
					OLED_ShowString(49,4,"?");
					sprintf (str,"cant identified\r\n" );//格式化发送字符串到TCP服务器
					ESP8266_SendString ( ENABLE, str, 0, Single_ID_0 );
					RevData = 0;
				}
			}
//-----------------------------------------------------------------------------------------------						  
			delay_ms(10);
			ClkTime++;
//			if(ClkTime % 20 == 0)
//			{
//				sprintf (str,"d" );//每隔0.2s给服务器发个消息
//				ESP8266_SendString ( ENABLE, str, 0, Single_ID_0 );
//			}
			if(ClkTime % 500 == 0 && AutoMode == 1)//自动巡航模式下每隔1.5秒切换电机移动方向
			{
				PulForward = 1;
				if(DirFlag == 1)//如果当前电机方向为正转
				{
					DirBack = 1;//电机反转
					DirFlag = 0;
				}
				else//如果电机方向为反转，就设置为正转
				{
					DirForward = 1;//电机正转
					DirFlag = 1;
				}
			}
			if(ClkTime == 1500)
			{
				ClkTime = 0;
			}
			
					
//-------------------------------------------------------------------------------------------------------//保证TCP时刻在线
        if(TcpClosedFlag) //判断是否失去连接
        {
					PulBack = 1;//断网保护，当检查到tcp没有连接时电机停止工作。
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

