#ifndef __MP3PLAY_H__ 
#define __MP3PLAY_H__ 
#include <sys.h>
#include <mp3dec.h>
#include "ff.h"	
//////////////////////////////////////////////////////////////////////////////////	 
//��������ֲ��helix MP3�����
//ALIENTEK STM32F407������
//MP3 �������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/6/29
//�汾��V1.0
//********************************************************************************
//V1.0 ˵��
//1,֧��16λ������/������MP3�Ľ���
//2,֧��CBR/VBR��ʽMP3����
//3,֧��ID3V1��ID3V2��ǩ����
//4,֧�����б�����(MP3�����320Kbps)����
////////////////////////////////////////////////////////////////////////////////// 	
  
#define MP3_TITSIZE_MAX		40		//����������󳤶�
#define MP3_ARTSIZE_MAX		40		//����������󳤶�
#define MP3_FILE_BUF_SZ    5*1024	//MP3����ʱ,�ļ�buf��С

//���ֲ��Ų����������
typedef enum {
	AP_OK=0X00,				//�����������
	AP_NEXT,				//������һ��
	AP_PREV,				//������һ��
	AP_ERR=0X80,			//�����д���(û����������,������ʾ����) 
}APRESULT;

//���ֲ��ſ�����
//typedef __packed struct
//{  
//	//2��I2S�����BUF
//	u8 *i2sbuf1;
//	u8 *i2sbuf2; 
//	u8 *tbuf;			//��ʱ����
//	FIL *file;			//��Ƶ�ļ�ָ�� 	
//	u32(*file_seek)(u32);//�ļ�������˺��� 

//	vu8 status;			//bit0:0,��ͣ����;1,��������
//						//bit1:0,��������;1,��������  
//						//bit2~3:����
//						//bit4:0,�����ֲ���;1,���ֲ����� (������)		
//						//bit5:0,�޶���;1,ִ����һ���и����(������)
//						//bit6:0,�޶���;1,������ֹ����(���ǲ�ɾ����Ƶ��������),������ɺ�,���������Զ������λ
// 						//bit7:0,��Ƶ����������ɾ��/����ɾ��;1,��Ƶ����������������(�������ִ��)
//	
//	u8 mode;			//����ģʽ
//						//0,ȫ��ѭ��;1,����ѭ��;2,�������;
//	
//	u8 *path;			//��ǰ�ļ���·��
//	u8 *name;			//��ǰ���ŵ�MP3��������
//	u16 namelen;		//name��ռ�ĵ���.
//	u16 curnamepos;		//��ǰ��ƫ��

//    u32 totsec ;		//���׸�ʱ��,��λ:��
//    u32 cursec ;		//��ǰ����ʱ�� 
//    u32 bitrate;	   	//������(λ��)
//	u32 samplerate;		//������ 
//	u16 bps;			//λ��,����16bit,24bit,32bit
//	
//	u16 curindex;		//��ǰ���ŵ���Ƶ�ļ�����
//	u16 mfilenum;		//�����ļ���Ŀ	    
//	u16 *mfindextbl;	//��Ƶ�ļ�������
//	
//}__audiodev; 
//extern __audiodev audiodev;	//���ֲ��ſ�����

//ȡ2��ֵ����Ľ�Сֵ.
#ifndef AUDIO_MIN			
#define AUDIO_MIN(x,y)	((x)<(y)? (x):(y))
#endif



//ID3V1 ��ǩ 
typedef __packed struct 
{
    u8 id[3];		   	//ID,TAG������ĸ
    u8 title[30];		//��������
    u8 artist[30];		//����������
	u8 year[4];			//���
	u8 comment[30];		//��ע
	u8 genre;			//���� 
}ID3V1_Tag;

//ID3V2 ��ǩͷ 
typedef __packed struct 
{
    u8 id[3];		   	//ID
    u8 mversion;		//���汾��
    u8 sversion;		//�Ӱ汾��
    u8 flags;			//��ǩͷ��־
    u8 size[4];			//��ǩ��Ϣ��С(��������ǩͷ10�ֽ�).����,��ǩ��С=size+10.
}ID3V2_TagHead;

//ID3V2.3 �汾֡ͷ
typedef __packed struct 
{
    u8 id[4];		   	//֡ID
    u8 size[4];			//֡��С
    u16 flags;			//֡��־
}ID3V23_FrameHead;

//MP3 Xing֡��Ϣ(û��ȫ���г���,���г����õĲ���)
typedef __packed struct 
{
    u8 id[4];		   	//֡ID,ΪXing/Info
    u8 flags[4];		//��ű�־
    u8 frames[4];		//��֡��
	u8 fsize[4];		//�ļ��ܴ�С(������ID3)
}MP3_FrameXing;
 
//MP3 VBRI֡��Ϣ(û��ȫ���г���,���г����õĲ���)
typedef __packed struct 
{
    u8 id[4];		   	//֡ID,ΪXing/Info
	u8 version[2];		//�汾��
	u8 delay[2];		//�ӳ�
	u8 quality[2];		//��Ƶ����,0~100,Խ������Խ��
	u8 fsize[4];		//�ļ��ܴ�С
	u8 frames[4];		//�ļ���֡�� 
}MP3_FrameVBRI;


//MP3���ƽṹ��
typedef __packed struct 
{
    u8 title[MP3_TITSIZE_MAX];	//��������
    u8 artist[MP3_ARTSIZE_MAX];	//����������
    u32 totsec ;				//���׸�ʱ��,��λ:��
    u32 cursec ;				//��ǰ����ʱ��
	
    u32 bitrate;	   			//������
	u32 samplerate;				//������
	u16 outsamples;				//PCM�����������С(��16λΪ��λ),������MP3,�����ʵ�����*2(����DAC���)
	
	u32 datastart;				//����֡��ʼ��λ��(���ļ������ƫ��)
}__mp3ctrl;

extern __mp3ctrl * mp3ctrl;



void mp3_i2s_dma_tx_callback(void) ;
void mp3_fill_buffer(u16* buf,u16 size,u8 nch);
u8 mp3_id3v1_decode(u8* buf,__mp3ctrl *pctrl);
u8 mp3_id3v2_decode(u8* buf,u32 size,__mp3ctrl *pctrl);
u8 mp3_get_info(u8 *pname,__mp3ctrl* pctrl);
u8 mp3_play_song(u8* fname);
#endif




























