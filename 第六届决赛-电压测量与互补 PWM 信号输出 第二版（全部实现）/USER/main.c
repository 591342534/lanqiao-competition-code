 /*������������⡪���������*/	
 /*ʵ��ȫ�����ݣ���bug,*/
 /*���ߣ�����Ȼ*/

#include "stm32f10x.h"
#include <stdio.h>
#include "lcd.h"
#include "LED.h"
#include "i2c.h"
#include "gong.h"


u8 string[20];

u8 SignalPA9=0,PB14=0;
u8 shezhi=0;
u8 qiehuan=0;
u8 i=0;

u8 open=0;
u8 status=1;

u16 run = 0xffff;

float temp1;
float temp2;
float pulse1;
float pulse2;
float CCR2_Val=550.0;
float period=999.0;
u8 f;



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

void LCD_Init(void);
void LED_RUN(u16 run);

int main(void)
{
    SysTick_Config(SystemCoreClock/1000);

	LCD_Init();
	GPIO_Int();
	i2c_init();
	Delay_Ms(10);
    if(IIC_Read(0x40)!=30)
	{
	Delay_Ms(10);
	IIC_Write(0x11,1);
	Delay_Ms(10);
	IIC_Write(0x40,30);
	}

	Delay_Ms(10);
	f=IIC_Read(0x11);

	TIM1_Configuration(period,CCR2_Val);
	TIM1_cmd();

//    sprintf((char*)string,"%s%d","period:",f);
//	 LCD_DisplayStringLine(Line9,string);


  while (1)
  {
  	KEY_Scan();
				
	
    if(shezhi==0)
	{
		AD_Read();
		if(open==0)
		{
		 	PA9_Display();
			PB14_Display();
			Period_Display();
		}
    }
	
   }
  
}

void AD_Read(void){

	   ADC_SoftwareStartConvCmd(ADC1, ENABLE);  
	   ADC_VALUE = ADC_GetConversionValue(ADC1)*3.3/0xFFF;
	   sprintf((char*)string,"%s%.2fV     "," Votage:",ADC_VALUE);
	   LCD_DisplayStringLine(Line3,string);
	   
	   switch(f)//Ƶ��ת��
	   {
        case 1:
	     period=999.0;
		 break;
	    case 2:
	     period=499.0;
		 break;
		case 3:
	     period=333.0;
		 break;
	    case 4:
	     period=250.0;
		 break;
		case 5:
	     period=199.0;
		 break;
		case 6:
	     period=166.0;
		 break;
		case 7:
	     period=141.0;
		 break;
		case 8:
	     period=124.0;
		 break;
		case 9:
	     period=110.0;
		 break;
		case 10:
	     period=99.0;
		 break;

	   	default:
			break;
		}
		
//		switch((int)period)
//	   {
//        case 999:
//	     f=1;
//		 break;
//	    case 499:
//	     f=2;
//		 break;
//		case 333:
//	     f=3;
//		 break;
//	    case 250:
//	     f=4;
//		 break;
//		case 199:
//	     f=5;
//		 break;
//		case 166:
//	     f=6;
//		 break;
//		case 141:
//	     f=7;
//		 break;
//		case 124:
//	     f=8;
//		 break;
//		case 110:
//	     f=9;
//		 break;
//		case 99:
//	     f=10;
//		 break;
//
//	   	default:
//			break;
//		}
		CCR2_Val=(ADC_VALUE/3.3)*(period+1);
			  	    
}

void PA9_Display(void)//PA9���
{
      temp1=CCR2_Val/(period+1);
   		pulse1=temp1*100;
			TIM1_Configuration(period,temp1*1000);
     	TIM1_cmd();
		  sprintf((char*)string, "%s%.0f%%   ","Signal:PA9:",pulse1);
	    LCD_DisplayStringLine(Line5, (unsigned char *)string);
}

void PB14_Display(void)//PB14���
{		
      temp2=1.0-CCR2_Val/(period+1);
    	pulse2=temp2*100;
   		sprintf((char*)string, "%s%.0f%%   ","Signal:PB14:",pulse2);
	    LCD_DisplayStringLine(Line6, (unsigned char *)string);
}

void Period_Display(void)//Ƶ�ʱ任��ʾ
{	
			TIM1_Configuration(period,CCR2_Val);
	    TIM1_cmd();
	    sprintf((char*)string, "%s%dKHz   ","       ",f);
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
    if(RB1==0) //�л�״̬
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

    if(RB2==0)	 //��������ҳ��
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
						sprintf((char*)string, "   %s%dKHz  ","Frq:  ",f);
						LCD_DisplayStringLine(Line5, (unsigned char *)string);
				    LCD_DisplayStringLine(Line6, (unsigned char *)"                   ");
				    LCD_DisplayStringLine(Line7, (unsigned char *)"                   ");
					 	LCD_DisplayStringLine(Line9,(unsigned char*)"                   2");
	
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
					    LCD_DisplayStringLine(Line9,(unsigned char*)"                   1");
	
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
  
    if(RB3==0)	 //����Ƶ�ʲ���������
   {
   	  Delay_Ms(10);
		 if(RB3==0)
   		  {
		  	if(shezhi==1)
			{
			 	if(f<10)
				{
				 	f=f+1;
					sprintf((char*)string, "   %s%dKHz  ","Frq:  ",f);
					LCD_DisplayStringLine(Line5, (unsigned char *)string);
					IIC_Write(0x11,f);
    			Delay_Ms(10);
				}
				else 
				{ 
					sprintf((char*)string, "   %s%dKHz  ","Frq:  ",f);
					LCD_DisplayStringLine(Line5, (unsigned char *)string);
					IIC_Write(0x11,f);
	    		Delay_Ms(10);
				
				}
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
    LCD_DisplayStringLine(Line9,(unsigned char*)"                   1");


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
		 TIM_TimeBaseStructure.TIM_Period = period;//װ��ֵ 
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
		 TIM_OCInitStructure.TIM_Pulse = CCR2_Val;        //ռ��ʱ��  	   CCR2_Val
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
//	TIM_Cmd(TIM1,ENABLE);
		      /* TIM1 Main Output Enable ʹ��TIM1����������*/
//	TIM_CtrlPWMOutputs(TIM1, ENABLE);  //pwm���ʹ�ܣ�һ��Ҫ�ǵô�
	/*TIM_OC1PreloadConfig(),TIM_ARRPreloadConfig();�������������Ƶ���ccr1��arr��Ԥװ��ʹ�ܣ�
	ʹ�ܺ�ʧ�ܵ�������ǣ�ʹ�ܵ�ʱ���������Ĵ����Ķ�д��Ҫ�ȴ��и����¼�����ʱ���ܱ��ı�
	���������������Ǹ���ʱ�䣩��
	ʧ�ܵ�ʱ�����ֱ�ӽ��ж�д��û���ӳ١�
	 ���������е�����Ҫ�ı�pwm��Ƶ�ʺ�ռ�ձȵ��ã�TIM_SetAutoreload() 
	TIM_SetCompare1()�����������Ϳ����ˡ�*/

}
