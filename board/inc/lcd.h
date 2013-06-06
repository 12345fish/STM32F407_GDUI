#ifndef __LCD_H
#define __LCD_H

#include "stm32f4xx_conf.h"
#include "fonts.h"
 
#define WHITE         	 	0xFFFF
#define BLACK         	 	0x0000	  
#define BLUE         	 	0x001F  
#define BRED             	0XF81F
#define GRED 			 	0XFFE0
#define GBLUE			 	0X07FF
#define RED           	 	0xF800
#define MAGENTA       	 	0xF81F
#define GREEN         	 	0x07E0
#define CYAN          	 	0x7FFF
#define YELLOW        	 	0xFFE0
#define BROWN 			 	0XBC40 	//��ɫ
#define BRRED 			 	0XFC07 	//�غ�ɫ
//#define GRAY  			 	0X8430 	//��ɫ
#define DARKBLUE      	 	0X01CF 	//����ɫ
#define LIGHTBLUE      	 	0X7D7C 	//ǳ��ɫ  
#define GRAYBLUE       	 	0X5458 	//����ɫ

#define	Fense				0xe9ce	//��ɫ
#define Chase			  	0xd5b1	//��ɫ
#define Hese		     	0xa285	//��ɫ
#define Tianlanse		 	0x05ff	//����ɫ
#define Jialanse		  	0x435c	//�ʼ���
#define Bohese			   	0xf5ff	//����ɫ
#define Huise			   	0x0000	//��ɫ
#define Yinbaise			0xc618	//����ɫ 
#define Qianhuise		 	0x8410	//ǳ��ɫ
#define Chengse			  	0xfbea	//��ɫ


//32�ҽ�
static const u16 GRAY[33]={0x0000,0x0841,0x1082,0x18C3,0x2104,0x2945,0x3186,0x39C7,0x4208,
							 0x4A49,0x528A,0x5ACB,0x630C,0x6B4D,0x738E,0x7BCF,0x8410,0x8C51,
					       	 0x9492,0x9CD3,0xA514,0xAD55,0xB596,0xBDD7,0xC618,0xCE59,0xD69A,
							 0xDEDB,0xE71C,0xEF5D,0xF79E,0xFFDF,0xFFFF};


#define LCD_RAM   *(vu16*)((u32)0x60020000)  //disp Data ADDR
#define LCD_REG   *(vu16*)((u32)0x60000000)	 //disp Reg  ADDR

#define LCD_WIDTH	  240
#define LCD_HEIGHT	  320


#define RGB565toRGB555(RGB) 	((RGB&0xF800)>>1)+((RGB&0x7C0)>>1)+(RGB&0x1f)
#define BGR565toRGB555(BGR) 	((BGR&0x1f)<<10)+((BGR&0x7C0)>>1)+((BGR&0xF800)>>11)
#define BGR565toRGB565(BGR) 	((BGR&0x1f)<<11)+(BGR&0x7e0)+((BGR&0xF800)>>11)
#define RGB888toRGB565(R,G,B)  	(R>>3)+((G&0xfc)<<3)+((B&0xf8)<<8)


extern u16 Bcolor;
extern u8 Charmd;
extern u8 Fsize;
extern u16 Llimit;
extern u16 Rlimit;

void LCD_Init(void);
void LCD_Font(u8 font);
void LCD_Dispmd(u8 mode);
void LCD_Charmd(u8 mode,u16 color);
void LCD_Cursor(u16 Xpos, u16 Ypos);
void LCD_Window(u16 Xpos, u16 Ypos, u16 Height, u16 Width);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);

void LCD_Clear(u16 Color);
void LCD_SetPoint(u16 xpos, u16 ypos,u16 color);
void Draw_Bigpiont(u16 x,u16 y,u16 color);
u16  LCD_GetPoint(u16 x,u16 y);
void LCD_Line(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void LCD_Rect(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void LCD_Circle(u16 Xpos, u16 Ypos, u16 Radius, u16 color);
void LCD_Fill(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void LCD_HLine(u16 x1, u16 y, u16 x2 ,u16 color);
void LCD_VLine(u16 y1, u16 x, u16 y2 ,u16 color);
void Fill_Circle(u16 x0, u16 y0, u16 r,u16 color);

void LCD_Char(int x,u16 y,u8 num,u16 color);
void LCD_String(u16 x,u16 y,const u8 *p,u16 color);
u32  mypow(u8 m,u8 n);
void LCD_Num(u16 x,u16 y,u32 num,u8 len,u16 color);
void Draw_Bmp(u16 x,u16 y,u16 width,u16 height,u16* pic);
void TransPoint(u16 x,u16 y,u16 Color,u8 Trans);
void TransWhite(u16 x,u16 y,u8 Trans);
void TransBlack(u16 x,u16 y,u8 trans);

void LCD_LineConf(void);
void LCD_FSMCConf(void);
void LCD_WriteReg(u8 Reg, u16 Value);
u16  LCD_ReadReg(u8 Reg);
void LCD_WriteRAM(u16 RGB_Code);
u16 LCD_ReadRAM(void);
void Disply3D(float xpos,float ypos,float zpos,u16 color,u16 color_back,u8 width,u8 speed);


#endif
