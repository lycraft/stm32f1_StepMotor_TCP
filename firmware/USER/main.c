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

int sa=0,sb=0,sc=0,sd=0;
uint8_t modetemp = 6,mode = 4;//modetemp��ָʾλ�� ָ��ѡ�е�mode��==============mode0-4
uint8_t flag = 1;


void measure1()//mode1��Ļ��ˢ�µĶ���
{
		OLED_Clear();
	
		OLED_ShowString(0,2,"PWM1:");
		OLED_ShowString(41,2,"100");
		OLED_ShowString(73,2,"hz");
	
		OLED_ShowString(0,4,"PWM2:");
		OLED_ShowString(41,4,"1.1k");
		OLED_ShowString(73,4,"hz");
	 

	
}

void meau()//������
{
		OLED_Clear();
		OLED_DrawBMP(96,0,128,2,BMP2);	
		OLED_ShowString(0,6,"Meau");
		OLED_ShowString(40,0,"PWM");
		OLED_ShowString(40,2,"USART");
		OLED_ShowString(40,4,"others");
		OLED_ShowString(72,6,"Lycraft");
}

void voltage1()//ADC���治ˢ�µ�
{
		OLED_Clear();
		

}
int main(void)
 {	
		vu8 key=0;
	 	u16 len;	
		u16 times=0;
	  u16 t; 
	 
		delay_init();	    	 //��ʱ������ʼ��	  
		NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 	
		OLED_Init();			//��ʼ��OLED  
//		KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
	 	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
		EXTIX_Init();
		TIM3_PWM_Init(1000 - 1,72 - 1);	 //����Ƶ��PWMƵ��=72000000/900=80Khz------new 72000000/72 = 1000000/1000 = 1khz(1ms)������1000����1ms
	 
	while(1) 
	{		
		if(modetemp == 3||modetemp == 9)//======��ͷ��־λ
		{
			modetemp = 6;
		}
//========================================================================Meau
		if(mode % 4 == 0)
		{
			if(flag == 1)//ִֻ��һ�ε�
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
			if(flag == 1)//ִֻ��һ�ε�
			{
				measure1();
				flag--;
			}
			
			delay_ms(10);	 
											 
			TIM_SetCompare2(TIM3,500);	//ռ�ձ�Ϊ50% 500/1000 = 50%
			
			
		}	
//========================================================================��ѹ��������ADC
		if(mode % 4 == 2)
		{
			if(flag == 1)//ִֻ��һ�ε�
			{
				voltage1();
				flag--;
			}
			if(USART_RX_STA&0x8000)
			{					   
				len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
				printf("\r\n�����͵���ϢΪ:\r\n\r\n");
				for(t=0;t<len;t++)
				{
					USART_SendData(USART1, USART_RX_BUF[t]);//�򴮿�1��������
					while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
				}
				printf("\r\n\r\n");//���뻻��
				USART_RX_STA=0;
			}else
			{
				times++;
				if(times%5000==0)
				{
					printf("\r\nս��STM32������ ����ʵ��\r\n");
					printf("����ԭ��@ALIENTEK\r\n\r\n");
				}
				if(times%200==0)printf("����������,�Իس�������\n");  
				delay_ms(10);   
			}
			
			
		}	
//=========================================================================others
			if(mode % 4 == 3)
		{
			if(flag == 1)//ִֻ��һ�ε�
			{
				OLED_Clear();
				flag--;				
			}
			OLED_DrawBMP(0,0,128,8,BMP1);
			delay_ms(100);
		
		}
	}	  
}

