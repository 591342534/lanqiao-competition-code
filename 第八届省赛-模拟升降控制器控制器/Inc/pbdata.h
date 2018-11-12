#ifndef __PBDATA_H
#define __PBDATA_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_tim.h"
#include "stdio.h"

#include "stm32f10x_it.h"
#include "lcd.h"
#include "led.h"
#include "key.h"
#include "rtc.h"
#include "pwm.h"

extern u32 TimingDelay;              //��ʱ����
extern uint32_t TimeDisplayControl;  //LCD��ʾ���Ʊ���
extern uint32_t PlatformMove;        //ƽ̨�ƶ�ʱ�����
extern uint32_t PlatformTiming;      //ƽ̨ͣ��ʱ�����
extern uint32_t PlatformTwinkle;     //ƽ̨��˸ʱ�����
extern uint8_t TwinkleTemp;          //ƽ̨��˸��ʱ����
extern uint8_t TwinkleControl;       //ƽ̨��˸ʹ�ܱ���
extern uint8_t PlatformControl;      //¥��ʵʱ��ʾ����
extern uint8_t PlatformTemp;		  	 //¥����ʱ��ʾ����
extern uint8_t MoveTemp;		      	 //¥����ʱ��˸����
extern uint8_t PlatformTempControl;	 //¥����ʱ���Ʊ���
extern uint8_t PlatformData[20];     //¥����ʾ���ݱ���

void Delay_Ms(u32 nTime);         //��ʱ����
void PBDATA_Configuration(void);  //����������ʼ��
void Platform_Display(void);      //��ǰ¥����ʾ����
void Platform_Twinkle(void);      //ƽ̨��˸���ƺ���
void Platform_Move(void);         //ƽ̨�ƶ����ƺ���

#endif
