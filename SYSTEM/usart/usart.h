#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

extern int receiveNum; //累计接收字节数
extern int ready; // 距离数据是否就绪
extern u8 low; 	// 低8位距离
extern u8 high; // 高8位距离

void uart_init(u32 bound);

#endif

