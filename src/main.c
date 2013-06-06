#include "main.h"
#include <stdlib.h>

void TEST_Char(u16 x,u16 y,u8 num,u16 color);
int main(void)
{         		  
	u8 	  res;
	FATFS fatfs;
	FIL   file;
	u8*   gifbuf;
	UINT  bytes;

	Key_Init();//������ʼ��		 
	LED_Init();//LED�Ƴ�ʼ��
	LCD_Init();//��ʾ����ʼ��
	COM1Init(115200);
	MyRTC_Init();//RTCʱ�ӳ�ʼ��
	Touch_Init();//������ʼ��			
	SPI_Flash_Init();//����FLASH���ڴ洢ϵͳͼƬ���ֿ⼰�ؼ�����
	Load_param();//��������ϵͳ������������.
	PWM_Init();//PWM���ڿ�����Ļ��������
	TIM2_Init(8400,5000);//LED��˸
	mem_init();//��̬�ڴ�����ʼ��
	Set_Keyint(KEY4);//�����ж����ڽ���
  	f_mount(0,&fatfs);//��ʼ������	
  	res=f_open(&file,"0:/GIF/nokia.gif",FA_READ);
	if(!res)
	{
		gifbuf=malloc(95*1024);
		f_read(&file,gifbuf,94018,&bytes);
		ShowGif(gifbuf,94018,0,0,80);//��������
		free(gifbuf);
		delay_ms(500);
	}					
	GDGUI_start();
}

