#ifndef _TEXT_H
#define _TEXT_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//�ֿ���� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/15
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 


void Get_MzMat(unsigned char *code,unsigned char *mat,u8 size);
void Show_Font(u16 x,u16 y,u8 *font,u8 size,u8 mode);
void Show_Str(u16 x,u16 y,u16 width,u16 height,u8 *str,u8 size,u8 mode);


#endif

