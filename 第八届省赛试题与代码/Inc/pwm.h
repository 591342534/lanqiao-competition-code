#ifndef __PWM_H
#define __PWM_H

#include "pbdata.h"

extern float Pwm1Temp;        //�����е��ռ�ձ���ʱ����
extern float Pwm2Temp;        //�����ŵ��ռ�ձ���ʱ����
extern float Pwm1Control;     //�����е��ռ�ձȿ��Ʊ���
extern float Pwm2Control;     //�����ŵ��ռ�ձȿ��Ʊ���

void PWM_Configuration(void); //��������ź����ú���
void PWM_OutputControl(void); //��������ź�������ƺ���
void PWM_Control(float Channel1Pulse, float Channel2Pulse);//��������źſ��ƺ���

#endif
