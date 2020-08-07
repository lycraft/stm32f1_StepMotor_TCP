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

int sa=0,sb=0,sc=0,sd=0;
int wendu,shidu;
uint8_t modetemp = 6,mode = 4;//modetemp��ָʾλ�� ָ��ѡ�е�mode��==============mode0-4
uint8_t flag = 1;


void measure1()//mode1��Ļ��ˢ�µĶ���
{
		OLED_Clear();
	
		OLED_ShowString(0,2,"PWM1:");
	
		OLED_ShowString(0,4,"PWM2:");

	 
	 	OLED_ShowCHinese(0,6,0);
		OLED_ShowCHinese(18,6,1);
		OLED_ShowCHinese(36,6,2);//��
		OLED_ShowCHinese(54,6,3);//��
		OLED_ShowCHinese(72,6,4);//��
		OLED_ShowCHinese(90,6,5);//��
		OLED_ShowCHinese(108,6,6);//Ļ
	
}

void meau()//������
{
		OLED_Clear();
		OLED_DrawBMP(96,0,128,2,BMP2);	
		OLED_ShowString(0,6,"Meau");
		OLED_ShowString(40,0,"PWM");
		OLED_ShowString(40,2,"ADC");
		OLED_ShowString(40,4,"others");
		OLED_ShowString(72,6,"Lycraft");
}

void voltage1()//ADC���治ˢ�µ�
{
		OLED_Clear();
		OLED_ShowCHinese(0,2,10);//��
		OLED_ShowCHinese(18,2,11);//ѹ
		OLED_ShowCHinese(0,4,10);//��
		OLED_ShowCHinese(18,4,12);//��
		
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
	 
		delay_init();	    	 //��ʱ������ʼ��	  
		NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 	
		OLED_Init();			//��ʼ��OLED  
//		KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
		EXTIX_Init();
		
	 
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
			
			
			
		}	
//========================================================================��ѹ��������ADC
		if(mode % 4 == 2)
		{
			if(flag == 1)//ִֻ��һ�ε�
			{
				voltage1();
				flag--;
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

