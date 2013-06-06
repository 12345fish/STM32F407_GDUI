#include "gdgui.h"
#include "at45db.h"
#include "MemPool.h"
#include "delay.h"
#include "touch.h"
#include "lcd.h"
#include "spi.h"
#include "rtc.h"
#include "key.h"
#include "apps.h"
#include "fsearch.h"
#include <stdlib.h>

u8*  work_buf;//�洢�豸
u16* gui_buf;
int  basex1,basex2;
u8   screen,eicocur,scrpos,appcur;
u8   Sysparam[528];//һҳ��ϵͳ�����ݴ�
u8*  icolist=Sysparam;
u8   GBK_Buf[32];
//�����ֿ�
void UpdateFont(u8 *path)
{
	u8 	 res;
	FIL  fileR;
	UINT Bytes;
	u16  page;

	res=f_open(&fileR,(TCHAR*)path,FA_READ);
	if(res)return;
	LCD_Charmd(1,WHITE);
	for(page=0;page<FON_size;page++)
	{
		f_read(&fileR,AT45_buffer,528,&Bytes);
		FlashPageWrite(FON_addr+page,&AT45_buffer[0]);//д��SPI FLASH
		LCD_Num(100,150,page*100/FON_size,3,RED);//��ʾд��ٷֱ�
	} 
	f_close(&fileR);			
}
//��ȡ���ֵ�ַ
u32 Get_FontAdr(u8* ch)
{
	u32 temp1,temp2; 
   
    temp1=*ch; 
    temp2=*(ch+1); 
    if(temp1<0x81||temp2<0x40)return 0;//���Ϸ��ĺ��� 
    temp1-=0x81;	//��ȥ������ʼ��											
    temp2-=0x40;	//λ�� 
    temp1*=192;		
    temp1+=temp2;	//�õ�ƫ�� 			
	temp2=((temp1/16)<<9)+((temp1%16)<<5);//�õ����ֿ��ļ��е�ƫ�Ƶ�ַ

	return temp2;	
}
//��ʾһ������,ȡģ��ʽΪ����,�ֽڵ���
void Show_Hanzi(int x0,u16 y0,u8 *word,u16 color)
{
	u8  m,n;
	u16 temp;
	int refx;

	for(m=0;m<16;m++)
	{
		refx=m+x0;
		if(refx>=Llimit&&refx<Rlimit)
		{
			temp=(word[m+16]<<8)+word[m];
			for(n=0;n<16;n++) 
			{				
				if(temp&0x8000)LCD_SetPoint(refx,y0+15-n,color); 
				else if(Charmd)LCD_SetPoint(refx,y0+15-n,Bcolor);
				temp<<=1;
			}
		}
	}
}
//��Ӣ�Ļ����ʾ
void Show_Fonts(u16 x0,u16 y0,u8* Fonts,u16 color)
{
	u32   offset;

	while(*Fonts!='\0')
	{			
		if(*Fonts<0x81)//��ASCII�ַ�
		{
			LCD_Char(x0,y0,*Fonts,color);
			*Fonts++;//ƫ�Ƶ�ַ��1
			x0+=Fsize/2;	
		}			
		else//�Ǻ��� 
		{	
			offset=Get_FontAdr(Fonts);
			SPI_Flash_Read(&GBK_Buf[0],FON_addr*528+offset,32);			
			Show_Hanzi(x0,y0,GBK_Buf,color);			
			*Fonts++;*Fonts++;//ƫ�Ƶ�ַ��2
			x0+=Fsize;
		}
	}	
}
//����ָ��·���µ�ͼƬ��SPI FLASH����addrҳ��ʼ��ҳ��Ϊnum
void Save_pic(u8* fname,u16 addr,u16 num)
{					 	 			
	u8 res;
	FIL fileR;
	UINT Bytes;
	u16 i,page,color;
	BITMAPFILEHEADER* bmph;

	//LCD_Cursor(0,0);
  	//LCD_REG=0x22;
	Add_path(path_curr,fname);
	res=f_open(&fileR,(TCHAR*)path_curr,FA_READ);
	Cut_path(path_curr);
	if(res)return;
	f_read(&fileR,AT45_buffer,54,&Bytes);//��ȡ54�ֽ��ļ�ͷ
	bmph=(BITMAPFILEHEADER*)AT45_buffer;
	f_lseek(&fileR,bmph->bfOffBytes);//ֱ������ͼ������
	for(page=0;page<num;page++)//����λ�ã���0~290ҳ����153600�ֽ�
	{		
		f_read(&fileR,&AT45_buffer[0],792,&Bytes);//��һ�����ݵ�SPI FLASH����
		for(i=0;i<264;i++)//264���ָպ�дһҳ
		{
			color=(AT45_buffer[3*i]>>3)+((AT45_buffer[3*i+1]&0xfc)<<3)+((AT45_buffer[3*i+2]&0xf8)<<8);
			AT45_buffer[2*i]=color&0xff;
			AT45_buffer[2*i+1]=color>>8;
			//LCD_RAM=color;
		}
		FlashPageWrite(addr+page,&AT45_buffer[0]);//д��SPI FLASH
	} 
	f_close(&fileR);	
}
//����ϵͳͼƬ
void Update_patterns(void)
{
	My_chdir("0:/PICTURE");
	Dialog_box(40,180,"���ڸ���ϵͳͼ�������Ե�...");
	Save_pic("i.bmp",BGD_addr,BGD_size);
	My_chdir("0:/SYSTEM/SYSICO");
	Save_pic("ico0.bmp",Icoaddr[0],ICO_size);
	Save_pic("ico1.bmp",Icoaddr[1],ICO_size);
	Save_pic("ico2.bmp",Icoaddr[2],ICO_size);
	Save_pic("ico3.bmp",Icoaddr[3],ICO_size);
	Save_pic("ico4.bmp",Icoaddr[4],ICO_size);
	Save_pic("ico5.bmp",Icoaddr[5],ICO_size);
	Save_pic("ico6.bmp",Icoaddr[6],ICO_size);
	Save_pic("ico7.bmp",Icoaddr[7],ICO_size);
	Save_pic("ico8.bmp",Icoaddr[8],ICO_size);
	Save_pic("ico9.bmp",Icoaddr[9],ICO_size);
	Save_pic("ico10.bmp",Icoaddr[10],ICO_size);
	Save_pic("ico11.bmp",Icoaddr[11],ICO_size);
	Save_pic("ico12.bmp",Icoaddr[12],ICO_size);
	Save_pic("ico13.bmp",Icoaddr[13],ICO_size);
	Save_pic("ico14.bmp",Icoaddr[14],ICO_size);
	Save_pic("ico15.bmp",Icoaddr[15],ICO_size);
	Save_pic("ico16.bmp",Icoaddr[16],ICO_size);
	Save_pic("ico17.bmp",Icoaddr[17],ICO_size);
	Save_pic("ico18.bmp",Icoaddr[18],ICO_size);
	Save_pic("ico19.bmp",Icoaddr[19],ICO_size);

	Save_pic("fil.bmp",Typaddr[T_ANY],TYP_size);
	Save_pic("fol.bmp",Typaddr[T_DIR],TYP_size);
	Save_pic("aud.bmp",Typaddr[T_AUDIO],TYP_size);
	Save_pic("pic.bmp",Typaddr[T_IMAGE],TYP_size);
	Save_pic("vid.bmp",Typaddr[T_VIDO],TYP_size);
	Save_pic("txt.bmp",Typaddr[T_BOOK],TYP_size);
	Save_pic("gam.bmp",Typaddr[T_GAME],TYP_size);
	Save_pic("sys.bmp",Typaddr[T_SYS],TYP_size);
	Save_pic("pla.bmp",Typaddr[T_PLAY],TYP_size);
	Save_pic("pre.bmp",Typaddr[T_PREV],TYP_size);
	Save_pic("nex.bmp",Typaddr[T_NEXT],TYP_size);
	Save_pic("pau.bmp",Typaddr[T_PAUSE],TYP_size);

	Save_pic("clock.bmp",CLK_addr,CLK_size);
	Save_pic("lock.bmp",LCK_addr,LCK_size);
	Save_mono("scroll.bmp",SCR_addr,SCR_size);
	Save_mono("dragon.bmp",GDL_addr,GDL_size);
	Del_Dialog();
}
/********************************************************************
						����ͼƬ���ú���
********************************************************************/
//��SPI FLASH��ȡ�����Ʊ���ͼƬ
void Load_bground(void)
{
	u16	i,page;

	LCD_Cursor(0,0);
  	LCD_REG=0x22;
	for(page=0;page<290;page++)
	{
		FlashPageRead(page,&AT45_buffer[0]);
		for(i=0;i<528;i+=2)LCD_RAM=(AT45_buffer[i+1]<<8)+AT45_buffer[i];//��ʾ
	}
	FlashPageRead(290,&AT45_buffer[0]);//��ȡ����ʾʣ������
	for(i=0;i<480;i+=2)LCD_RAM=(AT45_buffer[i+1]<<8)+AT45_buffer[i];	
}
/********************************************************************
						ʱ��ͼƬ���ú���
********************************************************************/
//��SPI FLASH��ȡ������ʱ��
void Load_clock(u16 x,u16 y)
{
	u16	i,j,k,page,color,refx,refy;

	k=528;
	refx=x+104;
	refy=y+104;
	page=CLK_addr;
	for(j=y;j<refy;j++)
	for(i=x;i<refx;i++)
	{
		if(k==528)
		{
			FlashPageRead(page,&AT45_buffer[0]);
			page++;
			k=0;
		}
		color=(AT45_buffer[k+1]<<8)+AT45_buffer[k];
		if(color!=WHITE)
		{
			TransPoint(i,j,color,90);
			//LCD_SetPoint(i,j,color);
		}
		k+=2;					
	}
}
/****************************************************************************
						�ô洢�豸ʵ�ֻ���ͼ��
****************************************************************************/
u16 bufwidth=240;//����Ŀ�͸�
u16 bufheight=80;
//�ڻ����л���,ע��y<=80
void Win_SetPoint(u16 x,u16 y,u16 color)
{
	if(y>bufheight||x>=bufwidth)return;
	gui_buf[bufwidth*y+x]=color;
}
//�ڻ����ж���
u16 Win_Getpoint(u16 x,u16 y)
{
	if(y>bufheight||x>=bufwidth)return 0;
	return gui_buf[bufwidth*y+x];
}
//�ڻ����л�͸����,ע��y<80
void Win_TransPoint(u16 x,u16 y,u16 fcolor,u8 Trans)
{
	u16 r1=0,g1=0,b1=0,r2=0,g2=0,b2=0,color;		      

	if(x>240||y>bufheight)return;
	r2=((fcolor>>11)&0x1F)*Trans/100;
	g2=((fcolor>>5)&0x3F)*Trans/100;
	b2=(fcolor&0x1F)*Trans/100;
	color=Win_Getpoint(x,y);//����		
	r1 = ((color>>11)&0x1F)*(100-Trans)/100;//����͸���Ȳ����Ըõ������Ⱦ
	g1 = ((color>>5)&0x3F)*(100-Trans)/100;
	b1 = (color&0x1F)*(100-Trans)/100;
	color = ((r1+r2)<<11)+((g1+g2)<<5)+(b1+b2);//͸��
	Win_SetPoint(x,y,color);//����
}
//�ڻ����л�ˮƽֱ��
void Win_HTLine(u16 x1, u16 y, u16 x2 ,u16 color,u8 trans)
{
	u16 i;
	for(i=x1;i<x2;i++)Win_TransPoint(i,y,color,trans);
}
//�ڻ����л�ˮƽֱ��
void Win_VTLine(u16 y1, u16 x, u16 y2 ,u16 color,u8 trans)
{
	u16 i;
	for(i=y1;i<y2;i++)Win_TransPoint(x,i,color,trans);
}
//�ڻ��������͸������
void Win_Fill(u16 x1,u16 y1,u16 x2,u16 y2,u16 color,u16 trans)
{
	u16 i,j;
	for(j=y1;j<y2;j++)
		for(i=x1;i<x2;i++)Win_TransPoint(i,j,color,trans);
}
//��͸���Ƚ�������
void Win_SFill(u16 x1,u16 y1,u16 x2,u16 y2,u16 color,u8 trans1,u8 trans2)
{
	u16 j;
	int trans;

	for(j=y1;j<y2;j++)
	{
		trans=trans2-trans1;
		if(trans>0)trans=trans1+(trans)*(j-y1)/(y2-y1);
		else trans=trans2+(-trans+trans*(j-y1)/(y2-y1)); 
		Win_HTLine(x1,j,x2,color,trans);
	}	
}
//�ڻ����л�Բ
void WIN_TCircle(u16 x0, u16 y0, u16 r,u16 color,u8 trans)
{
  	u16 i,x=r;
  	u16 imax=(r*707)/1000+1;
  	u16 sqmax=r*r+r/2;

  	Win_HTLine(x0-r,y0,x0+r,color,trans);
  	for(i=1;i<=imax;i++)
	{
    	if((i*i+x*x)>sqmax)
		{
      		if(x>imax) 
			{
        		Win_HTLine (x0-i+1,y0+x, x0+i-1,color,trans);
        		Win_HTLine (x0-i+1,y0-x, x0+i-1,color,trans);
      		}
      		x--;
    	}
    	Win_HTLine(x0-x,y0+i, x0+x,color,trans);
    	Win_HTLine(x0-x,y0-i, x0+x,color,trans);
  	}
}
//�ڻ�������ʾ�ַ�
void Win_Char(int x,u16 y,u8 num,u16 color)
{     
	u8  m,n;
	u16 temp,refx;

	num=num-' ';//�õ�ƫ�ƺ��ֵ
	for(m=0;m<8;m++)
	{
		refx=x+m;
		if(refx>0&&refx<240)
		{
			temp=(asc2_1608[num][m+8]<<8)+asc2_1608[num][m];
			for(n=0;n<16;n++) 
			{												
				if(temp&0x0001)Win_SetPoint(refx,y+n,color); 
				else if(Charmd)Win_SetPoint(refx,y+n,Bcolor);
				temp>>=1;
			}
		}
	} 	   	   	 	  
} 
//��ʾһ������
void Win_Hanzi(int x0,u16 y0,u8 *word,u16 color)
{
	u8  m,n;
	u16 temp,refx,refy;

	for(m=0;m<16;m++)
	{		
		refx=m+x0;
		refy=15+y0;
		temp=word[m+16]*256+word[m];
		for(n=0;n<16;n++) 
		{			
			if((temp&0x8000)&&refx>0&&refx<240)Win_SetPoint(refx,refy-n,color); 
			temp<<=1;
		}
	}
}
//��Ӣ�Ļ����ʾ
void Win_Fonts(int x0,u16 y0,u8* Fonts,u16 color)
{
	u32   offset;

	while(*Fonts!='\0')
	{			
		if(*Fonts<0x81)//��ASCII�ַ�
		{
			if(x0>-8&&x0<240)Win_Char(x0,y0,*Fonts,color);
			*Fonts++;//ƫ�Ƶ�ַ��1
			x0+=Fsize/2;	
		}			
		else//�Ǻ��� 
		{	
			if(x0>-16&&x0<240)
			{
				offset=Get_FontAdr(Fonts);
				SPI_Flash_Read(&GBK_Buf[0],FON_addr*528+offset,32);			
				Win_Hanzi(x0,y0,GBK_Buf,color);
			}			
			*Fonts++;*Fonts++;//ƫ�Ƶ�ַ��2
			x0+=Fsize;
		}
	}	
}
//������ʾ
void Win_Num(u16 x,u16 y,u32 num,u8 len,u16 color)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				Win_Char(x+(Fsize/2)*t,y,'0',color);
				continue;
			}else enshow=1; 		 	 
		}
	 	Win_Char(x+(Fsize/2)*t,y,temp+'0',color); 
	}
} 	
//�ڻ���������LINE1����LINE2�еı���,����������80��
void Fill_back(u16 LINE1,u16 LINE2)
{
	u16 i,j,page;

	j=0;
	page=BGDpage(LINE1);
	FlashPageRead(page,&AT45_buffer[0]);
	for(i=BGDoffset(LINE1);i<528;i++)
	{
		work_buf[j]=AT45_buffer[i];
		j++;
	}
	page++;
	for(;page<=BGDpage(LINE2);page++)
	{
		FlashPageRead(page,&AT45_buffer[0]);
		for(i=0;i<528;i++)
		{
			work_buf[j]=AT45_buffer[i];
			j++;
		}
	}
}
//��ͼ�����뻺��
void Load_icon(int x,u16 y,u8 iconum)
{
	u16	j,k,page,color,refy;
	int	i,refx;

	k=528;
	refx=x+60;
	refy=y+60;
	page=Icoaddr[iconum];
	for(j=y;j<refy;j++)
	for(i=x;i<refx;i++)
	{
		if(k==528)
		{
			FlashPageRead(page,&AT45_buffer[0]);
			page++;
			k=0;
		}
		if(i>=0&&i<240)//��֤��������ʾ����
		{
			color=(AT45_buffer[k+1]<<8)+AT45_buffer[k];
			if(color!=WHITE)
			{
				Win_SetPoint(i,j,color);
				//if(j>=y+40)Win_TransPoint(i,refy-j+60,color,(j-y)*5/3,240);
			}
		}
		k+=2;					
	}
	Win_Fonts(x+1,y+62,(u8*)Icotext[iconum],WHITE);
}
//���ļ�����ͼ�����뻺��
void Load_type(int x,int y,u8 typenum)
{
	u16	k,page,color,refx;
	int	i,j,refy;

	k=528;
	refx=x+24;
	refy=y+24;
	page=Typaddr[typenum];
	for(j=y;j<refy;j++)
	{
		for(i=x;i<refx;i++)
		{
			if(k==528)
			{
				FlashPageRead(page,&AT45_buffer[0]);
				page++;
				k=0;
			}
			if(j>=0&&j<bufheight&&i>=0&&i<240)//��֤��������ʾ����
			{						
				color=(AT45_buffer[k+1]<<8)+AT45_buffer[k];
				if(color!=BLACK)Win_SetPoint(i,j,color);
			}
			k+=2;					
		}
	}
}
//��ʾ�����������
void Dip_guibuf(u16 LINE1,u16 LINE2)
{
	u16 i,len;

	len=(LINE2-LINE1)*240;
	LCD_Window(0,LINE1,239,LINE2-1);
	LCD_Cursor(0,LINE1);
	LCD_REG=0x22;
	for(i=0;i<len;i++)LCD_RAM=gui_buf[i];
	LCD_Window(0,0,239,319);				
}
//��X������Ϊ-240~240��Y������Ϊ150��235��λ�û�����ͼ��
void Set_mico(int xpos)
{
	LCD_Charmd(0,WHITE);
	Fill_back(150,230);
	/*��ʾ��0��ͼ��*/
	//��ҳ��ͼ�꣨240��	
	if(xpos>160)Load_icon(xpos-220,0,0);
	if(xpos>90)Load_icon(xpos-150,0,1);
	if(xpos>20)Load_icon(xpos-80,0,2);
	//�м�ҳ��ͼ�꣨0��
	if(xpos>-80&&xpos<220)Load_icon(xpos+20,0,6);
	if(xpos>-150&&xpos<150)Load_icon(xpos+90,0,7);
	if(xpos>-220&&xpos<80)Load_icon(xpos+160,0,8);
	//��ҳ��ͼ�꣨-240��
	if(xpos<-20)Load_icon(xpos+260,0,12);
	if(xpos<-90)Load_icon(xpos+330,0,13);
	if(xpos<-160)Load_icon(xpos+400,0,14);
	Dip_guibuf(150,230);
	//Fill_slipbuf(1);
	Fill_back(235,315);
	/*��ʾ��1��ͼ��*/
	//��ҳ��ͼ�꣨240��
	if(xpos>160)Load_icon(xpos-220,0,3);
	if(xpos>90)Load_icon(xpos-150,0,4);
	if(xpos>20)Load_icon(xpos-80,0,5);
	//�м�ҳ��ͼ�꣨0��
	if(xpos>-80&&xpos<220)Load_icon(xpos+20,0,9);
	if(xpos>-150&&xpos<150)Load_icon(xpos+90,0,10);
	if(xpos>-220&&xpos<80)Load_icon(xpos+160,0,11);
	//��ҳ��ͼ�꣨-240��
	if(xpos<-20)Load_icon(xpos+260,0,15);
	if(xpos<-90)Load_icon(xpos+330,0,16);
	if(xpos<-160)Load_icon(xpos+400,0,17);	
	Dip_guibuf(235,315);
}
//���ƿ��ͼ��
void Set_eico(int xpos)//��ʾ��8~184		�����꣺17	   
{		//-30		 210
	if(xpos>120&&xpos<360)Draw_icon(xpos-150,17,icolist[0],WHITE);
	if(xpos>60&&xpos<300)Draw_icon(xpos-90,17,icolist[1],WHITE);
	if(xpos>0&&xpos<240)Draw_icon(xpos-30,17,icolist[2],WHITE);

	if(xpos>-60&&xpos<180)Draw_icon(xpos+30,17,icolist[3],WHITE);
	if(xpos>-120&&xpos<120)Draw_icon(xpos+90,17,icolist[4],WHITE);
	if(xpos>-180&&xpos<60)Draw_icon(xpos+150,17,icolist[5],WHITE);

	if(xpos>-240&&xpos<0)Draw_icon(xpos+210,17,icolist[6],WHITE);
	if(xpos>-300&&xpos<-60)Draw_icon(xpos+270,17,icolist[7],WHITE);
	if(xpos>-360&&xpos<-120)Draw_icon(xpos+330,17,icolist[8],WHITE);

	if(xpos>=180)LCD_Fill(30,17,xpos-150,77,WHITE);
	if(xpos<=-180)LCD_Fill(xpos+390,17,210,77,WHITE);	  
}
//ѡ��ͼ��
void ICO_Select(u16 xpos,u16 ypos,u8 iconum)
{
	u16	i,j,k,page,color;

	k=528;
	page=Icoaddr[iconum];
	//LCD_Window(0,0,239,319);
	for(j=ypos;j<ypos+60;j++)
	for(i=xpos;i<xpos+60;i++)
	{
		if(k==528)
		{
			FlashPageRead(page,&AT45_buffer[0]);
			page++;
			k=0;
		}
		color=(AT45_buffer[k+1]<<8)+AT45_buffer[k];
		if(color==WHITE)TransPoint(i,j,GREEN,(60+ypos-j)*5/3);
		else LCD_SetPoint(i,j,color);
		k+=2;					
	}
	delay_ms(400);
}
//��ͼ�궯̬��λ����
int MICO_slipback(int xpos)
{
	if(xpos>=120&&xpos<=240)
	{
		while(xpos<240)
		{			
			xpos+=(241-xpos)/2;
			Set_mico(xpos);				
		}
	}
	else if(xpos<120&&xpos>0)
	{
		while(xpos>0)
		{				
			xpos-=(xpos+1)/2;
			Set_mico(xpos);				
		}
	}
	else if(xpos<0&&xpos>-120)
	{
		while(xpos<0)
		{			
			xpos+=(1-xpos)/2;
			Set_mico(xpos);				
		}
	}
	else if(xpos<=-120&&xpos>=-240)
	{
		while(xpos>-240)
		{			
			xpos-=(241+xpos)/2;
			Set_mico(xpos);				
		}
	}
	else if(xpos>240)
	{
		while(xpos>240)
		{			
			xpos-=(xpos-239)/2;	
			Set_mico(xpos);			
		}
	}
	else if(xpos<-240)
	{
		while(xpos<-240)
		{			
			xpos+=(-xpos-239)/2;
			Set_mico(xpos);				
		}
	}
	return xpos;
}
//���ͼ�궯̬��λ����
int EICO_slipback(int xpos)	
{							
	u16 temp,offset;

	xpos+=240;
	temp=xpos/60;//0~8
	offset=xpos%60;
	if(offset<30)//����λ
	{
		temp=temp*60;
	 	while(xpos>temp)
		{
			if(xpos>temp+1)xpos-=2;
			else xpos--;
			Set_eico(xpos-240);
		}
	}
	else//���Ҷ�λ
	{
		temp=(temp+1)*60;
	 	while(xpos<temp)
		{
			if(xpos<temp-1)xpos+=2;
			else xpos++;
			Set_eico(xpos-240);
		}		
	}
	return xpos-240;
}
/********************************************************************
						����ͼ�����ú���
********************************************************************/
//��������ͼ��
void Load_lock(u16 x)
{
	u16	i,j,k,a,page,color,refx;

	k=528;
	refx=x+80;
	page=LCK_addr;
	for(j=0;j<70;j++)
	{
		for(i=x;i<refx;i++)
		{
			if(k==528)
			{
				FlashPageRead(page,&AT45_buffer[0]);
				page++;
				k=0;
			}
			color=(AT45_buffer[k+1]<<8)+AT45_buffer[k];
			if(color!=WHITE)
			{
				if(color==RED)Win_TransPoint(i,j,WHITE,70-j);
				else if(color==BLACK)Win_TransPoint(i,j,BLACK,30);
				else Win_SetPoint(i,j,color);
			}
			k+=2;
								
		}
		for(a=0;a<x;a++)Win_TransPoint(a,j,WHITE,70-j);
	}
}
//�ڻ������л��ƽ���ͼ��,xpos�������0
void Draw_lock(u8 xpos)
{
	u16 i,j,page,endpage;
	u16 r1,g1,b1,color;

	j=0;
	page=BGDpage(180);
	FlashPageRead(page,&AT45_buffer[0]);
	for(i=BGDoffset(180);i<528;i+=2)
	{
		color=(AT45_buffer[i+1]<<8)+AT45_buffer[i];
		r1 = ((color>>11)&0x1F)>>1;//����͸���Ȳ����Ըõ������Ⱦ
		g1 = ((color>>5)&0x3F)>>1;
		b1 = (color&0x1F)>>1;
		color = (r1<<11)+(g1<<5)+b1;//��ɫ͸��
		gui_buf[j]=color;
		j++;
	}
	page++;
	endpage=BGDpage(250)+1;
	for(;page<endpage;page++)
	{
		FlashPageRead(page,&AT45_buffer[0]);
		for(i=0;i<528;i+=2)
		{
			color=(AT45_buffer[i+1]<<8)+AT45_buffer[i];
			r1 = ((color>>11)&0x1F)>>1;//����͸���Ȳ����Ըõ������Ⱦ
			g1 = ((color>>5)&0x3F)>>1;
			b1 = (color&0x1F)>>1;
			color = (r1<<11)+(g1<<5)+b1;//��ɫ͸��
			gui_buf[j]=color;
			j++;
		}
	}
	WIN_TCircle(183,35,5,WHITE,xpos*2/3);
	Load_lock(xpos);
	LCD_Window(0,180,239,249);
	LCD_Cursor(0,180);
	LCD_REG=0x22;
	for(i=0;i<16800;i++)LCD_RAM=gui_buf[i];
	LCD_Window(0,0,239,319);
}
//���ƺ�ɫ͸���ı���
void Trans_bground(u8 trans)
{
	u16	i,page,color,r1,g1,b1;;

	//LCD_Window(0,0,239,319);
	LCD_Cursor(0,0);
  	LCD_REG=0x22;
	for(page=0;page<290;page++)
	{
		FlashPageRead(page,&AT45_buffer[0]);
		for(i=0;i<528;i+=2)
		{
			color=(AT45_buffer[i+1]<<8)+AT45_buffer[i];
			r1 = ((color>>11)&0x1F)*trans/100;//����͸���Ȳ����Ըõ������Ⱦ
			g1 = ((color>>5)&0x3F)*trans/100;
			b1 = (color&0x1F)*trans/100;
			color = (r1<<11)+(g1<<5)+b1;//��ɫ͸��
			LCD_RAM=color;
		}
	}
	FlashPageRead(290,&AT45_buffer[0]);//��ȡ����ʾʣ������
	for(i=0;i<480;i+=2)
	{
		color=(AT45_buffer[i+1]<<8)+AT45_buffer[i];
		r1 = ((color>>11)&0x1F)*trans/100;//����͸���Ȳ����Ըõ������Ⱦ
		g1 = ((color>>5)&0x3F)*trans/100;
		b1 = (color&0x1F)*trans/100;
		color = (r1<<11)+(g1<<5)+b1;//��ɫ͸��
		LCD_RAM=color;
	}
}
//ͼ�궯̬��λ����
int LCK_Slipback(int xpos)
{
	if(xpos>0&&xpos<150)
	{
		while(xpos>0&&PEN_int!=RESET)
		{			
			xpos-=(xpos+1)/2;
			Draw_lock(xpos);				
		}
	}
	return xpos;
}
//��ʾ����ҳ
void Load_lockpage(void)
{
	int basex=0,xpos=0;

	Trans_bground(50);
	Draw_lock(0);
	while(basex<150)
	{
		if(PEN_int==RESET)
		{			
			ADS_SPI_Init();
			Read_X(&PENX);
			basex-=PENX;
			while(PEN_int==RESET)
			{
				ADS_SPI_Init();
				Read_X(&PENX);
				xpos=basex+PENX;
				if(xpos>150)xpos=150;
				if(xpos<0)xpos=0;
				FLASH_SPI_Init();
				Draw_lock(xpos);				
			}
			basex=LCK_Slipback(xpos);
		}
	}		
}
/****************************************************************************
							��ͼ����
****************************************************************************/
extern u32 lcdid;
u8 Prt_Screen(u8* fpath)
{
	u8 res;
	u16 i,j,color;
	UINT WRBytes;            
	FIL fileW;	
			
	//LCD_Window(0,0,239,319);
	res=f_open(&fileW,(TCHAR*)fpath,FA_WRITE|FA_CREATE_ALWAYS);//��������
	if(res)return 1;
	WriteHeader(&fileW);
	WriteBMPInfo(&fileW);
	for(i=320;i>0;i--)
	{
		for(j=0;j<240;j++)
		{
			LCD_Cursor(j,i-1);
			//if((lcdid&0xff00)==0x9300)gui_buf[j]=BGR565toRGB555(LCD_ReadRAM());
			//else gui_buf[j]=RGB565toRGB555(LCD_ReadRAM());
			color=BGR565toRGB555(LCD_ReadRAM());
			gui_buf[j]=color;
		}
		res=f_write(&fileW,work_buf,480,&WRBytes);
		if(res)return 2;
	}
	f_close(&fileW);
	return 0;
}
//дBMPͷ
void WriteHeader(FIL* file)
{
	UINT WRBytes;
	u8 BMP_header[14];	
	BITMAPFILEHEADER* bmph;

	bmph=(BITMAPFILEHEADER*)BMP_header;
	bmph->bfType=0x4d42;
	bmph->bfSize=0x25836;//�ļ���С,����
	bmph->bfReserved1=0;//����,��������Ϊ0
	bmph->bfReserved2=0;
	bmph->bfOffBytes=54;//���ļ�ͷ��λͼ��������ƫ���ֽ�
	f_write(file,BMP_header,14,&WRBytes);
}
//дBMPͷ��Ϣ
void WriteBMPInfo(FIL* file)
{
 	UINT WRBytes;
	u8 BMP_info[40];	
	BITMAPINFOHEADER* bmpi;

	bmpi=(BITMAPINFOHEADER*)BMP_info;
	bmpi->biSize=40;
	bmpi->biWidth=240;
	bmpi->biHeight=320;
	bmpi->biPlanes=1;
	bmpi->biBitCount=16;
	bmpi->biCompression=0;
	bmpi->biSizeImage=0;
	bmpi->biXPelsPerMeter=0;
	bmpi->biYPelsPerMeter=0;
	bmpi->biClrUsed=0;
	bmpi->biClrImportant=0;
	f_write(file,BMP_info,40,&WRBytes);
}
/****************************************************************************
							��ͼЧ������
****************************************************************************/
//��̬����Ч��
void Dyna_clear(u16 color)
{
	u32 k;
	u16 i,j;

	Draw_scroll(40);
	for(i=0,j=0;i<101;i++,j++)
	{
		LCD_Window(100-i,140-j,139+i,219+j);
		LCD_Cursor(100-i,140-j);
		LCD_REG=0x22;
		for(k=(40+i+i)*(120+j+j);k>0;k--)LCD_RAM=color;
	}
	LCD_Window(0,0,239,319);
}
//���Ʊ���ͼƬ��ĳһ��,mode1:ֱ�Ӵ�SPI FLASH�ж�ȡ����,mode0:�ӻ����ж�ȡ����
void Draw_BGDline(u16 linenum,u8 mode)
{
	u16 i,page,offset;

	if(mode)
	{
		page=BGDpage(linenum);
		offset=BGDoffset(linenum);
		FlashPageRead(page,&AT45_buffer[0]);
		LCD_Cursor(0,linenum);
		LCD_REG=0x22;
		if(offset<=48)//������һҳ����
		{	   	
			for(i=0;i<480;i+=2)LCD_RAM=(AT45_buffer[i+offset+1]<<8)+AT45_buffer[i+offset];//��ʾ
		}
		else//һ�п���ҳ
		{
			for(i=offset;i<528;i+=2)LCD_RAM=(AT45_buffer[i+1]<<8)+AT45_buffer[i];//��ʾ
			FlashPageRead(page+1,&AT45_buffer[0]);
			for(i=0;i<offset-48;i+=2)LCD_RAM=(AT45_buffer[i+1]<<8)+AT45_buffer[i];
		}
	}
	else
	{
		LCD_Cursor(0,linenum);
		LCD_REG=0x22;
		for(i=0;i<240;i++)LCD_RAM=gui_buf[240*(linenum-40)+i];	
	}	
}
//��̬������ʾ����
void Dyna_bground(void)
{
	u16 i;

	Dyna_clear(BLACK);
	for(i=0;i<141;i++)
	{
		Draw_BGDline(180-i,1);
		Draw_BGDline(179+i,1);
		delay_ms(2);
	}
}
/****************************************************************************
							�Ի�����
****************************************************************************/
//�ڻ������л�͸����,ע��y<80
void Trans_WinPoint(u16 x,u8 y,u16 fcolor,u8 Trans)//
{
	u16 r1=0,g1=0,b1=0,r2=0,g2=0,b2=0;
	u16 color;

	r2=((fcolor>>11)&0x1F)*Trans/100;
	g2=((fcolor>>5)&0x3F)*Trans/100;
	b2=(fcolor&0x1F)*Trans/100;

	color = gui_buf[200*(y-160)+(x-20)];
	r1 = ((color>>11)&0x1F)*(100-Trans)/100;//����͸���Ȳ����Ըõ������Ⱦ
	g1 = ((color>>5)&0x3F)*(100-Trans)/100;
	b1 = (color&0x1F)*(100-Trans)/100;
	color = ((r1+r2)<<11)+((g1+g2)<<5)+(b1+b2);//͸��
	LCD_SetPoint(x,y,color);//����
}
//����β͸���ĺ���
void Spec_WINLine(u16 x1, u16 y, u16 x2 ,u16 color,u8 trans)
{
	u16 i;
	for(i=1;i<11;i++)Trans_WinPoint(x1+i-1,y,color,trans*i/10);
	for(i=x1+10;i<x2-9;i++)Trans_WinPoint(i,y,color,trans);
	for(i=1;i<11;i++)Trans_WinPoint(x2-10+i,y,color,trans*(10-i)/10);
}
//��͸��Բ�Ǿ���
void Draw_RRect(u16 x1,u16 y1,u16 x2,u16 y2)
{
	u8 i;

	Spec_WINLine(x1+9,y1,x2-9,WHITE,20);
	Spec_WINLine(x1+9,y2,x2-9,WHITE,20);
	Spec_WINLine(x1+5,y1+1,x2-5,WHITE,29);
	Spec_WINLine(x1+5,y2-1,x2-5,WHITE,29);
	Spec_WINLine(x1+4,y1+2,x2-4,WHITE,38);
	Spec_WINLine(x1+4,y2-2,x2-4,WHITE,38);
	Spec_WINLine(x1+3,y1+3,x2-3,WHITE,47);
	Spec_WINLine(x1+3,y2-3,x2-3,WHITE,47);
	Spec_WINLine(x1+2,y1+4,x2-2,WHITE,56);
	Spec_WINLine(x1+2,y2-4,x2-2,WHITE,56);
	for(i=5;i<9;i++)Spec_WINLine(x1+1,y1+i,x2-1,WHITE,56+9*(i-4));
	for(i=5;i<9;i++)Spec_WINLine(x1+1,y2-i,x2-1,WHITE,56+9*(i-4));
	for(i=y1+9;i<y2-8;i++)Spec_WINLine(x1,i,x2,WHITE,92);
}
//��ʾ�Ի���,x,yΪ�ַ�������
void Dialog_box(u8 x,u8 y,u8* Fonts)
{
	int k;
	u32 offset;
	u16 i,j,color;

	LCD_Charmd(0,WHITE);
	for(j=160,k=0;j<250;j++)//���Ի��������ݴ���������
	{
		for(i=20;i<220;i++,k++)
		{
			color=LCD_GetPoint(i,j);
			gui_buf[k]=color;
		}	
	}	
	for(i=0;i<35;i++)Draw_RRect(55-i,195-i,184+i,214+i);
	while(*Fonts!='\0')
	{			
		if(*Fonts<0x81)//��ASCII�ַ�
		{
			LCD_Char(x,y,*Fonts,GRAY[10]);
			*Fonts++;//ƫ�Ƶ�ַ��1
			x+=Fsize/2;
			if(x>192)
			{
				y+=20;
				x=40;
			}				
		}			
		else//�Ǻ��� 
		{	
			offset=Get_FontAdr(Fonts);
			SPI_Flash_Read(&GBK_Buf[0],FON_addr*528+offset,32);			
			Show_Hanzi(x,y,GBK_Buf,GRAY[10]);			
			*Fonts++;*Fonts++;//ƫ�Ƶ�ַ��2
			x+=Fsize;
			if(x>184)
			{
				y+=20;
				x=40;
			}
		}
	}
}
//ɾ���Ի���
void Del_Dialog(void)
{
	u16 i,j;

	for(i=0;i<45;i++)
	{
		for(j=20;j<220;j++)Trans_WinPoint(j,160+i,WHITE,0);
		for(j=20;j<220;j++)Trans_WinPoint(j,249-i,WHITE,0);
		delay_ms(3);
	}
}
/****************************************************************************
							����ɫλͼ����
****************************************************************************/
//����ָ��·���µĵ�ɫλͼ��SPI FLASH����addrҳ��ʼ��ҳ��Ϊnum
//���õ�ɫλͼ�Ŀ����Ϊ32��������
void Save_mono(u8* fname,u16 addr,u16 num)
{
	u8 res;//,k,data;
	FIL fileR;
	UINT Bytes;
	u16 page;//,i;
	BITMAPFILEHEADER* bmph;

	//LCD_Window(0,0,239,319);
	//LCD_Cursor(0,0);
  	//LCD_REG=0x22;
	Add_path(path_curr,fname);
	res=f_open(&fileR,(TCHAR*)path_curr,FA_READ);
	Cut_path(path_curr);
	if(res)return;
	f_read(&fileR,AT45_buffer,54,&Bytes);//��ȡ54�ֽ��ļ�ͷ
	bmph=(BITMAPFILEHEADER*)AT45_buffer;
	f_lseek(&fileR,bmph->bfOffBytes);//ֱ������ͼ������
	for(page=0;page<num;page++)
	{		
		f_read(&fileR,&AT45_buffer[0],528,&Bytes);//��һ�����ݵ�SPI FLASH����
//		for(i=0;i<528;i++)
//		{			
//			data=AT45_buffer[i];
//			for(k=0;k<8;k++)
//			{
//				if((data<<k)&0x80)LCD_RAM=WHITE;
//				else LCD_RAM=BLACK;					
//			}			
//		}
		FlashPageWrite(addr+page,&AT45_buffer[0]);//д��SPI FLASH
	} 
	f_close(&fileR);
}
//���Ƶ�ɫλͼ
void Draw_mono(u16 x,u16 y,u16 width,u16 height,u16 addr,u16 color,u8 trans)
{
	u8 a,data;
	u16	i,j,k,page,refx,refy;

	k=528;
	refx=x+width;
	refy=y+height;
	page=addr;
	//LCD_Window(0,0,239,319);
	for(j=y;j<refy;j++)
	{
		for(i=x;i<refx;i+=8)
		{
			if(k==528)
			{
				FlashPageRead(page,&AT45_buffer[0]);
				page++;
				k=0;
			}
			data=AT45_buffer[k];
			for(a=0;a<8;a++)if(!((data<<a)&0x80))
			{			
				TransPoint(i+a,j,color,trans);
			}
			k++;					
		}
	}
}
/****************************************************************************
							�����˵�����
****************************************************************************/
//���ƾ���
void Draw_scroll(u16 ypos)//yposΪ�ײ�������
{          
	u8 a,data;
	u16	i,k,page,offset;//
	int j;	
	
	scrpos=ypos;
	//10�лҶȽ���
	for(i=10;i>6;i--)LCD_HLine(0,ypos-i,240,WHITE);		
	LCD_HLine(0,ypos-6,240,GRAY[31]);
	LCD_HLine(0,ypos-5,240,GRAY[30]);
	LCD_HLine(0,ypos-4,240,GRAY[28]);	
	LCD_HLine(0,ypos-3,240,GRAY[24]);	
	LCD_HLine(0,ypos-2,240,GRAY[20]);
	LCD_HLine(0,ypos-1,240,GRAY[15]);
	//30~110��ͼ��								   
	page=((120-ypos)*28)/528+SCR_addr;
	offset=((120-ypos)*28)%528;
	FlashPageRead(page,&AT45_buffer[0]);
	page++;
	k=offset;
	for(j=0;j<ypos-10;j++)
	{
		LCD_HLine(0,j,8,WHITE);
		LCD_HLine(232,j,240,WHITE);
		for(i=8;i<232;i+=8)
		{
			if(k==528)
			{
				FlashPageRead(page,&AT45_buffer[0]);
				page++;
				k=0;
			}
			data=AT45_buffer[k];
			for(a=0;a<8;a++)			
			{			
				if(!((data<<a)&0x80))
				{
					if(j>ypos-40)LCD_SetPoint(i+a,j,GRAY[16]);//��ɫ����ͼ��
					else LCD_SetPoint(i+a,j,0xD71A);//����ͼ��							   
				}
				else LCD_SetPoint(i+a,j,WHITE);	
			}
			k++;		
		}
	}
} 
//�������
void Set_scroll(u8 mode)
{
	u16 i,ypos,ymax;
	
	ymax=ypos=scrpos-20;
	while(ypos>20||PEN_int==RESET)
	{
		if(PEN_int==RESET)
		{
			ADS_SPI_Init();
			Read_Y(&PENY);	
			ypos=PENY;
			if(ypos<20)ypos=20;
			if(ypos>100)ypos=100;
			FLASH_SPI_Init();
			Draw_scroll(ypos+20);
		}
		else
		{	
			if(ypos>60)
			{	
				while(ypos<100)//�Զ�����
				{
					if(ypos==99)ypos++;
					else ypos+=2;
					Draw_scroll(ypos+20);
				}
			}
			else//�Զ�����
			{
			 	ypos=Scroll_back(ypos,mode);
			}
			break;
		}
		if(ypos<ymax)//�ػ汳��
		{
			for(i=ypos;i<ymax;i++)Draw_BGDline(i+20,mode);
		}
		else ymax=ypos;				
	}
	if(scrpos==120)
	{
		Set_eico(basex2);
		eicocur=8-(basex2+240)/60;
		Show_Fonts(85,90,(u8*)Icotext[icolist[eicocur]],BLACK);
	}		
}
//�Զ�����
u16 Scroll_back(u16 ypos,u8 mode)
{
	u16 i,ymax;

	while(ypos>20)//�Զ�����
	{
		ymax=ypos;
		if(ypos==21)ypos--;
		else ypos-=2;
		Draw_scroll(ypos+20);
		for(i=ypos;i<ymax;i++)Draw_BGDline(i+20,mode);		
	}
	return ypos;
}
/****************************************************************************
							æ�ȴ�����
****************************************************************************/
//�ڻ������л�͸����,ע��y<80
void Trans_WinPoint2(u16 x,u8 y,u16 fcolor,u8 Trans)//
{
	u16 r1=0,g1=0,b1=0,r2=0,g2=0,b2=0;
	u16 color;

	r2=((fcolor>>11)&0x1F)*Trans/100;
	g2=((fcolor>>5)&0x3F)*Trans/100;
	b2=(fcolor&0x1F)*Trans/100;

	color = gui_buf[100*(y-150)+(x-70)];//����		
	r1 = ((color>>11)&0x1F)*(100-Trans)/100;//����͸���Ȳ����Ըõ������Ⱦ
	g1 = ((color>>5)&0x3F)*(100-Trans)/100;
	b1 = (color&0x1F)*(100-Trans)/100;
	color = ((r1+r2)<<11)+((g1+g2)<<5)+(b1+b2);//͸��
	LCD_SetPoint(x,y,color);//����
}
//������
void Trans_WINLine(u16 x1, u16 y, u16 x2 ,u16 color,u8 trans)
{
	u16 i;
	for(i=x1;i<x2;i++)Trans_WinPoint2(i,y,color,trans);
}
//�ӻ������л�͸�����Բ
void Trans_WINCircle(u16 x0, u16 y0, u16 r,u16 color,u8 trans)
{
  	u16 i,x=r;
  	u16 imax=(r*707)/1000+1;
  	u16 sqmax=r*r+r/2;

  	Trans_WINLine(x0-r,y0,x0+r,color,trans);
  	for(i=1;i<=imax;i++)
	{
    	if((i*i+x*x)>sqmax)
		{
      		if(x>imax) 
			{
        		Trans_WINLine (x0-i+1,y0+x, x0+i-1,color,trans);
        		Trans_WINLine (x0-i+1,y0-x, x0+i-1,color,trans);
      		}
      		x--;
    	}
    	Trans_WINLine(x0-x,y0+i, x0+x,color,trans);
    	Trans_WINLine(x0-x,y0-i, x0+x,color,trans);
  	}
}
void Draw_busy(u16 time)
{
	u16 i,j,k,color;
	u32 cycle;

	//LCD_Window(0,0,239,319);
	//LCD_Charmd(0,WHITE);
	for(j=150,k=0;j<250;j++)//æ�ȴ�ͼ�������ݴ���������
	{
		for(i=70;i<170;i++,k++)
		{
			color=LCD_GetPoint(i,j);
			gui_buf[k]=color;
		}	
	}
	for(i=0;i<time;i++)//ѭ��һ��תһȦ
	{
		cycle=0x87654321;//1000 0111 0110 0101 0100 0011 0010 0001
		for(j=0;j<8;j++) 
		{						
			cycle=(cycle<<4)+((cycle&0xf0000000)>>28);//ʵ��ѭ����λ
			Trans_WINCircle(120,160,8,WHITE,((cycle&0x0000000f)>>0)*12);
			Trans_WINCircle(148,171,8,WHITE,((cycle&0x000000f0)>>4)*12);	 
			Trans_WINCircle(159,199,8,WHITE,((cycle&0x00000f00)>>8)*12);		 
			Trans_WINCircle(148,227,8,WHITE,((cycle&0x0000f000)>>12)*12);		 
			Trans_WINCircle(120,238,8,WHITE,((cycle&0x000f0000)>>16)*12);	 
			Trans_WINCircle( 92,227,8,WHITE,((cycle&0x00f00000)>>20)*12); 		 
			Trans_WINCircle( 81,199,8,WHITE,((cycle&0x0f000000)>>24)*12); 		 
			Trans_WINCircle( 92,171,8,WHITE,((cycle&0xf0000000)>>28)*12); 		 
			delay_ms(100);
		}
	}	
	for(j=150,k=0;j<250;j++)//�����������ݻָ�����ʾ����
	{
		for(i=70;i<170;i++,k++)
		{
			color=gui_buf[k];
			LCD_SetPoint(i,j,color);
		}	
	}		
}
/****************************************************************************
							ͼ���ƶ�����
****************************************************************************/
int NX,NY=320;//��ǰͼ�������,�����긳�ɲ����ܵ�ֵ
u16 OX,OY;//��ȥͼ�������
u16* oldbuf;
u16* newbuf;
//��ȡ������Ļ��ĳһ�㣬���õ��ڱ����ػ滺���У�����ж�ȡ�õ�
u16 Get_bkpoint(u16 x,u16 y)
{
	//���ھɻ�������
	if(x>=OX&&x<(OX+bufwidth)&&y>=OY&&y<(OY+bufheight))return oldbuf[bufwidth*(y-OY)+(x-OX)];
	else return LCD_GetPoint(x,y);	
}
//���㣬���õ���ͼ���������У��򲻽��л���
void Set_bkpoint(u16 x,u16 y,u16 color)
{
	//������������
	if(x>=NX&&x<(NX+bufwidth)&&y>=NY&&y<(NY+bufheight))return;
	else if(x<240&&y<320)LCD_SetPoint(x,y,color);
}
//�ı䱳��ĳһ�㣬���õ���ͼ���������У���ӱ��������д��
void Change_bkpoint(u16 x,u16 y,u16 color)
{
	//������������
	if(x>=NX&&x<(NX+bufwidth)&&y>=NY&&y<(NY+bufheight))newbuf[bufwidth*(y-NY)+(x-NX)]=color;
	else if(x<240&&y<320)LCD_SetPoint(x,y,color);
}

u16 myabs(int value)
{
	if(value>0)return value;
	else return -value;
}
//��ָ�������µ�Memwidth*Memheight�������򱣴���������
void Fill_buff(void)
{
	u16 i,j,k;

	for(j=0,k=0;j<bufheight;j++)
	{
		for(i=0;i<bufwidth;i++,k++)
		{
			newbuf[k]=Get_bkpoint(NX+i,NY+j);
		}	
	}
}
//�ػ汳��
void Draw_buff(void)
{
	u16 i,j,k;

	for(j=0,k=0;j<bufheight;j++)
	{
		for(i=0;i<bufwidth;i++,k++)
		{
			Set_bkpoint(OX+i,OY+j,oldbuf[k]);
		}	
	}
}
//��ͼ�걣����������
void Buf_icon(u16* buf,u8 iconum)
{
	u16	i,k,page,color;

	k=528;
	page=Icoaddr[iconum];
	for(i=0;i<3600;i++)
	{
		if(k==528)
		{
			FlashPageRead(page,&AT45_buffer[0]);
			page++;
			k=0;
		}
		color=(AT45_buffer[k+1]<<8)+AT45_buffer[k];
		buf[i]=color;
		k+=2;					
	}
}
//���ƻ����е�ͼ��,mode1:��͸����ɫ,mode0:�޵�ɫ
void Draw_menbuf(int x,u16 y,u16* buf,u8 mode)
{
	int refx,refy;
	u16	i,j,fcolor,bcolor;
	u16 r1=0,g1=0,b1=0,r2=0,g2=0,b2=0;

	for(j=0;j<bufheight;j++)
	{
		refy=y+j;
		if(refy>319||refy<0)continue;
		for(i=0;i<bufwidth;i++)
		{
			refx=x+i;
			if(refx>239||refx<0)continue;
			fcolor=buf[bufwidth*j+i];
			bcolor = newbuf[bufwidth*j+i];
			if(!mode&&!fcolor)LCD_SetPoint(refx,refy,bcolor);
			else//��͸���� 
			{				
				r2=((fcolor>>11)&0x1F)*2/3;
				g2=((fcolor>>5)&0x3F)*2/3;
				b2=(fcolor&0x1F)*2/3;			
				r1 = ((bcolor>>11)&0x1F)/3;
				g1 = ((bcolor>>5)&0x3F)/3;
				b1 = (bcolor&0x1F)/3;
				fcolor = ((r1+r2)<<11)+((g1+g2)<<5)+(b1+b2);
				LCD_SetPoint(refx,refy,fcolor);//����
			}									
		}
	}
}
//ֱ������Ļ�ϻ���ͼ��,bcolorΪ͸����ɫ
void Draw_icon(int x,u16 y,u8 iconum,u16 bcolor)
{
	u16	j,k,page,color,refy;
	int	i,refx;

	k=528;
	refx=x+60;
	refy=y+60;
	page=Icoaddr[iconum];
	for(j=y;j<refy;j++)
	for(i=x;i<refx;i++)
	{
		if(k==528)
		{
			FlashPageRead(page,&AT45_buffer[0]);
			page++;
			k=0;
		}
		if(i>=30&&i<210)//��֤��������ʾ����
		{
			color=(AT45_buffer[k+1]<<8)+AT45_buffer[k];
			if(color==WHITE)color=bcolor;
			Change_bkpoint(i,j,color);
		}
		k+=2;					
	}	
}
//�ƶ�ͼ��
void Move_ICO(u8 iconum)
{
	u8 which;

	OX=240;//���ɲ����ܵ�ֵ
	OY=320;
	which=0;
	bufwidth=60;
    bufheight=60;
	Buf_icon(gui_buf,iconum);
	oldbuf=gui_buf+3600;
	newbuf=gui_buf+7200;
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
		if(NY>260)NY=260;
		Fill_buff();//�ڻ���ͼ��֮ǰ���������
		Draw_menbuf(NX,NY,gui_buf,1);
		Draw_buff();//��س���ǰͼ����������ı���
		FLASH_SPI_Init();
		if(NY<47&&NX>60&&NX<120)
		{
			Draw_icon(90,17,18,WHITE);							
		}
		else Draw_icon(90,17,icolist[eicocur],WHITE);
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
	if(NY<47&&NX>60&&NX<120)
	{
		icolist[eicocur]=iconum;
		Save_param();
	}
	NX=240;//���ɲ����ܵ�ֵ
	NY=320;
	Draw_buff();
	Draw_icon(90,17,icolist[eicocur],WHITE);
	bufwidth=240;
    bufheight=80;
}

/*****************************************************************************
								����С����
*****************************************************************************/
void Draw_pagepointer(void)
{
	if(basex1==0)
	{
		Fill_Circle(100,135,4,BLACK);
		Fill_Circle(120,135,4,WHITE);
		Fill_Circle(140,135,4,BLACK);
		screen=1;
	}
	else if(basex1==240)
	{
		Fill_Circle(100,135,4,WHITE);
		Fill_Circle(120,135,4,BLACK);
		Fill_Circle(140,135,4,BLACK);
		screen=0;
	}
	else if(basex1==-240)
	{
		Fill_Circle(100,135,4,BLACK);
		Fill_Circle(120,135,4,BLACK);
		Fill_Circle(140,135,4,WHITE);
		screen=2;
	}
}

//����ϵͳ����
//��0~9�ֽ�:���ͼ����
void Save_param(void)
{
	FlashPageWrite(PAR_addr,&Sysparam[0]);	
}
//��ȡϵͳ����
void Load_param(void)
{
	FlashPageRead(PAR_addr,&Sysparam[0]);
}

u8 Draw_bmp(u16 xpos,u16 ypos,u8* path)
{
	FIL fileR;
	UINT Bytes;
	u32 i;
	u16 k;
	BITMAPFILEHEADER* bmph;
	BITMAPINFOHEADER* infoh;

	if(f_open(&fileR,(TCHAR*)path,FA_READ))return 1;
	f_read(&fileR,AT45_buffer,54,&Bytes);//��ȡ54�ֽ��ļ�ͷ
	bmph=(BITMAPFILEHEADER*)AT45_buffer;//��ȡBMPͷ
	infoh=(BITMAPINFOHEADER*)(AT45_buffer+14);//��ȡBMP��Ϣͷ
	LCD_Window(xpos,ypos,xpos+infoh->biWidth-1,ypos+infoh->biHeight-1);
	LCD_Cursor(xpos,ypos);
	LCD_REG=0x22;
	f_lseek(&fileR,bmph->bfOffBytes);//ֱ������ͼ������
	k=1536;
	for(i=0;i<infoh->biSizeImage;i+=3)
	{		
		if(k==1536)
		{
			f_read(&fileR,&work_buf[0],1536,&Bytes);//��һ�����ݵ�����
			k=0;
		}
		LCD_RAM=RGB888toRGB565(work_buf[k],work_buf[k+1],work_buf[k+2]);
		k+=3;
	} 
	f_close(&fileR);
	LCD_Window(0,0,239,319);
	return 0;	
}

void Trans_Fill(u16 x1,u16 y1,u16 x2,u16 y2,u16 color,u8 trans)
{
	u16 i,j;

	for(j=y1;j<y2;j++)
	{
		for(i=x1;i<x2;i++)TransPoint(i,j,color,trans);
	}	
}

void Trans_HLine(u16 x1,u16 y,u16 x2,u16 color,u8 trans)
{
	u16 i;
	for(i=x1;i<x2;i++)TransPoint(i,y,color,trans);
}

void Trans_VLine(u16 y1,u16 x,u16 y2,u16 color,u8 trans)
{
	u16 i;
	for(i=y1;i<y2;i++)TransPoint(x,i,color,trans);
}

void Trans_SFill(u16 x1,u16 y1,u16 x2,u16 y2,u16 color,u8 trans1,u8 trans2)
{
	u16 j;
	int trans;

	for(j=y1;j<y2;j++)
	{
		trans=trans2-trans1;
		if(trans>0)trans=trans1+(trans)*(j-y1)/(y2-y1);
		else trans=trans2+(-trans+trans*(j-y1)/(y2-y1)); 
		Trans_HLine(x1,j,x2,color,trans);
	}	
}			
//��͸��������  direc 1:����;-1����
void Trans_triangle(u16 x,u16 y,u8 size,u16 color,u8 trans,int direc)
{
	u8 i;

	size=size/2;
	for(i=0;i<size;i+=3)
	{
		Trans_VLine(y-size+i,x,y+size-i,color,trans);
		Trans_VLine(y-size+i+1,x+direc*1,y+size-i-1,color,trans);
		Trans_VLine(y-size+i+1,x+direc*2,y+size-i-1,color,trans);
		Trans_VLine(y-size+i+2,x+direc*3,y+size-i-2,color,trans);
		Trans_VLine(y-size+i+2,x+direc*4,y+size-i-2,color,trans);
		x=x+direc*5;
	}
}

/*****************************************************************************
								���湦�ܺ���
*****************************************************************************/
//�����ʼ��
void GDGUI_start(void)
{
	if(!Get_Key(KEY1))Update_patterns();//������סKEY1������ϵͳͼƬ
	if(!Get_Key(KEY2))UpdateFont("0:/SYSTEM/FONT/st16.bin");//������סKEY2�������ֿ�
	//work_buf=mem_malloc(40960);//�洢�豸	
	work_buf=malloc(40960);//�洢�豸
	gui_buf=(u16*)work_buf;
	LCD_Clear(BLACK);
	Draw_mono(40,50,160,160,GDL_addr,WHITE,50);
	Draw_busy(2);
	Load_lockpage();//����ҳ��	
	Home_page();
}
//��ҳ��
void Home_page(void)
{
	u16 times;	
	int xpos,ypos,temp;

	xpos=0;
	basex1=0;
	basex2=0;
	screen=1;
	scrpos=40;
	appcur=100;
	bufheight=80;
	FLASH_SPI_Init();
	Dyna_bground();	
	Set_mico(0);
	Draw_pagepointer();
	Draw_scroll(40);
	while(1)
	{
		if(PEN_int==RESET)
		{						    
			ADS_SPI_Init();
			Read_X(&PENX);
			Read_Y(&PENY);
			xpos=PENX;
			ypos=PENY;
			if(ypos<40)
			{
				if(xpos<48)
				{
					Ch_BTcolor(8,6,46,33,RED);
					while(PEN_int==RESET);
					Draw_scroll(40);
				}
				else if(xpos>192)
				{
					Ch_BTcolor(195,6,240,33,RED);
					while(PEN_int==RESET);
					Draw_scroll(40);
				}
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
				if(ypos>120&&scrpos==120)continue;
				else Click_ico(xpos,ypos,1);					  	
			}
			else
			{								
				if(times==500&&scrpos==120)//��������
				{
					Copy_ico(xpos,ypos);					
				}
				else//��������
				{
					if(ypos<(scrpos-10)&&ypos>(scrpos-30))Set_scroll(1);//��������
					else if(ypos>17&&ypos<77&&xpos>10&&xpos<191&&scrpos==120)//������ݲ˵�
					{
						Slip_eico(xpos);
					}					
					else if(ypos>150)//�������˵�
					{
						Slip_mico(xpos);
					}
				}
			}			
		}
	}
}
//������ͼ����ƺ���
void Slip_mico(int xpos)
{	
	basex1-=xpos;
	while(PEN_int==RESET)
	{	
		ADS_SPI_Init();
		Read_X(&PENX);
		xpos=basex1+PENX;
		if(xpos>360)xpos=360;
		if(xpos<-360)xpos=-360;
		FLASH_SPI_Init();
		Set_mico(xpos);
	}
	basex1=MICO_slipback(xpos);
	Draw_pagepointer();

}
//���ͼ����ƺ���
void Slip_eico(int xpos)
{
	basex2-=xpos;
	LCD_Charmd(1,WHITE);
	while(PEN_int==RESET)
	{	
		ADS_SPI_Init();
		Read_X(&PENX);
		xpos=basex2+PENX;
		if(xpos>240)xpos=240;
		if(xpos<-240)xpos=-240;
		FLASH_SPI_Init();
		Set_eico(xpos);
	}
	basex2=EICO_slipback(xpos);
	eicocur=8-(basex2+240)/60;
	Show_Fonts(85,90,(u8*)Icotext[icolist[eicocur]],BLACK);
}
//����ͼ����ƺ���
void Copy_ico(u16 xpos,u16 ypos)
{
	if(ypos>150&&ypos<210)//��һ��
	{
		if(xpos>20&&xpos<80)Move_ICO(screen*6);
		else if(xpos>90&&xpos<150)Move_ICO(screen*6+1);//ͼ��2
		else if(xpos>60&&xpos<220)Move_ICO(screen*6+2);//ͼ��3
	}
	else if(ypos>235&&ypos<295)//�ڶ���
	{
		if(xpos>20&&xpos<80)Move_ICO(screen*6+3);
		else if(xpos>90&&xpos<150)Move_ICO(screen*6+4);
		else if(xpos>60&&xpos<220)Move_ICO(screen*6+5);
	}
}
//���ͼ�����
void Click_ico(u16 xpos,u16 ypos,u8 mode)
{
	FLASH_SPI_Init();
	if(ypos>150&&ypos<210&&mode)//��һ��
	{
		if(xpos>20&&xpos<80)
		{
			ICO_Select(20,150,screen*6);//ͼ��1
			Select_app(screen*6);
		}
		else if(xpos>90&&xpos<150)//ͼ��2
		{
			ICO_Select(90,150,screen*6+1);
			Select_app(screen*6+1);
		}
		else if(xpos>160&&xpos<220)//ͼ��3
		{						
			ICO_Select(160,150,screen*6+2);
			Select_app(screen*6+2);
		}
		Set_mico(basex1);
	}
	else if(ypos>235&&ypos<295&&mode)//�ڶ���
	{
		if(xpos>20&&xpos<80)
		{						
			ICO_Select(20,235,screen*6+3);//ͼ��4
			Select_app(screen*6+3);
		}
		else if(xpos>90&&xpos<150)
		{
			ICO_Select(90,235,screen*6+4);//ͼ��5
			Select_app(screen*6+4);
		}
		else if(xpos>160&&xpos<220)
		{
			ICO_Select(160,235,screen*6+5);//ͼ��6
			Select_app(screen*6+5);
		}
		Set_mico(basex1);
	}
	else if(ypos>17&&ypos<77&&scrpos==120)//���ͼ��
	{
		if(xpos>30&&xpos<90)
		{						
			ICO_Select(30,17,icolist[eicocur-1]);//ͼ��4
			scrpos=Scroll_back(scrpos-20,mode)+20;
			Select_app(icolist[eicocur-1]);
		}
		else if(xpos>90&&xpos<150)
		{
			ICO_Select(90,17,icolist[eicocur]);//ͼ��5
			scrpos=Scroll_back(scrpos-20,mode)+20;
			Select_app(icolist[eicocur]);
		}
		else if(xpos>150&&xpos<210)
		{
			ICO_Select(150,17,icolist[eicocur+1]);//ͼ��6
			scrpos=Scroll_back(scrpos-20,mode)+20;
			Select_app(icolist[eicocur+1]);
		}
    }	
}
//�ı䰴ť��ɫ(��������Ϊ��ɫ�ĵ��Ϊcolor)
void Ch_BTcolor(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
	u16 i,j,temp;

	for(j=y1;j<y2;j++)
		for(i=x1;i<x2;i++)
		{
			temp=LCD_GetPoint(i,j);//����
			if(temp!=WHITE)LCD_SetPoint(i,j,color);//����
		}
}
//���Ṧ��
u8 Scroll_ctrl(void(*finishfun)(void))
{
	int temp;
	u16 xpos,ypos,i,j,k;

	if(PEN_int==RESET)
	{
		Read_X(&PENX);
		xpos=PENX;
		if(xpos<48)//���·�����ҳ�水ť
		{
			Ch_BTcolor(8,6,46,33,RED);
			while(PEN_int==RESET);
			finishfun();//������ǰӦ�ò��ͷ���Դ
			Draw_scroll(40);
			Home_page();	
		}
		else if(xpos<192)
		{		
			for(j=0,k=0;j<80;j++)//�ݴ��40~120�еı���
			{
				for(i=0;i<240;i++,k++)
				{
					gui_buf[k]=LCD_GetPoint(i,40+j);
				}	
			}
			Set_scroll(0);
			while(scrpos==120)
			{
				if(PEN_int==RESET)
				{						    
					ADS_SPI_Init();
					Read_X(&PENX);
					Read_Y(&PENY);	
					xpos=PENX;
					ypos=PENY;			
					while(1)
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
						if(ypos>17&&ypos<77&&xpos>30&&xpos<210)
						{
							finishfun();
							Click_ico(xpos,ypos,0);
						}					  	
					}
					else
					{
						if(ypos<(scrpos-10)&&ypos>(scrpos-30))Set_scroll(0);//��������
						else if(ypos>17&&ypos<77&&xpos>10&&xpos<191&&scrpos==120)//������ݲ˵�
						{
							Slip_eico(xpos);
						}										 	
					}
				}
			}				
		}
		else 
		{
			Ch_BTcolor(195,6,240,33,RED);
			while(PEN_int==RESET);
			Draw_scroll(40);
			return 1;//���·��ذ�ť
		}	
	}
	return 0;//�޲���
}

//Ӧ�÷�֧��ת
void Select_app(u8 iconum)
{
	if(iconum==appcur)
	{
		Dialog_box(40,180,"Ӧ�ó����Ѿ�������!");
		delay_ms(1200);
		Del_Dialog();
		return;
	}
	switch(iconum)
	{
	 	case 0:break;
		case 1:appcur=1;App_ebook();break;
		case 2:break;
		case 3:break;
		case 4:appcur=4;App_game1();break;
		case 5:appcur=5;App_game2();break;
		case 6:break;
		case 7:break;
		case 8:appcur=8;App_album();break;
		case 9:appcur=9;App_camera();break;
	 	case 10:break;
		case 11:appcur=11;App_music();break;
		case 12:break;
		case 13:break;
		case 14:break;
		case 15:break;
		case 16:appcur=16;App_fmanager();break;
		case 17:break;
		default:break;
	}
}  

								

