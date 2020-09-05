#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "usart3.h"

int main(void)
{
	delay_init();	    	//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 	//串口1初始化
	usart3_init(9600);	//串口3初始化
 	LED_Init();			    //LED端口初始化
	LCD_Init();					//LCD驱动初始化
	POINT_COLOR=BLUE;		//LCD使用蓝色字体

	LED0=0; // 红色
	LED1=0; // 绿色
	
	while(1) 
	{
		
		if(ready==0){
			USART_SendData(USART1, 0x55);//向串口1发送数据
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
		}
		
		if(ready==1){
			int distance = 0; // 距离
			int mod = 0;			// 余数
			int div = 0;			// 商
			int strFlag = 0;
			u8 strResult[10] = {0};

			// 计算出距离
			distance = high<<8;
			distance |= low;
			
			// 在LCD屏幕上显示
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

			// 判定是否有车辆进入车位, 距离测距器小于400mm时红灯亮,否则绿灯亮
			if(distance < 400){
				LED0=0;
				LED1=1;
			}
			else{
				LED0=1;
				LED1=0;
			}
			
			// 距离值转为字符串,并通过蓝牙透传给其他设备,格式如: distance:340
			u3_printf("distance:");
			
			div = distance;
			while(div!=0){
				mod = div % 10; // 余数
				div = div / 10; // 去除个位数
				strResult[strFlag++] = '0' + mod;
			}
			
			while(strFlag>=0){
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕   
				USART_SendData(USART3, strResult[--strFlag]); 
			}
			
			u3_printf("\r\n");//换行
			
			
			// 重置距离数据, 开始下一个循环
			low = 0;
			high = 0;
			ready = 0;
			receiveNum = 0;
		}
		
		delay_ms(300);	
	} 
}

