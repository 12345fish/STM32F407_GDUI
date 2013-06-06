#ifndef __EBOOK_H__
#define __EBOOK_H__

#include "stm32f4xx_conf.h"

#define EBKPGSize	4928//һҳֽ����ģ��С
#define EBKRDSize	308 //һҳֽ��������


typedef struct
{
	FIL  file;//�������ļ���
	DIR  dir;//�������ļ�·��
	u32  hdlen;//ͷ��Ϣ����
	int  basex;//��ͼX���ַ
	int  basey;//��ͼY���ַ
	UINT bytes;//���ļ��ֽ���	
	u16  filecnt;//�ı��ļ���
	u16  filecur;//��ǰ���ļ�
	u8   plctrl;//���ſ���,��һλ���š���ͣ���ڶ�λ��һ��������λ��һ��
	u8   pagecur;//��ǰҳ
	u8   buf_sw;//�����л�����
	u8   page_buf[EBKRDSize];//ҳ����
}ebkdef;//�������õ�����Դ

void App_ebook(void);
void Change_buf(u8 direction);
void Fill_paper(u8* buf);
void Show_Vfonts(int xpos,u8 num,u8* Fonts);
void Slip_paper(int xpos);
int  Stick_paper(int xpos);
void Finish_ebook(void);


#endif


