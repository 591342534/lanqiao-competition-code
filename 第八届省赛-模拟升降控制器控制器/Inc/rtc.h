#ifndef __RTC_H
#define __RTC_H

#include "pbdata.h"

#define HH 12  //Hour
#define MM 50  //Minute
#define SS 55  //Second

extern uint8_t TimeData[20];     //TIME��ʾ���ݱ���

void RTC_Configuration(void);    //RTC���ú���
void TIME_Display(void);         //TIME��ʾ����
void TIME_Read(u32 TimeRead);    //TIME��ȡ����

#endif
