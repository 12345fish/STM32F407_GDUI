#include <string.h>
#include "arm_math.h"
#include "stdlib.h"
#include "at45db.h"
#include "fsearch.h"
#include "apps.h"
#include "gdgui.h"
#include "delay.h"
#include "touch.h"
#include "lcd.h"
#include "spi.h"

u16  file_cnt;
u8*  file_name;
u32* file_size;
u8*  file_trib;
u16	 file_curr;


void App_fmanager(void)
{
	DIR filedir;
	int temp,xpos,ypos,basey,i;
	u16 times;

	file_name=namebuf;
	file_size=sizebuf;
	file_trib=tribbuf;
	My_chdir("0:");
	Load_bground();
	Draw_scroll(40);
fmstart:
	basey=0;
	if(f_opendir(&filedir,(TCHAR*)path_curr))
	{
		Dialog_box(40,180,"�������Ч��SD��������λϵͳ.");
		delay_ms(1200);
		Del_Dialog();
		Home_page();
	}
	file_cnt=File_search(path_curr,T_ANY);//�ܻ�������С������,���֧��50���ļ�
	FLASH_SPI_Init();
	i=300;
	LCD_Charmd(0,WHITE);
	while(i>0)
	{
		i/=2;
		File_list(i,basey);//��ʾ�б�		
	}		
	Show_path(path_curr);//��ʾ��ǰĿ¼	
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
				if(Scroll_ctrl(Finish_fmanager)&&path_curr[2])//�����ϼ�
				{
					FLASH_SPI_Init();
					Cut_path(path_curr);
					Show_path(path_curr);
					goto fmstart;
				}
				ADS_SPI_Init();
			}
			for(times=0;times<1000;times++)//��������ʶ��
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
				if(ypos>70&&ypos<310&&ypos<(74+file_cnt*26))
				{
					for(i=0;i<9&&i<file_cnt;i++)//һ����ʾ9���ļ�
					{
						if(ypos>(74+i*26)&&ypos<(100+i*26))
						{
							Trans_SFill(0,74+i*26,240,100+i*26,WHITE,100,0);
							file_curr=i-basey/26;
						}
					}
					FLASH_SPI_Init();
					switch(Get_type(file_name+file_curr*13,file_trib[file_curr]))
					{
					 	case T_ANY:	Dialog_box(40,180,"�ļ���ʽ��֧��.");
									delay_ms(1200);
									Del_Dialog();
									break;
						case T_DIR:	Add_path(path_curr,file_name+file_curr*13);//��Ŀ¼									
									Show_path(path_curr);
									goto fmstart;	
						case T_AUDIO:appcur=11;
									App_music();
									break;
						case T_IMAGE:appcur=8;
									App_album();
									break;
						case T_VIDO:break;
						case T_BOOK:break;
						case T_GAME:break;
						case T_SYS: Dialog_box(40,180,"ϵͳ�ļ������ɲ���.");
									delay_ms(1200);
									Del_Dialog();
									break;
						default:break;
					}
					delay_ms(300);
					File_list(0,basey);
					ADS_SPI_Init();				
				}									
			}
			else//��������
			{
				if(times==1000)//��������
				{
					if(ypos>70&&ypos<310&&ypos<(74+file_cnt*26))
					{
						for(i=0;i<9&&i<file_cnt;i++)//һ����ʾ9���ļ�
						{
							if(ypos>(74+i*26)&&ypos<(100+i*26))file_curr=i-basey/26;
						}				 	
						Move_file();
					}
				}
				else//��������
				{
					if(ypos>70&&ypos<300)
					{
						if(xpos>40&&xpos<180)//�����б�
						{
							basey-=ypos;
							while(PEN_int==RESET)
							{	
								ADS_SPI_Init();														
								Read_Y(&PENY);
								ypos=basey+PENY;
								if(file_cnt<10||ypos>0)ypos=0;//ֻ�����ϻ���,��ַ������С��0
								else if(ypos<(230-26*file_cnt))ypos=230-26*file_cnt;
								FLASH_SPI_Init();
								File_list(0,ypos);//��ʾ�б�							
							}
							basey=Stick_Flist(ypos);
							ADS_SPI_Init();
						}
						else if(xpos<40)
						{
							Select_file();
						}
					}
				}
			}
		}	
	}
}
//��ʾ�ļ��б�
void File_list(int xpos,int ypos)
{
	u16 i;
	int temp;

	bufheight=80;
	Fill_back(70,150);//����һ�α���
	for(i=0;i<file_cnt;i++)//�ļ��б���ʾ
	{		
		if(ypos>(-26-26*i)&&ypos<(80-26*i))//����ʾ��Χ�ڲŸ�����ʾ
		{
			temp=ypos+i*26+8;//temp=ypos+i*26+78-70; 78Ϊ��һ���ļ���ʾ��λ��
			Load_type(xpos+20,temp-4,Get_type(file_name+i*13,file_trib[i]));//��ʾ�ļ�����ͼ��
			Win_Fonts(xpos+50,temp,file_name+i*13,WHITE);//��ʾ�ļ���			
		}					
	}
	Dip_guibuf(70,150);//��ʾ��һ������
	Fill_back(150,230);//���ڶ��α���		   
	for(i=0;i<file_cnt;i++)//�ļ��б���ʾ
	{		
		if(ypos>(54-26*i)&&ypos<(160-26*i))//����ʾ��Χ�ڲŸ�����ʾ
		{
			temp=ypos+i*26-72;//temp=ypos+i*26+78-150;
			Load_type(xpos+20,temp-4,Get_type(file_name+i*13,file_trib[i]));//��ʾ�ļ�����ͼ��
			Win_Fonts(xpos+50,temp,file_name+i*13,WHITE);//��ʾ�ļ���			
		}					
	}
	Dip_guibuf(150,230);//��ʾ�ڶ�������
	Fill_back(230,310);//�������α���		   
	for(i=0;i<file_cnt;i++)//�ļ��б���ʾ
	{		
		if(ypos>(134-26*i)&&ypos<(240-26*i))//����ʾ��Χ�ڲŸ�����ʾ
		{
			temp=ypos+i*26-152;//temp=ypos+i*26+78-230;
			Load_type(xpos+20,temp-4,Get_type(file_name+i*13,file_trib[i]));//��ʾ�ļ�����ͼ��
			Win_Fonts(xpos+50,temp,file_name+i*13,WHITE);//��ʾ�ļ���			
		}					
	}
	Dip_guibuf(230,310);//��ʾ����������
} 
//�б�λ
int Stick_Flist(int ypos)
{
	int temp,offset;

	temp=ypos/26;
	offset=ypos%26;
	if(offset<-13)//���϶�λ
	{
		temp=(temp-1)*26;
		while(ypos>temp)
		{			
			if(ypos>temp+1)ypos-=2;
			else ypos--;
			File_list(0,ypos);
		}
	}
	else//���¶�λ
	{
		temp=temp*26;
		while(ypos<temp)
		{
			if(ypos<temp-1)ypos+=2;
			else ypos++;
			File_list(0,ypos);
		}		
	}
	return ypos;
}
//��ʾ��ǰ·��
void Show_path(u8* path)
{
	u8 i;

	LCD_Charmd(0,WHITE);
	for(i=45;i<70;i++)Draw_BGDline(i,1);
	Show_Fonts(23,48,path,WHITE);
}
//�ƶ��ļ�
void Move_file(void)
{
	u8 which;

	OX=240;//���ɲ����ܵ�ֵ
	OY=320;
	which=0;
	FLASH_SPI_Init();
	bufwidth=135;
	bufheight=24;
	memset(gui_buf,0,7200);
	Load_type(0,0,Get_type(file_name+file_curr*13,file_trib[file_curr]));//��ʾ�ļ�����ͼ��
	Win_Fonts(30,4,file_name+file_curr*13,WHITE);//��ʾ�ļ���	
	oldbuf=gui_buf+3600;
	newbuf=gui_buf+7200;		
	while(PEN_int==RESET)
	{		
		ADS_SPI_Init();
		Read_X(&PENX);
		Read_Y(&PENY);
		NX=PENX-13;
		NY=PENY-13;
		if(NX<0)NX=0;
		if(NY<0)NY=0;
		if(NX>215)NX=215;
		if(NY>295)NY=295;
		Fill_buff();//�ڻ���ͼ��֮ǰ���������
		Draw_menbuf(NX,NY,gui_buf,0);
		Draw_buff();//��س���ǰͼ����������ı���
		FLASH_SPI_Init();
		//if(NX>60&&NX<120&&NY>73&&NY<133)Draw_icon(90,103,18,BLACK);
		//else Draw_icon(90,103,19,BLACK);		
		OX=NX;
		OY=NY;
 		if(which)
		{
			oldbuf=gui_buf+3600;
			newbuf=gui_buf+7200;
			which=0;			
		}
		else 
		{
			oldbuf=gui_buf+7200;
			newbuf=gui_buf+3600;//�л�������
			which=1;			
		}
	}
//	if(NX>60&&NX<120&&NY>73&&NY<133)
//	{
//
//	}
//	else
//	{	
		NX=240;//���ɲ����ܵ�ֵ
		NY=320;
		Draw_buff();
//	}
	bufwidth=240;
	ADS_SPI_Init();
}
//ѡ���ļ����ļ���
void Select_file(void)
{
	while(PEN_int==RESET)
	{
		Read_Y(&PENY);
		Read_X(&PENX);
		if(PENY<70)PENY=70;
		if(PENY>305)PENY=305;
		Draw_Bigpiont(PENX,PENY,RED);
	}
}
















//�����ļ�������
void Finish_fmanager(void)
{

}








