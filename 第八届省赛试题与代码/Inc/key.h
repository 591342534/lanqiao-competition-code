#ifndef __KEY_H
#define __KEY_H

#include "pbdata.h"

#define KEY1_IN	  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define KEY2_IN	  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)
#define KEY3_IN   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)
#define KEY4_IN   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2)

extern uint8_t KeyOne;         //KEY1���Ʊ���
extern uint8_t KeyTwo;         //KEY2���Ʊ���
extern uint8_t KeyThree;       //KEY3���Ʊ���
extern uint8_t KeyFour;        //KEY4���Ʊ���
extern uint8_t Key1Control;    //KEY1��ȡ����
extern uint8_t Key2Control;	   //KEY2��ȡ����
extern uint8_t Key3Control;    //KEY3��ȡ����
extern uint8_t Key4Control;    //KEY4��ȡ����
extern uint8_t KeyControl;     //KEYʹ�ܿ��Ʊ���
extern uint32_t KeyTiming;     //KEY��ʱ���Ʊ���
extern uint8_t KeyScanContuol; //KEYɨ����Ʊ���

uint8_t KeyScan(void);             //KEYɨ�躯��
void KEY_Configuration(void);      //KEY���ú���
void KEY_Control(uint8_t KeyTemp); //KEYʹ�ܺ���

#endif
