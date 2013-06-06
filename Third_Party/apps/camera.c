#include <stdlib.h>
#include "ov7670.h"
#include "fsearch.h"
#include "camera.h"  
#include "gdgui.h"
#include "delay.h"
#include "touch.h"
#include "lcd.h"
#include "spi.h"

camdef* cam;
void App_camera(void)
{
	cam=malloc(sizeof(camdef));
	if(cam==NULL){Finish_camera();return;}
	if(OV7670_Init())
	{
		Dialog_box(40,180,"OV7670��ʼ��ʧ��.");
		delay_ms(1200);
		Del_Dialog();
		Finish_camera();
		Home_page();
	}
	if(f_opendir(&cam->dir,"0:/DCMI"))
	{
		Dialog_box(40,180,"�������Ч��SD��������λϵͳ.");
		delay_ms(1200);
		Del_Dialog();
		Finish_camera();
		Home_page();
	}
	Cam_Start();
  	while(1);
}

void Finish_camera(void)
{
	f_close(&cam->file);
	//�ͷ��ڴ�
	free(cam);
}



