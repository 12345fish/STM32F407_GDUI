#include "arm_math.h"
#include "MemPool.h"
#include "stdlib.h"
#include "at45db.h"
#include "cs4334.h"
#include "fsearch.h"
#include "music.h"
#include "gdgui.h"
#include "delay.h"
#include "touch.h"
#include "lcd.h"
#include "spi.h"

wavdef* wav;//wav�ļ��ṹ��
musdef* mus;//���ֲ������ṹ��
u8*  	mus_buf1;
u8*  	mus_buf2;//�����õ�˫����
int* 	FFTinbuf;//Ƶ�׷����õĻ���
int* 	FFToutbuf;
void App_music(void)
{
	u8  flag;
	u16 times;
	int xpos,ypos;		
	//Ӧ�ó��������õ����ڴ�ռ�����ö�̬���䷽ʽ
//	wav=mem_malloc(sizeof(wavdef));
//	if(wav==NULL){Finish_music();return;}
//	mus=mem_malloc(sizeof(musdef));
//	if(mus==NULL){Finish_music();return;}
//	mus_buf1=mem_malloc(RDSize*sizeof(u8));
//	if(mus_buf1==NULL){Finish_music();return;}
//	mus_buf2=mem_malloc(RDSize*sizeof(u8));
//	if(mus_buf2==NULL){Finish_music();return;}
//	FFTinbuf=mem_malloc(FFTSize*sizeof(int));
//	if(FFTinbuf==NULL){Finish_music();return;}
//	FFToutbuf=mem_malloc(FFTSize*sizeof(int));
//	if(FFToutbuf==NULL){Finish_music();return;}
	wav=malloc(sizeof(wavdef));
	if(wav==NULL){Finish_music();return;}
	mus=malloc(sizeof(musdef));
	if(mus==NULL){Finish_music();return;}
	mus_buf1=malloc(RDSize*sizeof(u8));
	if(mus_buf1==NULL){Finish_music();return;}
	mus_buf2=malloc(RDSize*sizeof(u8));
	if(mus_buf2==NULL){Finish_music();return;}
	FFTinbuf=malloc(FFTSize*sizeof(int));
	if(FFTinbuf==NULL){Finish_music();return;}
	FFToutbuf=malloc(FFTSize*sizeof(int));
	if(FFToutbuf==NULL){Finish_music();return;}


	flag=0;
	mus->PCMcnt=0;
	mus->basex=0;
	mus->basey=0;
	mus->filecur=0;
	mus->aud_pl=0;
	mus->plctrl=0;
	mus->viewcur=1;
	Load_bground();
	Draw_scroll(40);
	if(f_opendir(&mus->dir,"0:/MUSIC"))
	{
		Dialog_box(40,180,"�������Ч��SD��������λϵͳ.");
		delay_ms(1200);
		Del_Dialog();
		Home_page();
	}
	mus->filecnt=File_search("0:/MUSIC",T_AUDIO);//�ܻ�������С������,���֧��50���ļ�	
	//�������
	FFT_init();//��ʼ������Ҷ�任
	Play_view(0,0);//������ͼ
	Info_board();//������Ϣ��
	Play_ctrl(mus->plctrl);//���ſ�����
	ADS_SPI_Init();	
	while(1)
	{
		if(flag)//����Ƶ����ʾ
		{
			FLASH_SPI_Init();
			Play_view(mus->basex,mus->basey);//������ͼ
			ADS_SPI_Init();
			flag=0;
		}
		else//���²�����Ϣ
		{
			FLASH_SPI_Init();
			Info_board();
			if(mus->plctrl)
			{
				Play_ctrl(mus->plctrl);//���ſ��ư�ť
				delay_ms(300);
				mus->plctrl=0;
				Play_ctrl(mus->plctrl);
			}
			ADS_SPI_Init();
			flag=1;
		}
		if(PEN_int==RESET)
		{
	  		Read_X(&PENX);
			Read_Y(&PENY);
			xpos=PENX;
			ypos=PENY;
			if(ypos<40)
			{
				Scroll_ctrl(Finish_music);
				ADS_SPI_Init();
			}
	  		else if(ypos>270&&ypos<310)//���ſ���
			{
				if(xpos>30&&xpos<90)mus->plctrl=0x02;//��һ����ť
				else if(xpos>90&&xpos<150)mus->plctrl=0x01;//���Ű�ť
				else if(xpos>150&&xpos<210)mus->plctrl=0x04;//��һ����ť								
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
				if(ypos>44&&ypos<176&&mus->viewcur==1)//ѡ���б���
				{
					if(ypos<72){mus->filecur=-mus->basey/26;Trans_SFill(0,45,240,77,WHITE,100,0);}
					else if(ypos<98){mus->filecur=1-mus->basey/26;Trans_SFill(0,67,240,103,WHITE,100,0);}
					else if(ypos<124){mus->filecur=2-mus->basey/26;Trans_SFill(0,93,240,129,WHITE,100,0);}
					else if(ypos<150){mus->filecur=3-mus->basey/26;Trans_SFill(0,119,240,155,WHITE,100,0);}
					else {mus->filecur=4-mus->basey/26;Trans_SFill(0,145,240,175,WHITE,100,0);}
					FLASH_SPI_Init();
					if(mus->aud_pl)//�����ڲ��ţ���ֹͣ
					{
						mus->aud_pl=0;
						f_close(&mus->file);
					}
					delay_ms(300);
					mus->plctrl=0x01;					
					Musicplay(namebuf+mus->filecur*13);					
					ADS_SPI_Init();						
				}
				else if(ypos>225&&ypos<245&&xpos>52&&xpos<188)//���Ž��ȿ���
				{
					mus->PCMcnt=((wav->DataSize>>10)*(xpos-52)/136)<<10;//����������
					if(mus->PCMcnt%2)mus->PCMcnt++;//16λ����Ƶ,�ֽ�����Ϊż
					mus->aud_pl=0;					
					f_lseek(&mus->file,mus->hdlen+mus->PCMcnt);//����ָ��λ��
					delay_ms(300);
					mus->aud_pl=1;
					AUDIO_TransferComplete();//��������			
				}												
			}
			else//��������
			{
				if(times==600){}//��������
				else//��������
				{
					if(ypos>40&&ypos<180)
					{
						if(xpos<30||xpos>210)//�л���ͼ
						{
							mus->basex-=xpos;
							while(PEN_int==RESET)
							{						
								ADS_SPI_Init();
								Read_X(&PENX);
								xpos=mus->basex+PENX;
								if(xpos<-240)xpos=-240;
								if(xpos>240)xpos=240;
								FLASH_SPI_Init();
								Play_view(xpos,mus->basey);
							}
							mus->basex=Stick_view(xpos,mus->basey);
							ADS_SPI_Init();						
						}
						else if(mus->viewcur==1)//�����б�
						{
							mus->basey-=ypos;
							while(PEN_int==RESET)
							{							
								ADS_SPI_Init();
								Read_Y(&PENY);
								ypos=mus->basey+PENY;
								if(ypos>0)ypos=0;//ֻ�����ϻ���,��ַ������С��0
								if(ypos<(130-26*mus->filecnt))ypos=130-26*mus->filecnt;
								FLASH_SPI_Init();
								Play_view(0,ypos);//��ʾ�б�							
							}
							mus->basey=Stick_Mlist(ypos);
							ADS_SPI_Init();
						}
					}
				}
			}
		}	
	}
}
/************************************************************************************
							     ���������溯��
************************************************************************************/
//���ƽ�����
void Draw_prg(u16 x1,u16 y1,u16 x2,u16 y2,u8 prg)
{
	Win_HTLine(x1+2,y1,x2-1,WHITE,80);	
	Win_HTLine(x1+2,y2,x2-1,WHITE,80);
	Win_TransPoint(x1+1,y1+1,WHITE,80);
	Win_TransPoint(x1+1,y2-1,WHITE,80);
	Win_TransPoint(x2-1,y1+1,WHITE,80);
	Win_TransPoint(x2-1,y2-1,WHITE,80);
	Win_VTLine(y1+2,x1,y2-1,WHITE,80);
	Win_VTLine(y1+2,x2,y2-1,WHITE,80);
	Win_Fill(x1+2,y1+1,x1+2+prg,y2,BLUE,50);
}
//����Ƶ����,xpos:X���ַ,num:Ƶ������,extent:�߶�,which:�����
void Draw_column(int xpos,u16 num,u16 extent,u8 which)
{
	u8 i,j;
	int pos;

	pos=xpos+25+num*6;
	if(pos<-5)return;
	if(which)which=130;
	else which=65;
	for(i=0;i<5;i++)
	{
		if(pos+i<0)continue;	
		for(j=0;j<=extent;j++)Win_TransPoint(pos+i,which-j,WHITE,60);
	}	
}
//��ʾ�����б�
void Play_view(int xpos,int ypos)
{
	u16 i;
	int temp;

	bufheight=65;
	Fill_back(45,110);//����ϰ�ر���
	for(i=0;i<mus->filecnt;i++)//�ļ��б���ʾ
	{		
		if(ypos>(-26-26*i)&&ypos<(70-26*i))//����ʾ��Χ�ڲŸ�����ʾ
		{
			temp=ypos+i*26+5;//temp=ypos+i*26+50-45;
			if(xpos>-34&&xpos<230)Load_type(xpos+10,temp-4,T_AUDIO);//��ʾ����ͼ��
			if(xpos>-150&&xpos<200)Win_Fonts(xpos+40,temp,namebuf+i*13,WHITE);//��ʾ�ļ���			
			if(xpos>-230&&xpos<60)Win_Fonts(xpos+196,temp,".  M",WHITE);
		    if(xpos>-222&&xpos<60)Win_Num(xpos+180,temp,sizebuf[i]/1048576,2,WHITE);//��ʾ�ļ���С��������
			if(xpos>-200&&xpos<40)Win_Num(xpos+204,temp,(sizebuf[i]%1048576)/10240,2,WHITE);//��ʾ�ļ���СС������
		}					
	}
	if(mus->FillOK&&xpos>0)//����Ƶ���ϰ��
	{
		My_FFT(&FFTinbuf[0],&FFToutbuf[0]);
	    for(i=0;i<FFTSize;i++)
	    {
	      	FFTinbuf[i]>>=21;    	
	    } 
	    for(i=0;i<FFTSize/4;i++)
	    {
			if(FFTinbuf[i]>130)FFTinbuf[i]=130;      	
			if(FFTinbuf[i]>=65)Draw_column(xpos-240,i,FFTinbuf[i],1);
	    }
	}
	Dip_guibuf(45,110);//��ʾ�ϰ������
	Fill_back(110,175);//����°�ر���		   
	for(i=0;i<mus->filecnt;i++)//�ļ��б���ʾ
	{		
		if(ypos>(44-26*i)&&ypos<(140-26*i))//����ʾ��Χ�ڲŸ�����ʾ
		{
			temp=ypos+i*26-60;//temp=ypos+i*26+50-110;
			if(xpos>-34&&xpos<230)Load_type(xpos+10,temp-4,T_AUDIO);//��ʾ����ͼ��
			if(xpos>-150&&xpos<200)Win_Fonts(xpos+40,temp,namebuf+i*13,WHITE);//��ʾ�ļ���			
			if(xpos>-230&&xpos<60)Win_Fonts(xpos+196,temp,".  M",WHITE);
		    if(xpos>-222&&xpos<60)Win_Num(xpos+180,temp,sizebuf[i]/1048576,2,WHITE);//��ʾ�ļ���С��������
			if(xpos>-200&&xpos<40)Win_Num(xpos+204,temp,(sizebuf[i]%1048576)/10240,2,WHITE);//��ʾ�ļ���СС������
		}					
	}
	if(mus->FillOK&&xpos>0)//����Ƶ���°��
	{
	    for(i=0;i<FFTSize/4;i++)
	    {     	
			if(FFTinbuf[i]>65)FFTinbuf[i]=65;
			Draw_column(xpos-240,i,FFTinbuf[i],0);
	    }
		mus->FillOK=0;
	}
	Dip_guibuf(110,175);//��ʾ�°������
} 
//��ͼ��λ
int Stick_view(int xpos,int ypos)
{
	int temp;
	
	if(xpos>=120)//���Ҷ�λ
	{
		temp=240-xpos;
		while(xpos<240)
		{			
			temp/=2;
			xpos=240-temp;
			Play_view(xpos,ypos);
		}
		mus->viewcur=0;
	}
	else if(xpos<120&&xpos>0)//����λ
	{
		while(xpos>0)
		{			
			xpos/=2;
			Play_view(xpos,ypos);
		}
		mus->viewcur=1;
	}
	else if(xpos<0&&xpos>-120)//���Ҷ�λ
	{
		while(xpos<0)
		{			
			xpos/=2;
			Play_view(xpos,ypos);
		}
		mus->viewcur=1;
	}
	else if(xpos<=-120)//����λ
	{
		temp=-240-xpos;
		while(xpos>-240)
		{			
			temp/=2;
			xpos=-240-temp;
			Play_view(xpos,ypos);
		}
		mus->viewcur=2;
	}
	return xpos;
}
//�б�λ
int Stick_Mlist(int ypos)
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
			Play_view(0,ypos);
		}
	}
	else//���¶�λ
	{
		temp=temp*26;
		while(ypos<temp)
		{
			if(ypos<temp-1)ypos+=2;
			else ypos++;
			Play_view(0,ypos);
		}		
	}
	return ypos;	
}
//��ʾʱ��
void Show_time(u16 x,u16 y,u32 second)
{
	u8 hour,min,sec;

	if(second>3600)
	{
		hour=second/3600;
		Win_Num(x,y,hour,2,WHITE);
		Win_Char(x+16,y,':',WHITE);
		x+=16;
	}
	min=(second%3600)/60;	
	sec=second%60;
	Win_Num(x,y,min,2,WHITE);
	Win_Char(x+16,y,':',WHITE);
	Win_Num(x+24,y,sec,2,WHITE);
}
//������Ϣ��
void Info_board(void)
{
	bufheight=80;
	Fill_back(180,260);//��䱳��
	Win_Fill(0,0,240,80,BLACK,30);
	Win_Fonts(70,20,namebuf+mus->filecur*13,WHITE);//��ʾ��ǰ�����ļ���
	Draw_prg(50,50,190,60,136*(mus->PCMcnt>>10)/(wav->DataSize>>10));//������
	Show_time(195,47,mus->pltime);
	Show_time(5,47,mus->pltime*(mus->PCMcnt>>10)/(wav->DataSize>>10));	
	Dip_guibuf(180,260);//��ʾ����
}
//���ſ�����
void Play_ctrl(u8 ctrl)
{
	u16 i;
	u8 trans;

	bufheight=60;
	Fill_back(260,320);//��䱳��
	for(i=0;i<240;i++)
	{
		trans=100-abs(80-i*2/3);
		Win_TransPoint(i,0,WHITE,trans);
	}
	Win_SFill(0,0,240,32,WHITE,50,20);
	Win_HTLine(0,32,240,WHITE,10);
	Win_HTLine(0,33,240,BLACK,10);
	Win_SFill(0,34,240,60,BLACK,20,50);
	if(ctrl&0x01)
	{
		if(mus->aud_pl)//��ͣ
		{
			mus->aud_pl=0;
			Load_type(108,15,T_PLAY);
		}
		else//����
		{
			mus->aud_pl=1;
			Load_type(108,15,T_PAUSE);
  			AUDIO_TransferComplete();						
		}	
	}
	else
	{
		if(mus->aud_pl)Load_type(108,15,T_PAUSE);
		else Load_type(108,15,T_PLAY);	
	}
	if(ctrl&0x02){Load_type(25,15,T_PREV);mus->aud_pl=0;delay_ms(300);Play_prev();mus->aud_pl=1;}//��һ��
	else Load_type(35,15,T_PREV);	
	if(ctrl&0x04){Load_type(190,15,T_NEXT);mus->aud_pl=0;delay_ms(300);Play_next();mus->aud_pl=1;}//��һ��
	else Load_type(180,15,T_NEXT);	
	Dip_guibuf(260,320);//��ʾ����		
}







/************************************************************************************
							     ���������ܺ���
************************************************************************************/
//���ֲ���
void Musicplay(u8* fname)
{
	u8* suffix;	

	My_chdir("0:/MUSIC");//�л��������ļ���
	Add_path(path_curr,fname);
	suffix=fname+Get_suff(fname);//�ж��ļ���ʽ
	if(*suffix=='w'||*suffix=='W')Wavplay(path_curr);//wav��Ƶ�ļ�
	else if(*suffix=='m'||*suffix=='M')Mp3play(path_curr);//mp3��Ƶ�ļ�
	else 
	{
		Dialog_box(40,180,"��ʽ��֧��!");
		delay_ms(1200);
		Del_Dialog();	
	}	
}
//WAV��Ƶ����
void Wavplay(u8* fname)
{            
  	f_open(&mus->file,(TCHAR*)fname,FA_READ);
	f_read(&mus->file,mus_buf1,RDSize,&mus->bytes);
	if(WaveParsing((u8*)mus_buf1))
	{
		Dialog_box(40,180,"�ļ���������!");
		delay_ms(1200);
		Del_Dialog();
		return;	
	}
	mus->pltime=wav->DataSize/wav->ByteRate;//���㲥��ʱ��
  	AUDIO_Init(wav->SampleRate);
  	f_lseek(&mus->file,mus->hdlen);//�����ļ�ͷ
  	f_read(&mus->file,mus_buf1,RDSize,&mus->bytes); 
  	f_read(&mus->file,mus_buf2,RDSize,&mus->bytes);
	mus->PCMcnt=2*RDSize;
  	Audio_MAL_Play((u32)mus_buf1,RDSize);
  	mus->buf_sw=1;
}
//����WAV�ļ�ͷ
u8 WaveParsing(u8* buf)
{
  	u32 temp=0x00;
	u32 DataOffset=0;
  	u32 exbytes=0;	  

  	temp=ReadUnit(buf,0,4,BigEndian);//��'RIFF'
  	if(temp!=CHUNK_ID)return 1;
  	wav->RIFFchunksize=ReadUnit(buf,4,4,LittleEndian);//���ļ�����
  	temp=ReadUnit(buf,8,4,BigEndian);//��'WAVE'
  	if(temp!=FILE_FORMAT)return 2;
  	temp=ReadUnit(buf,12,4,BigEndian);//��'fmt '
  	if(temp!=FORMAT_ID)return 3;
  	temp=ReadUnit(buf,16,4,LittleEndian);//��'fmt'���ݳ���
  	if(temp!=0x10)exbytes=1;
  	wav->FormatTag=ReadUnit(buf,20,2,LittleEndian);//����Ƶ��ʽ
  	if(wav->FormatTag!=WAVE_FORMAT_PCM)return 4;  
  	wav->NumChannels=ReadUnit(buf,22,2,LittleEndian);//�������� 
	wav->SampleRate=ReadUnit(buf,24,4,LittleEndian);//��������
	wav->ByteRate=ReadUnit(buf,28,4,LittleEndian);//��������
	wav->BlockAlign=ReadUnit(buf,32,2,LittleEndian);//�������
	wav->BitsPerSample=ReadUnit(buf,34,2,LittleEndian);//��������λ��
	if(wav->BitsPerSample!=BITS_PER_SAMPLE_16)return 5;
	DataOffset=36;
	if(exbytes==1)
	{
		temp=ReadUnit(buf,36,2,LittleEndian);//�������ʽ�ֽ�
		if(temp!=0x00)return 6;
		temp=ReadUnit(buf,38,4,BigEndian);//��'fact'
		if(temp!=FACT_ID)return 7;
		temp=ReadUnit(buf,42,4,LittleEndian);//��Fact���ݴ�С
		DataOffset+=10+temp;
	}
	temp=ReadUnit(buf,DataOffset,4,BigEndian);//��'data'
	DataOffset+=4;
	if(temp!=DATA_ID)return 8;
	wav->DataSize=ReadUnit(buf,DataOffset,4,LittleEndian);//���������ݴ�С
	DataOffset+=4;
	mus->hdlen=DataOffset;
	return 0;
}
//MP3��Ƶ����
void Mp3play(u8* fname)
{
	Dialog_box(40,180,"���ڲ���MP3�ļ�.");
	delay_ms(1200);
	Del_Dialog();
}
//������һ��
void Play_next(void)
{
	f_close(&mus->file);
	if(mus->filecur<mus->filecnt)
	{
		mus->filecur++;
		Musicplay(namebuf+mus->filecur*13);
	}
}
//������һ��
void Play_prev(void)
{
	f_close(&mus->file);
	if(mus->filecur>0)
	{
		mus->filecur--;
		Musicplay(namebuf+mus->filecur*13);
	}
}
//������ɻص�����
void AUDIO_TransferComplete(void)
{  
	u16 i;
	u16* tempbuf;

	if(!mus->aud_pl)return;
	if(mus->buf_sw)
    {   
 		Audio_MAL_Play((u32)mus_buf2,RDSize);//��buffer2����
     	f_read(&mus->file,mus_buf1,RDSize,&mus->bytes);//���buffer1
		mus->PCMcnt+=mus->bytes;
		if(mus->bytes<RDSize)
		{
			mus->aud_pl=0;
			mus->plctrl=0x04;//Play_next();
		}		
    	mus->buf_sw=0;		
		tempbuf=(u16*)mus_buf1;
	} 
   	else 
    {
    	Audio_MAL_Play((u32)mus_buf1,RDSize);//��buffer1����
     	f_read(&mus->file,mus_buf2,RDSize,&mus->bytes);//���buffer2
		mus->PCMcnt+=mus->bytes;
		if(mus->bytes<RDSize)
		{
			mus->aud_pl=0;
			mus->plctrl=0x04;//Play_next();
		}	 		
    	mus->buf_sw=1;		
		tempbuf=(u16*)mus_buf2;
  	}
	if(!mus->FillOK)
	{
		for(i=0;i<FFTSize;i++)FFTinbuf[i]=tempbuf[i*4]<<16;
		mus->FillOK=1;
	}
}
//����Ҷ�任��ʼ��
static arm_rfft_instance_q31 S;
static arm_cfft_radix4_instance_q31 S_CFFT;
void FFT_init(void)
{
	arm_status status;

	status=arm_rfft_init_q31(&S,&S_CFFT,FFTSize,0,1);//���任��־Ϊ0,λ��ת��־Ϊ1
	if(status!=ARM_MATH_SUCCESS)
	{
		Dialog_box(40,180,"����Ҷ�任��ʼ������");
		delay_ms(1200);
		Del_Dialog();
	}
}
//����Ҷ�任
void My_FFT(int* inbuf,int* outbuf)
{
	arm_rfft_q31(&S,inbuf,outbuf); 
	arm_cmplx_mag_q31(outbuf,inbuf,FFTSize);
}
//��ȡ�������ָ����Ԫ
u32 ReadUnit(u8 *buffer,u8 index,u8 Bytes,Endianness Format)
{
  	u8 off=0;
  	u32 temp=0;
  
  	for(off=0;off<Bytes;off++)temp|=buffer[off+index]<<(off*8);
  	if (Format==BigEndian)temp=__REV(temp);
  	return temp;
}

////��������
//void Finish_music(void)
//{
//	mus->aud_pl=0;
//	f_close(&mus->file);
//	delay_ms(300);
//	//�ͷ��ڴ�
//	mem_free(wav);
//	mem_free(mus);				   
//	mem_free(mus_buf1);
//	mem_free(mus_buf2);
//	mem_free(FFTinbuf);
//	mem_free(FFToutbuf);
//}

//��������
void Finish_music(void)
{
	mus->aud_pl=0;
	f_close(&mus->file);
	delay_ms(300);
	//�ͷ��ڴ�
	free(wav);
	free(mus);				   
	free(mus_buf1);
	free(mus_buf2);
	free(FFTinbuf);
	free(FFToutbuf);
}

