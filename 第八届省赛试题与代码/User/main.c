/*************************************************************************************
* �ļ����ƣ��ڰ˽����ű�ʡ���⡪��ģ������������																			 *
* ʵ��Ŀ�ģ�ʵ��CT117EǶ��ʽ�������LCD��ʾ���������ܡ�PWM�����LED��ˮָʾ��RTC���� *
* ����˵��������TIM1 CH1��TIM3 CH3ΪPWM���ģʽ��TIM3���1KHZ��ռ�ձ�80%��60%�ź�		 *
*          	TIM1���2KHz��ռ�ձ�60%��50%�ź�																				 *
* ���������Keil uVision4 MDK4.50                                                    *
* Ӳ��������CT117EǶ��ʽ������/Rev1.1                                                *
* ��Ʒ��Դ�����Ӵ��¹����ҡ���Ruby20zh60                                               *
* ���ڰ汾��2018-3-23																							          				 *
*************************************************************************************/
#include "pbdata.h"

int main(void)
{
		SystemInit();
		PBDATA_Configuration();
	
		STM3210B_LCD_Init();
		RTC_Configuration();
		LED_Configuration();
	  KEY_Configuration();
		PWM_Configuration();

		LCD_Clear(Blue2);
		LCD_SetBackColor(Blue2);
		LCD_SetTextColor(White);
		Delay_Ms(10);
		LCD_DisplayStringLine(Line2 ,"  Current Platform  ");
		LED_Control(LEDALL,0);
		PWM_Control(Pwm1Control ,Pwm2Control);
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
	
		while(1)
		{		
				TIME_Display();		
				Platform_Display();
				Platform_Move();	
				LED_Display();
				LED_CycleControl(LedControl);	
				KEY_Control(KeyControl);
				PWM_OutputControl();
		}
}
