 /*�������������*/
 /*����ʵ��ȫ�����ݣ���bug*/
 /*���ߣ�����Ȼ*/

#include "stm32f10x.h"
#include <stdio.h>
#include "lcd.h"
#include "LED.h"
#include "i2c.h"
#include "gong.h"


u8 string[20];
u8 RXbuf[20];

u8 SignalPA9=0,PB14=0;
u8 shezhi=0;
u8 qiehuan=0;
u8 i=0;

u8 current_f=0;

u8 open=0;
u8 status=1;

u16 run = 0xffff;
u16 set_f=1000;

float temp1;
float temp2;
float pulse1;
float pulse2;
float CCR2_Val=550.0;
float period=999.0;
float f=1;

u32 TimingDelay = 0;

float ADC_VALUE;

void TIM1_Configuration(uint16_t period,uint16_t CCR2_Val);
void TIM1_cmd(void);
void TIM1_dcmd(void);
void PA9_Display(void);
void PB14_Display(void);
void Period_Display(void);


void Delay_Ms(u32 nTime);
void KEY_Scan(void);
void AD_Read(void);

void IIC_Write(unsigned char add,unsigned char dat);
unsigned char IIC_Read(unsigned char add);
void LCD_Init(void);
void LED_RUN(u16 run);

int main(void)
{
    SysTick_Config(SystemCoreClock/1000);

	LCD_Init();
	GPIO_Int();

	TIM1_Configuration(period,CCR2_Val);
	TIM1_cmd();
	i2c_init();


  while (1)
  {
  	KEY_Scan();
	
    if(shezhi==0)
	{
	AD_Read();
	PA9_Display();
	PB14_Display();
	Period_Display();

    }
	
   }
  
}

void AD_Read(void){

	   ADC_SoftwareStartConvCmd(ADC1, ENABLE);  
	   ADC_VALUE = ADC_GetConversionValue(ADC1)*3.3/0xFFF;
	   sprintf((char*)string,"%s%.2fV     "," Votage:",ADC_VALUE);//�¶�Ϊfloat
	   LCD_DisplayStringLine(Line3,string);

	  	    
}

void PA9_Display(void)
{
        temp1=CCR2_Val/(period+1);
   		pulse1=temp1*100;
   		sprintf((char*)string, "%s%.0f%%   ","Signal:PA9:",pulse1);
	    LCD_DisplayStringLine(Line5, (unsigned char *)string);

}

void PB14_Display(void)
{		
        temp2=1.0-CCR2_Val/(period+1);
    	pulse2=temp2*100;
   		sprintf((char*)string, "%s%.0f%%   ","Signal:PB14:",pulse2);
	    LCD_DisplayStringLine(Line6, (unsigned char *)string);
}

void Period_Display(void)
{
		sprintf((char*)string, "%s%.0fKHz   ","      ",f);
	    LCD_DisplayStringLine(Line7, (unsigned char *)string);

}


void LED_RUN(u16 run)
{
     
	  GPIO_SetBits(GPIOC,run);
	  GPIO_SetBits(GPIOD,GPIO_Pin_2);
	  GPIO_ResetBits(GPIOD,GPIO_Pin_2);

}

void TIM1_cmd(void)
{
     TIM_Cmd(TIM1,ENABLE);
  	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	 
}

void TIM1_dcmd(void)
{
  	TIM_CtrlPWMOutputs(TIM1, DISABLE);
	 TIM_Cmd(TIM1,DISABLE); 
}


void KEY_Scan(void)
{
    if(RB1==0)
   {
   	  Delay_Ms(10);
		 if(RB1==0)
   		  {
		  if(shezhi==0)
		  {
		  	 if(open==0)
		   {
		      open=1;
			  LCD_DisplayStringLine(Line4,(unsigned char*)" Status: OFF         ");	
			  status=0;
		   	  run = run |0x0100;
       		  LED_RUN(run);
			  TIM1_dcmd();
			  

		   }
		   else if(open==1)
		   {
		   	  open=0;
			  LCD_DisplayStringLine(Line4,(unsigned char*)" Status: ON          ");
			  status=1;
		   	  run = run & 0xfeff;
	   		  LED_RUN(run);
			  TIM1_cmd();
			
		   }
		  }
		 		
		  }					
		  while(!RB1);
   }

    if(RB2==0)
   {
   	  Delay_Ms(10);
		 if(RB2==0)
   		  {
           if(shezhi==0)
			{
			shezhi=1;
			LCD_SetBackColor(Blue);
			LCD_SetTextColor(White);
			LCD_DisplayStringLine(Line0,(unsigned char*)"                    ");	
		    LCD_DisplayStringLine(Line1,(unsigned char*)"                    ");
			LCD_DisplayStringLine(Line2,(unsigned char*)"                    ");
			LCD_DisplayStringLine(Line3, (unsigned char*)"    Para Setup     ");
			LCD_DisplayStringLine(Line4,(unsigned char*)"                    ");										
				sprintf((char*)string, "   %s%0.fKHz  ","Frq:  ",f);
			LCD_DisplayStringLine(Line5, (unsigned char *)string);
		    LCD_DisplayStringLine(Line6, (unsigned char *)"                   ");
		    LCD_DisplayStringLine(Line7, (unsigned char *)"                   ");
			}
			else if(shezhi==1)
			{
			    shezhi=0;
				LCD_SetBackColor(Blue);
      	        LCD_SetTextColor(White);
			  	LCD_DisplayStringLine(Line0,(unsigned char*)"                    ");	
				LCD_DisplayStringLine(Line1,(unsigned char*)"   Paras Interface  ");
			    LCD_DisplayStringLine(Line2,(unsigned char*)"                    ");	
	  			sprintf((char*)string,"%s%.2fV     "," Votage:",ADC_VALUE);
	  			LCD_DisplayStringLine(Line3,string);
				PA9_Display();
				PB14_Display();
				Period_Display();				
			    LCD_DisplayStringLine(Line8,(unsigned char*)"                    ");
			    LCD_DisplayStringLine(Line9,(unsigned char*)"                    ");

				if(status==1)
				{
					LCD_DisplayStringLine(Line4,(unsigned char*)" Status: ON            ");						
				}
				 else if(status==0)
				{
					LCD_DisplayStringLine(Line4,(unsigned char*)" Status: OFF            ");						
				}
			
			}
			}
		   while(!RB2);
   }
  
    if(RB3==0)
   {
   	  Delay_Ms(10);
		 if(RB3==0)
   		  {
		  	if(shezhi==1)
			{
			if(f<10)
			f=f+1;
//			period=-(100*f)+1100-1;
			sprintf((char*)string, "   %s%0.fKHz  ","Frq:  ",f);
			LCD_DisplayStringLine(Line5, (unsigned char *)string);
			}
			else 
			{ 
			sprintf((char*)string, "   %s%0.fKHz  ","Frq:  ",f);
			LCD_DisplayStringLine(Line5, (unsigned char *)string);
			
			}
	
			   
		  }
		while(!RB3);
      }

    if(RB4==0)
   {
   	  Delay_Ms(10);
		 if(RB4==0)
   		  {		
		   }
		 while(!RB4);
   }
}



void LCD_Init(void)
{
    STM3210B_LCD_Init();
	LCD_Clear(Blue);
	LCD_SetBackColor(Blue);
	LCD_SetTextColor(White);

	LCD_DisplayStringLine(Line0,(unsigned char*)"                    ");	
	LCD_DisplayStringLine(Line1,(unsigned char*)"   Paras Interface  ");
    LCD_DisplayStringLine(Line2,(unsigned char*)"                    ");	
	LCD_DisplayStringLine(Line3,(unsigned char*)" Votage:	         ");
	LCD_DisplayStringLine(Line4,(unsigned char*)" Status: ON         ");						
	LCD_DisplayStringLine(Line5,(unsigned char*)" Signal:PA9:        ");	
	LCD_DisplayStringLine(Line6,(unsigned char*)"        PB14:       ");		
	LCD_DisplayStringLine(Line7,(unsigned char*)"       1KHz         ");		
    LCD_DisplayStringLine(Line8,(unsigned char*)"                    ");
    LCD_DisplayStringLine(Line9,(unsigned char*)"                    ");


}


void Delay_Ms(u32 nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);	
}


void TIM1_Configuration(uint16_t period,uint16_t CCR2_Val)
{
	   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
       TIM_OCInitTypeDef  TIM_OCInitStructure;
	   TIM_BDTRInitTypeDef TIM_BDTRInitStructure;

	   	/******PWM��ƽ����ֵ**********/
//		uint16_t CCR2_Val=500;
 
	/******��һ���� ʱ����ʼ��**********/
       TIM_DeInit(TIM1); //����Ϊȱʡֵ
		 /*TIM1ʱ������*/
		 TIM_TimeBaseStructure.TIM_Period = 999;//װ��ֵ 
		 TIM_TimeBaseStructure.TIM_Prescaler = 71; //Ԥ��Ƶ(ʱ�ӷ�Ƶ)
		 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���
		 TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;   //������ʱ�ӷָ� �����ò���
		 TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;    //���ڼ�����ֵ  �����ò���
		 TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);    //��ʼ��TIMx��ʱ�������λ
		
  	/******�ڶ����� ���ģʽ��ʼ��**********/
		 /* Channel 1 Configuration in PWM mode ͨ��һ��PWM */
		 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;     //PWMģʽ2
		 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //����ͨ����Ч  PA8 
		 TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; //����ͨ��Ҳ��Ч PB13
 	/******�������� װ������ֵ**********/
		 TIM_OCInitStructure.TIM_Pulse = CCR2_Val;        //ռ��ʱ��  144 ����40��ʱ��Ϊ�ߣ���������������෴	   CCR2_Val
		 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;   //�������
		 TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;  //ʹ�ܸ�ͨ�����
	     //���漸�������Ǹ߼���ʱ���Ż��õ���ͨ�ö�ʱ����������
		 TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;     //�����˵ļ��� 
		 TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable;//ʹ�ܻ�������� 
		 TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;  //����״̬�µķǹ���״̬ ����
		 TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;  //�Ȳ���
		
		 TIM_OC2Init(TIM1,&TIM_OCInitStructure);       //����ʼ������TIMxͨ��1����2.0��ΪTIM_OCInit
     	 TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);//ʹ��Ԥװ�ؼĴ���

	/******������ɲ���������ã��߼���ʱ�����еģ�ͨ�ö�ʱ����������******/
		TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Disable;//����ģʽ�����ѡ��
		TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Disable;//����ģʽ�����ѡ�� 
		TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;    //��������
		TIM_BDTRInitStructure.TIM_DeadTime = 50;                  //����ʱ������
		TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;        //ɲ������ʹ��
		TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_Low;//ɲ�����뼫��
		TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;//�Զ����ʹ�� 
	
		TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure);

		TIM_ARRPreloadConfig(TIM1,ENABLE);	  //ʹ����װ�ؼĴ���
		 /* TIM1 counter enable����ʱ�� */
//		 TIM_Cmd(TIM1,ENABLE);
		      /* TIM1 Main Output Enable ʹ��TIM1����������*/
//		 TIM_CtrlPWMOutputs(TIM1, ENABLE);  //pwm���ʹ�ܣ�һ��Ҫ�ǵô�
	/*TIM_OC1PreloadConfig(),TIM_ARRPreloadConfig();�������������Ƶ���ccr1��arr��Ԥװ��ʹ�ܣ�
	ʹ�ܺ�ʧ�ܵ�������ǣ�ʹ�ܵ�ʱ���������Ĵ����Ķ�д��Ҫ�ȴ��и����¼�����ʱ���ܱ��ı�
	���������������Ǹ���ʱ�䣩��
	ʧ�ܵ�ʱ�����ֱ�ӽ��ж�д��û���ӳ١�
	 ���������е�����Ҫ�ı�pwm��Ƶ�ʺ�ռ�ձȵ��ã�TIM_SetAutoreload() 
	TIM_SetCompare1()�����������Ϳ����ˡ�*/

}

