//////////////////////////////////////////////////////////////////////////////////	 
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��������   : OLED 4�ӿ���ʾ����(51ϵ��)
//              ˵��: 
//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              D0   ��PA8��SCL��
//              D1   ��PB15��SDA��
//              RES  ��PB14
//              DC   ��PB13
//              CS   ��PA3               
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

void measure1()//mode1��Ļ��ˢ�µĶ���
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
//��ʼ������ 
//	 
//////////////////////////////////////////////////////////////////////////////////  
		vu8 key=0;
	 	uint8_t res;
	  char str[100]={0};
		
//////////////////////////////////////////////////////////////////////////////////	 
//��ʼ������	  
//////////////////////////////////////////////////////////////////////////////////  	 
		delay_init();	    	 //��ʱ������ʼ��	  
		NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 	
		OLED_Init();			//��ʼ��OLED  
//		KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
	 	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	  ESP8266_Init(115200);
		EXTIX_Init();
		TIM3_PWM_Init(1000 - 1,72 - 1);	 //new 72000000/72 = 1000000/1000 = 1khz(1ms)������1000����1ms
		LED_Init();

//////////////////////////////////////////////////////////////////////////////////	 
//ִֻ��һ�εĺ���  
////////////////////////////////////////////////////////////////////////////////// 		
		measure1();
	 
		OLED_ShowString(0,0, "configing_0");	
    ESP8266_AT_Test();
		OLED_ShowString(0,0, "configing_1");	 
		printf("��������ESP8266\r\n");
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
		printf("\r\n�������");
		OLED_ShowString(0,0, "config is ok!");	
		
//////////////////////////////////////////////////////////////////////////////////	 
//һֱѭ���ĺ��� 
////////////////////////////////////////////////////////////////////////////////// 			
	while(1) 
	{		
		
//========================================================================��������������Ʒ����ٶ�
			if(PulForward == 1)
			{
				delay_ms(10);	 
				TIM_SetCompare2(TIM3,500);	//ռ�ձ�Ϊ50% 500/1000 = 50%	//PB5������PUL�������PWM����
				OLED_ShowString(41,2,"1000");
				PulForward--;
			}				
			if(PulBack == 1)
			{
				delay_ms(10);
				TIM_SetCompare2(TIM3,1000);	//ֹͣ���pwm���������ű�Ϊ�͵�ƽ
				OLED_ShowString(41,2,"----");
//				TIM_PrescalerConfig(TIM3,5000,TIM_PSCReloadMode_Immediate);//�޸�psr���޸�ռ�ձ�
				PulBack--;
			}				
			if(DirForward == 1)
			{
				delay_ms(10);
			  GPIO_SetBits(GPIOB,GPIO_Pin_6); //��������PB6	
//				GPIOB->BRR=GPIO_Pin_6;
				OLED_ShowString(49,4,"+");
				DirForward--;
			}
			if(DirBack == 1)
			{
				delay_ms(10);
				GPIO_ResetBits(GPIOB,GPIO_Pin_6);//��������PB6
//				GPIOB->BSRR=GPIO_Pin_6;
				OLED_ShowString(49,4,"-");
				DirBack--;
			}
//========================================================================����	 
      		if(RevData == 'a')
					{
						OLED_ShowString(49,4,"a");
						sprintf (str,"a������������������\r\n" );//��ʽ�������ַ�����TCP������
						ESP8266_SendString ( ENABLE, str, 0, Single_ID_0 );
						DirForward = 1;
						RevData = 0;
					}
					else if(RevData == 'b')
					{
						OLED_ShowString(49,4,"b");
						sprintf (str,"b������������������\r\n" );//��ʽ�������ַ�����TCP������
						ESP8266_SendString ( ENABLE, str, 0, Single_ID_0 );
						DirBack = 1;
						RevData = 0;
					}
					else if(RevData == 'c')
					{
						OLED_ShowString(49,4,"c");
						sprintf (str,"c�������\r\n" );//��ʽ�������ַ�����TCP������
						ESP8266_SendString ( ENABLE, str, 0, Single_ID_0 );
						PulForward = 1;
						RevData = 0;
					}
					else if(RevData == 'd')
					{
						OLED_ShowString(49,4,"d");
						sprintf (str,"d���ֹͣ\r\n" );//��ʽ�������ַ�����TCP������
						ESP8266_SendString ( ENABLE, str, 0, Single_ID_0 );
						PulBack = 1;
						RevData = 0;
					}
					else
					{
						if(RevData != 0)
						{
							OLED_ShowString(49,4,"?");
							sprintf (str,"������ȷָ��\r\n" );//��ʽ�������ַ�����TCP������
							ESP8266_SendString ( ENABLE, str, 0, Single_ID_0 );
							RevData = 0;
						}
					}
						  
				delay_ms(10);
			
        if(TcpClosedFlag) //�ж��Ƿ�ʧȥ����
        {
            ESP8266_ExitUnvarnishSend(); //�˳�͸��ģʽ
            do
            {
                res = ESP8266_Get_LinkStatus();     //��ȡ����״̬
            }   
            while(!res);

            if(res == 4)                     //ȷ��ʧȥ���ӣ�����
            {
                
                
                while (!ESP8266_JoinAP(User_ESP8266_SSID, User_ESP8266_PWD ) );
                while (!ESP8266_Link_Server(enumTCP, User_ESP8266_TCPServer_IP, User_ESP8266_TCPServer_PORT, Single_ID_0 ) );        
            } 
            while(!ESP8266_UnvarnishSend());                    
        }
				

	}	  
}

