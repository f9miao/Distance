#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

extern int receiveNum; //�ۼƽ����ֽ���
extern int ready; // ���������Ƿ����
extern u8 low; 	// ��8λ����
extern u8 high; // ��8λ����

void uart_init(u32 bound);

#endif

