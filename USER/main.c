#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "usart3.h"

int main(void)
{
	delay_init();	    	//��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 	//����1��ʼ��
	usart3_init(9600);	//����3��ʼ��
 	LED_Init();			    //LED�˿ڳ�ʼ��
	LCD_Init();					//LCD������ʼ��
	POINT_COLOR=BLUE;		//LCDʹ����ɫ����

	LED0=0; // ��ɫ
	LED1=0; // ��ɫ
	
	while(1) 
	{
		
		if(ready==0){
			USART_SendData(USART1, 0x55);//�򴮿�1��������
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
		}
		
		if(ready==1){
			int distance = 0; // ����
			int mod = 0;			// ����
			int div = 0;			// ��
			int strFlag = 0;
			u8 strResult[10] = {0};

			// ���������
			distance = high<<8;
			distance |= low;
			
			// ��LCD��Ļ����ʾ
			LCD_Clear(WHITE);
			LCD_ShowString(30,40,210,24,24,"Elite STM32F1 ^_^"); 
			LCD_ShowString(30,70,200,16,16,"TFTLCD TEST");
			LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK"); 

			LCD_ShowString(30,170,200,16,16, "high:");
			LCD_ShowxNum(130,170,high,			5,12, 0);
			
			LCD_ShowString(30,190,200,16,16, "low:");
			LCD_ShowxNum(130,190,low,				5,12, 0);
			
			LCD_ShowString(30,210,200,16,16, "result(mm):");
			LCD_ShowxNum(130,210,distance,	5,12, 0);

			// �ж��Ƿ��г������복λ, ��������С��400mmʱ�����,�����̵���
			if(distance < 400){
				LED0=0;
				LED1=1;
			}
			else{
				LED0=1;
				LED1=0;
			}
			
			// ����ֵתΪ�ַ���,��ͨ������͸���������豸,��ʽ��: distance:340
			u3_printf("distance:");
			
			div = distance;
			while(div!=0){
				mod = div % 10; // ����
				div = div / 10; // ȥ����λ��
				strResult[strFlag++] = '0' + mod;
			}
			
			while(strFlag>=0){
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������   
				USART_SendData(USART3, strResult[--strFlag]); 
			}
			
			u3_printf("\r\n");//����
			
			
			// ���þ�������, ��ʼ��һ��ѭ��
			low = 0;
			high = 0;
			ready = 0;
			receiveNum = 0;
		}
		
		delay_ms(300);	
	} 
}

