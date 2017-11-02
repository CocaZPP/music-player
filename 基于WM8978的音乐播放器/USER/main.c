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
//用户测试主程序
//返回值:0,正常
//       1,有问题

u8 USH_User_App(void)
{ 
	u32 total,free;
	u8 res=0;
	LED0=0;LED1=0;
	Show_Str(30,500,200,16,"设备连接成功!.",16,0);	 
	res=exf_getfree("2:",&total,&free);
	if(res==0)
	{
		POINT_COLOR=BLUE;//设置字体为蓝色	   
		LCD_ShowString(30,520,200,16,16,"FATFS OK!");	
		LCD_ShowString(30,540,200,16,16,"U Disk Total Size:     MB");	 
		LCD_ShowString(30,560,200,16,16,"U Disk  Free Size:     MB"); 	    
		LCD_ShowNum(174,540,total>>10,5,16); //显示U盘总容量 MB
		LCD_ShowNum(174,560,free>>10,5,16);	
	} 
 

	return res;
} 




int main(void)
{        
 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	LED_Init();					//初始化LED 
	usmart_dev.init(84);		//初始化USMART
 	LCD_Init();					//LCD初始化  
 	KEY_Init();					//按键初始化  
	W25QXX_Init();				//初始化W25Q128
	WM8978_Init();				//初始化WM8978
	WM8978_HPvol_Set(40,40);	//耳机音量设置
	WM8978_SPKvol_Set(50);		//喇叭音量设置
	
	my_mem_init(SRAMIN);		//初始化内部内存池 
	my_mem_init(SRAMCCM);		//初始化CCM内存池 
	exfuns_init();				//为fatfs相关变量申请内存  
   	f_mount(fs[0],"0:",1); 	//挂载SD卡  
  	f_mount(fs[1],"1:",1); 	//挂载SD卡  
  	f_mount(fs[2],"2:",1); 	//挂载U盘
	
	POINT_COLOR=RED;      
	
	while(font_init()) 			//检查字库
	{	    
		LCD_ShowString(30,50,200,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(30,50,240,66,WHITE);//清除显示	     
		delay_ms(200);				  
	}  	 
	POINT_COLOR=RED;      
 	Show_Str(60,50,200,16,"Explorer STM32F4开发板",16,0);				    	 
	Show_Str(60,70,200,16,"音乐播放器",16,0);				    	 
	Show_Str(60,90,200,16,"2017年10月28日",16,0);
	Show_Str(60,110,200,16,"KEY0:NEXT   KEY2:PREV",16,0); 
	Show_Str(60,130,200,16,"KEY_UP:PAUSE/PLAY",16,0);	    	 

	
	//初始化USB主机
  	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_Callbacks); 
	
	while(1)
	{ 	
   USBH_Process(&USB_OTG_Core, &USB_Host);
	audio_play();
		
	} 
}







