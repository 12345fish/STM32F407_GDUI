#ifndef __CAMERA_H
#define __CAMERA_H

#include <stm32f4xx.h>

																	
#define RGB565toRGB555(RGB) ((RGB&0xF800)>>1)+((RGB&0x7C0)>>1)+(RGB&0x1f)
#define BGR565toRGB555(BGR) ((BGR&0x1f)<<10)+((BGR&0x7C0)>>1)+((BGR&0xF800)>>11)


typedef struct
{
	FIL  file;//��Ƭ�ļ���
	DIR  dir;//��Ƭ�ļ�·��
	u32  hdlen;//ͷ��Ϣ����
	int  basex;//��ͼX���ַ
	int  basey;//��ͼY���ַ
	UINT bytes;//���ļ��ֽ���	
	u16  filecnt;//�ı��ļ���
	u16  filecur;//��ǰ���ļ�
	u8   plctrl;//���ſ���
	u8   pagecur;//��ǰҳ
	u8   buf_sw;//�����л�����
	u16  line_buf[240];//�л���
}camdef;//����õ�����Դ

void App_camera(void);
void Finish_camera(void);

#endif

