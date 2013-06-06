#ifndef __MUSIC_H
#define __MUSIC_H

#include <stm32f4xx.h>
#include "ff.h"


typedef enum
{
  LittleEndian,
  BigEndian
}Endianness;

typedef struct
{
  u32  RIFFchunksize;
  u16  FormatTag;
  u16  NumChannels;
  u32  SampleRate;
  u32  ByteRate;
  u16  BlockAlign;
  u16  BitsPerSample;
  u32  DataSize;
}wavdef;//wavͷ��Ϣ

typedef struct
{
	FIL  file;//�����ļ���
	DIR  dir;//�����ļ�·��
	u32  hdlen;//ͷ��Ϣ����
	u32  PCMcnt;//WAV���ݼ���
	u32  pltime;//����ʱ��
	int  basex;//��ͼX���ַ
	int  basey;//��ͼY���ַ
	UINT bytes;//���ļ��ֽ���	
	u16  filecnt;//�����ļ���
	u16  filecur;//��ǰ���ŵ������ļ�
	u8   FillOK;//�����ɱ�־
	u8   aud_pl;//����ֹͣ��־
	u8   plctrl;//���ſ���,��һλ���š���ͣ���ڶ�λ��һ��������λ��һ��
	u8   viewcur;//��ǰ��ͼ
	u8   buf_sw;//�����л�����
}musdef;//���ֲ������õ�����Դ



#define  CHUNK_ID                            0x52494646//'RIFF'
#define  FILE_FORMAT                         0x57415645//'WAVE'
#define  FORMAT_ID                           0x666D7420//'fmt ' 
#define  DATA_ID                             0x64617461//'data' 
#define  FACT_ID                             0x66616374//'fact' 
#define  WAVE_FORMAT_PCM                     0x01
#define  FORMAT_CHNUK_SIZE                   0x10
#define  CHANNEL_MONO                        0x01
#define  CHANNEL_STEREO                      0x02
#define  SAMPLE_RATE_8000                    8000
#define  SAMPLE_RATE_11025                   11025
#define  SAMPLE_RATE_22050                   22050
#define  SAMPLE_RATE_44100                   44100
#define  BITS_PER_SAMPLE_8                   8
#define  BITS_PER_SAMPLE_16                  16


#define	FFTSize  128
#define	RDSize   1024

void App_music(void);
void Draw_prg(u16 x1,u16 y1,u16 x2,u16 y2,u8 prg);
void Draw_column(int xpos,u16 num,u16 extent,u8 which);
void Play_view(int xpos,int ypos);
int  Stick_view(int xpos,int ypos);
int  Stick_Mlist(int ypos);
void Show_time(u16 x,u16 y,u32 second);
void Info_board(void);
void Play_ctrl(u8 ctrl);

void Musicplay(u8* fname);
void Wavplay(u8* fname);
u8   WaveParsing(u8* buf);
void Play_next(void);
void Play_prev(void);
void Finish_music(void);
void AUDIO_TransferComplete(void);
void Mp3play(u8* fname);
void FFT_init(void);
void My_FFT(int* inbuf,int* outbuf);
u32  ReadUnit(u8 *buffer,u8 index,u8 Bytes,Endianness Format);





#endif

