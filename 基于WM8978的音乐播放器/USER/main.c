#include "sys.h"
#include "delay.h"  
#include "usart.h"   
#include "led.h"
#include "lcd.h"
#include "key.h"  
#include "usmart.h" 
#include "sram.h"   
#include "malloc.h" 
#include "w25qxx.h"    
#include "sdio_sdcard.h"
#include "ff.h"  
#include "exfuns.h"    
#include "fontupd.h"
#include "text.h"	 
#include "wm8978.h"	 
#include "audioplay.h"	
#include "piclib.h"	 
#include "usbh_usr.h" 
#include "mp3play.h"

  USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core;
//�û�����������
//����ֵ:0,����
//       1,������

u8 USH_User_App(void)
{ 
	u32 total,free;
	u8 res=0;
	LED0=0;LED1=0;
	Show_Str(30,500,200,16,"�豸���ӳɹ�!.",16,0);	 
	res=exf_getfree("2:",&total,&free);
	if(res==0)
	{
		POINT_COLOR=BLUE;//��������Ϊ��ɫ	   
		LCD_ShowString(30,520,200,16,16,"FATFS OK!");	
		LCD_ShowString(30,540,200,16,16,"U Disk Total Size:     MB");	 
		LCD_ShowString(30,560,200,16,16,"U Disk  Free Size:     MB"); 	    
		LCD_ShowNum(174,540,total>>10,5,16); //��ʾU�������� MB
		LCD_ShowNum(174,560,free>>10,5,16);	
	} 
 

	return res;
} 




int main(void)
{        
 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ��LED 
	usmart_dev.init(84);		//��ʼ��USMART
 	LCD_Init();					//LCD��ʼ��  
 	KEY_Init();					//������ʼ��  
	W25QXX_Init();				//��ʼ��W25Q128
	WM8978_Init();				//��ʼ��WM8978
	WM8978_HPvol_Set(40,40);	//������������
	WM8978_SPKvol_Set(50);		//������������
	
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ�� 
	my_mem_init(SRAMCCM);		//��ʼ��CCM�ڴ�� 
	exfuns_init();				//Ϊfatfs��ر��������ڴ�  
   	f_mount(fs[0],"0:",1); 	//����SD��  
  	f_mount(fs[1],"1:",1); 	//����SD��  
  	f_mount(fs[2],"2:",1); 	//����U��
	
	POINT_COLOR=RED;      
	
	while(font_init()) 			//����ֿ�
	{	    
		LCD_ShowString(30,50,200,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(30,50,240,66,WHITE);//�����ʾ	     
		delay_ms(200);				  
	}  	 
	POINT_COLOR=RED;      
 	Show_Str(60,50,200,16,"Explorer STM32F4������",16,0);				    	 
	Show_Str(60,70,200,16,"���ֲ�����",16,0);				    	 
	Show_Str(60,90,200,16,"2017��10��28��",16,0);
	Show_Str(60,110,200,16,"KEY0:NEXT   KEY2:PREV",16,0); 
	Show_Str(60,130,200,16,"KEY_UP:PAUSE/PLAY",16,0);	    	 

	
	//��ʼ��USB����
  	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_Callbacks); 
	
	while(1)
	{ 	
   USBH_Process(&USB_OTG_Core, &USB_Host);
	audio_play();
		
	} 
}







