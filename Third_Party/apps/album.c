#include <string.h>
#include <stdlib.h>
#include "arm_math.h"
#include "fsearch.h"
#include "at45db.h"
#include "album.h"
#include "gdgui.h"
#include "delay.h"
#include "touch.h"
#include "lcd.h"
#include "led.h"
#include "spi.h"

															  
u16* spic_buf;//Ԥ��ͼ����
u8*  pic_name;//ͼƬ�ļ�������
u32* pic_size;//ͼƬ�ļ�������
u16  piccnt;//ͼƬ������
u8   curpic;//��ǰѡ�е�ͼƬ
u8*  linebuf;//��Ƭ�л���
int  basex=0,basea=0;//ͼƬˮƽ�������ת�ǶȵĻ�ֵ
void App_album(void)
{
	u8  XX;
	DIR picdir;
	int xpos,temp;
	u16 i,ypos,times,angle=0;
	float scale=0;

	spic_buf=gui_buf;
	pic_name=namebuf;
	pic_size=sizebuf;
	linebuf=AT45_buffer;
	Dyna_clear(BLACK);
	if(f_opendir(&picdir,"0:/PICTURE"))
	{
		Dialog_box(40,180,"�������Ч��SD��������λϵͳ.");
		delay_ms(1200);
		Del_Dialog();
		Home_page();
	}
	Dialog_box(40,180,"���ڼ������ͼ...");
	piccnt=File_search("0:/PICTURE",T_IMAGE);//�ܻ�������С������,���֧��50���ļ�
	for(i=0;i<piccnt;i++)//��������ͼƬ������ͼ
	{
		Convert_pic(pic_name+i*13);
	}
	Del_Dialog();
	Dyna_clear(BLACK);
	Draw_scroll(40);
	Set_Spic(basex,230);
	Fill_Mpic(pic_name+curpic*13);	
	for(i=0;i<=100;i++)Draw_Mpic(60,53,i);	
	ADS_SPI_Init();
	while(1)//������ѭ��
	{
		if(PEN_int==RESET)
		{
			Read_X(&PENX);
			Read_Y(&PENY);
			xpos=PENX;
			ypos=PENY;
			if(ypos<40)
			{
				Scroll_ctrl(Finish_album);
				ADS_SPI_Init();
			}
			for(times=0;times<500;times++)//��������ʶ��
			{
				Read_X(&PENX);
				Read_Y(&PENY);
				temp=xpos-PENX;
				if(temp>6||temp<-6)break;
				temp=ypos-PENY;
				if(temp>6||temp<-6)break;
				if(PEN_int!=RESET)
				{
					delay_ms(50);
					if(PEN_int!=RESET)break;
				}					
			}
			if(PEN_int!=RESET)//�������
			{				
				if(ypos>230)//ѡ������ͼ
				{					
					temp=curpic;
					if(xpos<60){curpic=-basex/60;XX=0;}
					else if(xpos<120){curpic=1-basex/60;XX=60;}
					else if(xpos<180){curpic=2-basex/60;XX=120;}
					else {curpic=3-basex/60;XX=180;}
					My_chdir("0:/PICTURE/temp");	
					Fill_Spic(pic_name+curpic*13);
					LCD_Fill(XX,230,XX+60,240,BLACK);
					Draw_Spic(XX,240);					
					Fill_Mpic(pic_name+temp*13);
					while(basea>0)//ת��0��
					{
					 	basea/=2;
						Rotation(20,33,basea);
					}					
					for(i=0;i<=100;i++)Draw_Mpic(60,53,100-i);								
					Fill_Mpic(pic_name+curpic*13);	
					for(i=0;i<=100;i++)Draw_Mpic(60,53,i);//Ԥ��ͼƬ
					//f_chdir("0:/PICTURE/temp");
					My_chdir("0:/PICTURE/temp");
					Fill_Spic(pic_name+curpic*13);
					LCD_Fill(XX,310,XX+60,320,BLACK);
					Draw_Spic(XX,230);
				}	  	
			}
			else//��������
			{
				if(times==500)//��������
				{
					if(ypos>113&&ypos<153&&xpos>100&&xpos<140)//�Ŵ�ͼƬ
					{
						//���ز˵�
						Fill_Mpic(pic_name+curpic*13);	
						while(basea>0)//ת��0��
						{
						 	basea/=2;
							Rotation(20,33,basea);
						}
						for(i=1;i<=10;i++)
						{
							Draw_scroll(40-4*i);
							LCD_Fill(0,40-4*i,240,45-4*i,BLACK);
							Set_Spic(basex,230+8*i);
							LCD_Fill(0,221+8*i,240,230+8*i,BLACK);
						}
						Set_Spic(basex,318);
						LCD_Fill(0,309,240,318,BLACK);
						Set_Spic(basex,326);
						LCD_Fill(0,317,240,326,BLACK);
						Fill_Mpic(pic_name+curpic*13);//�����
						while(PEN_int==RESET)
						{	
							Read_X(&PENX);
							Read_Y(&PENY);		
							xpos=abs(PENX-120);
							ypos=abs(PENY-133);
							arm_sqrt_f32(xpos*xpos+ypos*ypos,&scale);
							if(scale>100)scale=100;
							if(scale<0)scale=0;
							Zoom_Mpic(scale+100);
						}
						Zoomback(scale+100);
					}
					else if(ypos>230)//�ƶ�����ͼ
					{
						Fill_Mpic(pic_name+curpic*13);	
						while(basea>0)//ת��0��
						{
						 	basea/=2;
							Rotation(20,33,basea);
						}
						if(xpos<60)curpic=-basex/60;
						else if(xpos<120)curpic=1-basex/60;
						else if(xpos<180)curpic=2-basex/60;
						else curpic=3-basex/60;	
						for(i=0;i<=100;i++)Draw_Mpic(60,53,100-i);//��ȥ							
						Move_Spic(pic_name+curpic*13);
						Fill_Mpic(pic_name+curpic*13);	
						for(i=0;i<=100;i++)Draw_Mpic(60,53,i);//Ԥ��ͼƬ
					}
				}
				else//��������
				{
					if(ypos>230)//��������ͼ
					{			
						basex-=xpos;
						while(PEN_int==RESET)
						{	
							Read_X(&PENX);
							xpos=basex+PENX;
							if(xpos>0)xpos=0;//ֻ�����󻬶�,��ַ������С��0
							if(xpos<(240-60*piccnt))xpos=240-60*piccnt;
							Set_Spic(xpos,230);//����ͼ��ʾ
						}
						basex=Spic_slipback(xpos);//������λ
					}
					else if(ypos>40&&ypos<230&&xpos>180)//��תԤ��ͼ
					{
						Fill_Mpic(pic_name+curpic*13);//�����
						basea-=ypos;
						while(PEN_int==RESET)
						{	
							Read_Y(&PENY);
							ypos=basea+PENY;
							angle=ypos%360;
							Rotation(20,33,angle);
						}
						basea=Mpic_rotaback(angle);				
					}
				}
			}
		}
	}
}

//��ͼƬ�ļ�ת��������ͼ�����ݴ��ļ���
void Convert_pic(u8* filename)
{	
	u8 res;
	FIL file;
	UINT Bytes;	
	u16 i,j,color;
	BITMAPFILEHEADER* bmph;
	u8* readbuf;

	My_chdir("0:/PICTURE/temp");//�л����ݴ��ļ���
	Add_path(path_curr,filename);
	res=f_open(&file,(TCHAR*)path_curr,FA_READ);
	f_close(&file);
	if(!res)return;//�򿪳ɹ���˵�������ļ��Ѵ���,ֱ�ӷ���
	My_chdir("0:/PICTURE");//�л���ͼƬ�ļ���
	Add_path(path_curr,filename);	
	readbuf=work_buf+9600;//�л���
	f_open(&file,(TCHAR*)path_curr,FA_READ);
	f_read(&file,readbuf,54,&Bytes);//��ȡ54�ֽ��ļ�ͷ
	bmph=(BITMAPFILEHEADER*)readbuf;//��ȡBMPͷ
	f_lseek(&file,bmph->bfOffBytes);//ֱ������ͼ������
	for(j=0;j<80;j++)
	{		
		f_read(&file,readbuf,720,&Bytes);//��һ�����ݵ�����
		f_lseek(&file,f_tell(&file)+2160);//����3������
		for(i=0;i<60;i++)
		{
			color=RGB888toRGB565(readbuf[12*i],readbuf[12*i+1],readbuf[12*i+2]);
			spic_buf[60*j+i]=color;
		}
	} 
	f_close(&file);
	//д�ļ�
	My_chdir("0:/PICTURE/temp");//�л����ݴ��ļ���
	Add_path(path_curr,filename);
	f_open(&file,(TCHAR*)path_curr,FA_WRITE|FA_CREATE_ALWAYS);//��������
	f_write(&file,spic_buf,9600,&Bytes);
	f_close(&file);			
}
//�������ͼ
void Fill_Spic(u8* filename)
{
	FIL file;
	UINT Bytes;

	My_chdir("0:/PICTURE/temp");//�л����ݴ��ļ���
	Add_path(path_curr,filename);
	f_open(&file,(TCHAR*)path_curr,FA_READ);
	f_read(&file,spic_buf,9600,&Bytes);
}
//��������ͼ
void Draw_Spic(int x,u16 y)
{
	int temp;
	u8  i,j;

	for(j=0;j<80;j++)
	{
		if(y+j<320)
		{
			for(i=0;i<60;i++)
			{
				temp=x+i;
				if(temp>=0&&temp<240)LCD_SetPoint(temp,y+j,spic_buf[60*j+i]);
			}
		}
	}	
}
//��������ͼλ��
void Set_Spic(int xpos,u16 ypos)
{
	u16 i;

	for(i=0;i<piccnt;i++)//����ͼ��ʾ
	{		
		if(xpos>(-60-60*i)&&xpos<(240-60*i))//����ʾ��Χ�ڲŸ�����ʾ
		{
			Fill_Spic(pic_name+i*13);
			Draw_Spic(xpos+60*i,ypos);
		}					
	}
}
//����ͼ��λ
int Spic_slipback(int xpos)
{
	int temp,offset;

	temp=xpos/60;
	offset=xpos%60;
	if(offset<-30)//����λ
	{
		temp=(temp-1)*60;
		while(xpos>temp)
		{			
			if(xpos>temp+1)xpos-=2;
			else xpos--;
			Set_Spic(xpos,230);
		}
	}
	else//���Ҷ�λ
	{
		temp=temp*60;
		while(xpos<temp)
		{
			if(xpos<temp-1)xpos+=2;
			else xpos++;
			Set_Spic(xpos,230);
		}		
	}
	return xpos;
}
//��Ԥ��ͼ���������
void Fill_Mpic(u8* filename)
{
	FIL file;
	UINT Bytes;	
	u16 i,j,k,color;
	BITMAPFILEHEADER* bmph;
					  
	My_chdir("0:/PICTURE");//�л����ݴ��ļ���
	Add_path(path_curr,filename);
	f_open(&file,(TCHAR*)path_curr,FA_READ);
	f_read(&file,linebuf,54,&Bytes);//��ȡ54�ֽ��ļ�ͷ
	bmph=(BITMAPFILEHEADER*)linebuf;//��ȡBMPͷ
	f_lseek(&file,bmph->bfOffBytes);//ֱ������ͼ������
	for(j=0,k=0;j<160;j++)
	{		
		f_read(&file,linebuf,720,&Bytes);//��һ�����ݵ�����
		f_lseek(&file,f_tell(&file)+720);//����1������
		for(i=0;i<120;i++,k++)
		{
			color=RGB888toRGB565(linebuf[6*i],linebuf[6*i+1],linebuf[6*i+2]);
			spic_buf[k]=color;
		}
	}
	f_close(&file);	
}
//���ƻ����е�Ԥ��ͼ
void Draw_Mpic(u16 x,u16 y,u8 trans)
{
	u16	i,color;
	u16 r=0,g=0,b=0;

	LCD_Window(x,y,x+119,y+159);
	LCD_Cursor(x,y);
	LCD_REG=0x22;
	for(i=0;i<19200;i++)
	{
		color=spic_buf[i];
		//��ɫ��͸����
		r=((color>>11)&0x1F)*trans/100;
		g=((color>>5)&0x3F)*trans/100;
		b=(color&0x1F)*trans/100;
		color=(r<<11)+(g<<5)+b;
		LCD_RAM=color;		
	} 
	LCD_Window(0,0,239,319);
}
//����ԭͼ
void Draw_Lpic(int xpos,u8* filename)
{
	FIL file;
	UINT Bytes;	
	u16 i,j,k,color;
	BITMAPFILEHEADER* bmph;
	int temp;

	My_chdir("0:/PICTURE");//�л���ͼƬ�ļ���
	Add_path(path_curr,filename);
	f_open(&file,(TCHAR*)path_curr,FA_READ);
	f_read(&file,linebuf,54,&Bytes);//��ȡ54�ֽ��ļ�ͷ
	bmph=(BITMAPFILEHEADER*)linebuf;//��ȡBMPͷ
	f_lseek(&file,bmph->bfOffBytes);//ֱ������ͼ������
	for(j=0,k=0;j<320;j++)
	{		
		f_read(&file,linebuf,720,&Bytes);//��һ�����ݵ�����
		for(i=0;i<240;i++,k++)
		{
			temp=xpos+i;
			color=RGB888toRGB565(linebuf[3*i],linebuf[3*i+1],linebuf[3*i+2]);
			if(temp>=0&&temp<240)LCD_SetPoint(temp,j,color);
		}
	}
	f_close(&file);	
}
//120*160ͼƬ��ת����	
void Rotation(u16 x,u16 y,int angle)
{	
	int i,j;
	float sina,cosa,radian;
	u16* srcBuf=spic_buf;//ԭͼ�񻺴�
	int tX,tY,tXN,tYN;			

	radian=(360-angle)*PI/180.0f;//�Ƕȵ�����ת��   
	//ͨ����ͼ������꣬�����Ӧ��ԭͼ�������
	sina=arm_sin_f32(radian);
	cosa=arm_cos_f32(radian);
	for(i=7;i<197;i++)
	{        
		for(j=0;j<200;j++)
		{
			//ת������ͼ��Ϊ���ĵ�����ϵ������������ת
			tX= (j-100)*cosa+(-i+100)*sina;
			tY=-(j-100)*sina+(-i+100)*cosa;
			//���������겻��ԭͼ���ڣ��򲻸�ֵ
			if(tX>=60||tX<-60||tY>80||tY<=-80)
			{
				LCD_SetPoint(j+x,i+y,BLACK);
				continue;
			}
			//��ת����ԭ����ϵ��
			tXN=tX+60; 
			tYN=abs(tY-80);
			LCD_SetPoint(j+x,i+y,srcBuf[tYN*120+tXN]);
		}
	}
}
//�Ƕȶ�λ
int Mpic_rotaback(int angle)
{
	int temp,offset;

	temp=angle/90;
	offset=abs(angle%90);
	if(offset<45)//����λ
	{
		temp=temp*90;
		while(angle>temp)
		{			
			if(angle>temp+1)angle-=2;
			else angle--;
			Rotation(20,33,angle);
		}
	}
	else//���Ҷ�λ
	{
		temp=(temp+1)*90;
		while(angle<temp)
		{
			if(angle<temp-1)angle+=2;
			else angle++;
			Rotation(20,33,angle);
		}		
	}
	return angle;	
}
//ͼƬ����
void Zoom_Mpic(u16 scale)
{
	u16 i,j,refw,refh,temp;
	POINT PLT,PRB;//���Ϻ�������������

	refw=(120*scale/200)*2;//����Ϊż��
	refh=(160*scale/200)*2;
	if(scale<100)scale=100;
	scale=(scale-100)*27/100;//������ͼƬ�Ŵ���������	
	PLT.x=120-refw/2;
	PLT.y=133-refh/2+scale;
	PRB.x=119+refw/2;
	PRB.y=132+refh/2+scale;
	LCD_Window(PLT.x,PLT.y,PRB.x,PRB.y);
	LCD_Cursor(PLT.x,PLT.y);
	LCD_REG=0x22;
	for(j=0;j<refh;j++)
	{
		temp=120*(160*j/refh);
		for(i=0;i<refw;i++)LCD_RAM=spic_buf[temp+(120*i/refw)];//����ʾ
	}
	LCD_Window(0,0,239,319);
	if(PLT.y>0)LCD_Fill(0,0,240,PLT.y,BLACK);
	if(PRB.y<320)LCD_Fill(0,PRB.y+1,240,320,BLACK);
	if(PLT.x>0)LCD_Fill(0,0,PLT.x,320,BLACK);
	if(PRB.x<240)LCD_Fill(PRB.x+1,0,240,320,BLACK);
}
//�ָ�����
void RecallUI(u16 scale)
{
	u8 i,temp;

	temp=scale-85;
	while(temp>0)
	{
		temp=temp*94/100;
		scale=temp+85;
		Zoom_Mpic(scale);
	}
	while(scale<100)
	{
		scale++;
		Zoom_Mpic(scale);
	}
	Set_Spic(basex,318);					
	for(i=1;i<=10;i++)
	{
		Draw_scroll(4*i);
		Set_Spic(basex,320-8*i);
	}
	Set_Spic(basex,230);
	LCD_Fill(0,310,240,320,BLACK);
}
//���ſ���
void Zoomback(u16 scale)
{
	if(scale<150)RecallUI(scale);
	else
	{
		while(scale<200)
		{
		 	scale++;
		 	Zoom_Mpic(scale);		
		}
		Draw_Lpic(0,pic_name+curpic*13);
		while(PEN_int!=RESET);
		RecallUI(scale);			
	}
}
//����ͼ�ƶ�
void Move_Spic(u8* filename)
{
	u8 which;

	OX=240;//���ɲ����ܵ�ֵ
	OY=320;
	which=0;
	bufwidth=60;
    bufheight=80;
	FLASH_SPI_Init();
	Draw_icon(90,103,19,BLACK);
	My_chdir("0:/PICTURE/temp");//�л����ݴ��ļ���
	Fill_Spic(filename);//��Ҫ�ƶ�������ͼ����
	oldbuf=gui_buf+4800;
	newbuf=gui_buf+9600;		
	while(PEN_int==RESET)
	{		
		ADS_SPI_Init();
		Read_X(&PENX);
		Read_Y(&PENY);
		NX=PENX-30;
		NY=PENY-30;
		if(NX<0)NX=0;
		if(NY<0)NY=0;
		if(NX>180)NX=180;
		if(NY>240)NY=240;
		Fill_buff();//�ڻ���ͼ��֮ǰ���������
		Draw_menbuf(NX,NY,gui_buf,1);
		Draw_buff();//��س���ǰͼ����������ı���
		FLASH_SPI_Init();
		if(NX>60&&NX<120&&NY>73&&NY<133)Draw_icon(90,103,18,BLACK);
		else Draw_icon(90,103,19,BLACK);		
		OX=NX;
		OY=NY;
 		if(which)
		{
			oldbuf=gui_buf+4800;
			newbuf=gui_buf+9600;
			which=0;			
		}
		else 
		{
			oldbuf=gui_buf+9600;
			newbuf=gui_buf+4800;//�л�������
			which=1;			
		}
	}
	if(NX>60&&NX<120&&NY>73&&NY<133)
	{
		delay_ms(400);
		LCD_Fill(60,73,181,214,BLACK);
		Dialog_box(40,180,"���ڸ��±���ͼƬ�����Ե�...");
		My_chdir("0:/PICTURE");
		Save_pic(pic_name+curpic*13,BGD_addr,BGD_size);
		Del_Dialog();
	}
	else
	{	
		NX=240;//���ɲ����ܵ�ֵ
		NY=320;
		Draw_buff();
	}
	bufwidth=240;
    bufheight=80;
	ADS_SPI_Init();
}
//�������
void Finish_album(void)
{

}













