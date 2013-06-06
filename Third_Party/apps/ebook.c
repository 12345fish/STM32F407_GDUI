#include <string.h>
#include <stdlib.h>
//#include "arm_math.h"
//#include "MemPool.h"
#include "at45db.h"
#include "fsearch.h"
#include "ebook.h"  
#include "gdgui.h"
#include "delay.h"
#include "touch.h"
#include "lcd.h"
#include "spi.h"


ebkdef* ebk;//������ṹ��
u8*  	ebk_buf1;
u8*  	ebk_buf2;
u8*  	ebk_buf3;//������,���Դ�ŵ�ǰҳ��ǰһҳ����һҳ����ʾ����
u32*	ebk_ptr;//�������ÿһҳ��ͷ���ļ��е�λ��
void App_ebook(void)
{
	u8  i;
	u16 times;
	int xpos,ypos;		
	//Ӧ�ó��������õ����ڴ�ռ�����ö�̬���䷽ʽ
	ebk=malloc(sizeof(ebkdef));
	if(ebk==NULL){Finish_ebook();return;}
	ebk_buf1=malloc(EBKPGSize*sizeof(u8));
	if(ebk_buf1==NULL){Finish_ebook();return;}
	ebk_buf2=malloc(EBKPGSize*sizeof(u8));
	if(ebk_buf2==NULL){Finish_ebook();return;}
	ebk_buf3=malloc(EBKPGSize*sizeof(u8));
	if(ebk_buf3==NULL){Finish_ebook();return;}	


	ebk->basex=0;
	ebk->basey=0;
	ebk->filecur=0;
	ebk->pagecur=0;		   
	LCD_Fill(0,40,240,320,GRAY[29]);
	LCD_Charmd(1,GRAY[29]);
	My_chdir("0:/EBOOK");//�л����������ļ���
	if(f_opendir(&ebk->dir,"0:/EBOOK"))
	{
		Dialog_box(40,180,"�������Ч��SD��������λϵͳ.");
		delay_ms(1200);
		Del_Dialog();
		Finish_ebook();
		Home_page();
	}
	ebk->filecnt=File_search("0:/EBOOK",T_BOOK);//�ܻ�������С������,���֧��50���ļ�	
	Add_path(path_curr,namebuf+ebk->filecur*13);
	f_open(&ebk->file,(TCHAR*)path_curr,FA_READ);
	ebk_ptr=malloc(sizebuf[ebk->filecur]/EBKRDSize*sizeof(u32));
	if(ebk_ptr==NULL){Finish_ebook();return;}	
	ebk_ptr[0]=0;
	Fill_paper(ebk_buf2);
	ebk_ptr[1]=f_tell(&ebk->file);
	Fill_paper(ebk_buf3);
	ebk_ptr[2]=f_tell(&ebk->file);
	for(i=0;i<11;i++)LCD_HLine(8,69+i*24,232,GRAY[20]);
	Slip_paper(0);//��ʾ��һҳ	
	ADS_SPI_Init();	
	while(1)
	{
		if(PEN_int==RESET)
		{
	  		Read_X(&PENX);
			Read_Y(&PENY);
			xpos=PENX;
			ypos=PENY;
			if(ypos<40)
			{
				LCD_Charmd(1,WHITE);
				Scroll_ctrl(Finish_ebook);
				ADS_SPI_Init();
				LCD_Charmd(1,GRAY[29]);
			}
			for(times=0;times<600;times++)//��������ʶ��
			{
				Read_X(&PENX);
				Read_Y(&PENY);	
				if(abs(xpos-PENX)>8)break;
				if(abs(ypos-PENY)>8)break;
				if(PEN_int!=RESET)
				{
					delay_ms(50);
					if(PEN_int!=RESET)break;
				}					
			}
			if(PEN_int!=RESET)//�������
			{				
											
			}
			else//��������
			{
				if(times==600){}//��������
				else//��������
				{
				 	if(ypos>40)
					{
						ebk->basex-=xpos;
						while(PEN_int==RESET)
						{						
							Read_X(&PENX);
							xpos=ebk->basex+PENX;
							if(ebk->pagecur==0&&xpos>0)xpos=0;
							Slip_paper(xpos);
						}
						Stick_paper(xpos);
						ebk->basex=0;						
					}
				}
			}
		}	
	}	
}
//��������,direction:���ƻ�����
void Change_buf(u8 direction)
{
	u8* tempbuf;

	if(direction)//����,����һҳ
	{		
		ebk->pagecur++;
		tempbuf=ebk_buf1;
		ebk_buf1=ebk_buf2;
		ebk_buf2=ebk_buf3;
		ebk_buf3=tempbuf;
		Fill_paper(ebk_buf3);
		ebk_ptr[ebk->pagecur+2]=f_tell(&ebk->file);//���浱ǰ�ļ�������ָ��			
	}
	else//����,����һҳ
	{
		ebk->pagecur--;
		if(ebk->pagecur>0)
		{
			f_lseek(&ebk->file,ebk_ptr[ebk->pagecur-1]);
			tempbuf=ebk_buf3;
			ebk_buf3=ebk_buf2;
			ebk_buf2=ebk_buf1;		
			ebk_buf1=tempbuf;
			Fill_paper(ebk_buf1);
			f_lseek(&ebk->file,ebk_ptr[ebk->pagecur+2]);
		}
		else//��һҳ
		{
			tempbuf=ebk_buf3;
			ebk_buf3=ebk_buf2;
			ebk_buf2=ebk_buf1;		
			ebk_buf1=tempbuf;
			f_lseek(&ebk->file,ebk_ptr[ebk->pagecur+2]);		
		}					
	}
}
//�򻺳����һҳֽ����ģ,һҳ��154������(�����ַ�����һ������,�������ַ������ո�)
void Fill_paper(u8* buf)
{
 	u16 i,k,count;
	u8* Fonts;
	u32 offset;

	count=0;
	Fonts=ebk->page_buf;	
 	f_read(&ebk->file,ebk->page_buf,EBKRDSize,&ebk->bytes);//��ȡһҳֽ������	
	if(ebk->bytes<EBKRDSize)
	{
		if(!ebk->bytes)
		{
			Dialog_box(40,180,"�������һҳ!");
			delay_ms(1200);
			Del_Dialog();
			return;			
		}
		for(i=ebk->bytes;i<EBKRDSize;i++)ebk->page_buf[i]=' ';//��ʣ��λ����ɿո�	
	}
	for(i=0;i<154;i++)//ѭ��һ�����������ֽ�
	{			
		if(*Fonts<0x81)//��ASCII�ַ�,�������ַ�������ȡģ�ķ�ʽ���Ƶ�������
		{
			if(*Fonts<0x20)*Fonts=0x20;//�����ַ������ո���ʾ
			for(k=0;k<8;k++)buf[k]=asc2_1608[*Fonts-' '][k];
			for(k=16;k<24;k++)buf[k]=asc2_1608[*Fonts-' '][k-8];
			*Fonts++;			
			if(*Fonts<0x81)//���������ַ�
			{
				if(*Fonts<0x20)*Fonts=0x20;//�����ַ������ո���ʾ
				for(k=8;k<16;k++)buf[k]=asc2_1608[*Fonts-' '][k-8];
				for(k=24;k<32;k++)buf[k]=asc2_1608[*Fonts-' '][k-16];
				*Fonts++;//ƫ�Ƶ�ַ��1									
			}
			else//����ַ�,���ո� 
			{
				for(k=8;k<16;k++)buf[k]=asc2_1608[0][k-8];
				for(k=24;k<32;k++)buf[k]=asc2_1608[0][k-16];
				count++;//�����Ŀո����,�Խ��ж�ָ���ƫ��				
			}
			buf+=32;
		}			
		else//�Ǻ��� 
		{	
			offset=Get_FontAdr(Fonts);
			SPI_Flash_Read(buf,FON_addr*528+offset,32);	
			buf+=32;					
			*Fonts++;//ƫ�Ƶ�ַ��2
			*Fonts++;
		}
	}	  	
	f_lseek(&ebk->file,f_tell(&ebk->file)-count);//ָ��ǰ��,���������ո��������ȱʧ
}
//��ʾ��num�е�����
void Show_Vfonts(int xpos,u8 num,u8* Fonts)
{
	u8  i;

	for(i=0;i<11;i++)
	{
		Show_Hanzi(xpos,50+i*24,Fonts+i*448+num*32,GRAY[3]);
	}
}

//��ҳ
void Slip_paper(int xpos)
{
	u8  i,j;
	int temp;

	for(i=0;i<3;i++)
	{
		temp=xpos+i*240-240;
		if(temp>-8&&temp<240)for(j=0;j<11;j++)LCD_Char(temp,50+j*24,' ',GRAY[3]);
	}
	for(i=0;i<14;i++)
	{					  
		temp=i*16-232+xpos;
		if(temp>-16&&temp<240)Show_Vfonts(temp,i,ebk_buf1);
		temp=i*16+8+xpos;
		if(temp>-16&&temp<240)Show_Vfonts(temp,i,ebk_buf2);
		temp=i*16+248+xpos;
		if(temp>-16&&temp<240)Show_Vfonts(temp,i,ebk_buf3);
	}
	for(i=0;i<3;i++)
	{
		temp=xpos+i*240-8;
		if(temp>-8&&temp<240)
		for(j=0;j<11;j++)LCD_Char(temp,50+j*24,' ',GRAY[6]);	
	}
}
//ֽ�Ŷ�λ����
int Stick_paper(int xpos)
{
	u16 temp;

	if(xpos>=120&&xpos<=240)
	{
		temp=240-xpos;
		while(xpos<240)
		{			
			temp/=2;
			xpos+=temp+1;
			Slip_paper(xpos);				
		}
		Change_buf(0);//��������
	}
	else if(xpos<120&&xpos>0)
	{	
		while(xpos>0)
		{				
			xpos/=2;
			Slip_paper(xpos);				
		}
	}
	else if(xpos<0&&xpos>-120)
	{
		while(xpos<0)
		{			
			xpos/=2;
			Slip_paper(xpos);				
		}
	}
	else if(xpos<=-120&&xpos>=-240)
	{
		temp=240+xpos;
		while(xpos>-240)
		{	
			temp/=2;	
			xpos-=temp+1;
			Slip_paper(xpos);				
		}
		Change_buf(1);//��������
	}
	return xpos;
}
//��������
void Finish_ebook(void)
{
	f_close(&ebk->file);
	//�ͷ��ڴ�
	free(ebk);				   
	free(ebk_buf1);
	free(ebk_buf2);
	free(ebk_buf3);
	free(ebk_ptr);		
}




