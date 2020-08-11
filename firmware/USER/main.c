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

uint8_t PulForward = 0, DirForward = 0, PulBack = 0, DirBack = 0;


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
		TIM3_PWM_Init(1000 - 1,72 - 1);	 //new 72000000/72 = 1000000/1000 = 1khz(1ms)������1000����1ms
		LED_Init();
	 
		measure1();
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
			if(USART_RX_STA&0x8000)//�յ���Ϣʱ
			{					   
				len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
				printf("\r\n�����͵���ϢΪ:\r\n\r\n");
				for(t=0;t<len;t++)//��ӡ�����յ���Ϣ
				{
					USART_SendData(USART1, USART_RX_BUF[t]);//�򴮿�1��������
					while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
				}
				printf("\r\n\r\n");//���뻻��
				USART_RX_STA=0;
			}else
			{
				times++;
				if(times%200==0)printf("����������,�Իس�������\n");  
				delay_ms(10);   
			}
	

	}	  
}

