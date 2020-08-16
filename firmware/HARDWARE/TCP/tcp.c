#include "tcp.h"
#include "usart.h"
#include "esp8266.h"
#include "delay.h"
#include "stdio.h"
#include "string.h"
#include "stm32f10x.h"
#include "oled.h"

volatile u8 TcpClosedFlag = 0;

//void ESP8266_STA_TCPClient_Test(void)
//{
//    u8 res;
//	
//    char str[100]={0};
//		OLED_ShowString(0,0, "configing_0");	
//    ESP8266_AT_Test();
//		OLED_ShowString(0,0, "configing_1");	 
//		printf("正在配置ESP8266\r\n");
//    ESP8266_Net_Mode_Choose(STA);
//		OLED_ShowString(0,0, "configing_2");
//    while(!ESP8266_JoinAP(User_ESP8266_SSID, User_ESP8266_PWD));
//		OLED_ShowString(0,0, "configing_3");
//    ESP8266_Enable_MultipleId ( DISABLE );
//		OLED_ShowString(0,0, "configing_4");
//    while(!ESP8266_Link_Server(enumTCP, User_ESP8266_TCPServer_IP, User_ESP8266_TCPServer_PORT, Single_ID_0));
//		OLED_ShowString(0,0, "configing_5");
//    while(!ESP8266_UnvarnishSend());
//		OLED_ShowString(0,0, "configing_6");
//		printf("\r\n配置完成");
//		OLED_ShowString(0,0, "config is ok!");	 
//    while ( 1 )
//    {       
//			  sprintf (str,"test sign" );//格式化发送字符串到TCP服务器
//				OLED_ShowString(0,6, "sending");	 
//        ESP8266_SendString ( ENABLE, str, 0, Single_ID_0 );
//        delay_ms(1000);
//        if(TcpClosedFlag) //判断是否失去连接
//        {
//            ESP8266_ExitUnvarnishSend(); //退出透传模式
//            do
//            {
//                res = ESP8266_Get_LinkStatus();     //获取连接状态
//            }   
//            while(!res);

//            if(res == 4)                     //确认失去连接，重连
//            {
//                
//                
//                while (!ESP8266_JoinAP(User_ESP8266_SSID, User_ESP8266_PWD ) );
//                while (!ESP8266_Link_Server(enumTCP, User_ESP8266_TCPServer_IP, User_ESP8266_TCPServer_PORT, Single_ID_0 ) );        
//            } 
//            while(!ESP8266_UnvarnishSend());                    
//        }
//    }   
//}
