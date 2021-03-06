#ifndef __GAME1_H__
#define __GAME1_H__

#include "stm32f4xx_conf.h"

static const unsigned char Image_ball[1800] = {
0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,
0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X56,0X9D,0XEF,0X7B,0X8E,0X73,0X6D,0X6B,
0XF0,0X83,0X56,0X9D,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,
0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,
0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0XEF,0X63,
0X8A,0X3A,0X86,0X29,0XC5,0X41,0XC1,0X20,0X68,0X62,0X2B,0X7B,0X2B,0X73,0X07,0X4A,
0X44,0X21,0X18,0XAE,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,
0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,
0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X56,0X9D,0X0C,0X63,0X00,0X00,0XC9,0X62,
0XE9,0X6A,0X2C,0X94,0X6E,0XAC,0XE8,0X7A,0XCB,0X8B,0X07,0X6B,0XC9,0X6A,0X44,0X31,
0X25,0X31,0XEF,0X63,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,
0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,
0X30,0X74,0X54,0X9D,0X82,0X10,0X20,0X10,0XC9,0X6A,0X8C,0X83,0X0D,0X94,0X0E,0X8C,
0X70,0X9C,0X50,0X9C,0X0E,0X94,0XCD,0X93,0XCC,0X8B,0XCA,0X83,0X06,0X5A,0X8C,0X73,
0X17,0XAE,0XEB,0X5A,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,
0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X0C,0X43,0X8A,0X52,0X00,0X00,
0X23,0X29,0XC5,0X41,0X53,0XB5,0XDA,0XDE,0X7D,0XF7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X9E,0XF7,0X5C,0XF7,0XB5,0XBD,0XEE,0X7B,0X0A,0X73,0X64,0X41,0XAA,0X5A,0X14,0X95,
0XC7,0X21,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,
0X00,0XF8,0X00,0XF8,0X85,0X11,0X40,0X08,0X00,0X00,0X07,0X42,0XEB,0X6A,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XFE,0XFF,0XFE,0XFF,
0XFF,0XFF,0XFF,0XFF,0X17,0XCE,0X27,0X52,0X40,0X10,0X24,0X29,0XB1,0X9C,0X69,0X32,
0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0XD6,0XA5,
0X85,0X31,0X61,0X18,0X47,0X5A,0X48,0X52,0XD3,0X9C,0XFF,0XFF,0XBE,0XFF,0XDE,0XFF,
0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XDE,0XFF,0X9D,0XF7,
0XDE,0XFF,0XFA,0XE6,0X06,0X4A,0X81,0X28,0X2B,0X63,0X0B,0X63,0X92,0X84,0X00,0XF8,
0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0XBA,0XD6,0XD2,0X94,0X00,0X00,0X48,0X5A,
0X09,0X7B,0X06,0X42,0XBA,0XD6,0X9E,0XF7,0X9E,0XF7,0XFF,0XFF,0XFE,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XBE,0XFF,0XBD,0XF7,0XFF,0XFF,
0XF6,0XC5,0XA5,0X51,0XA2,0X20,0X6D,0X73,0X6D,0X63,0X00,0XF8,0X00,0XF8,0X00,0XF8,
0X00,0XF8,0X00,0XF8,0XBA,0XD6,0X85,0X31,0X81,0X10,0X8C,0X83,0X2A,0X7B,0XC4,0X31,
0X3C,0XE7,0XBE,0XFF,0X9D,0XF7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDE,0XFF,0XDE,0XFF,0XFF,0XFF,0XFA,0XD6,0X05,0X52,
0X85,0X51,0X82,0X18,0X55,0XAD,0XB5,0XB5,0X00,0XF8,0X00,0XF8,0X00,0XF8,0XBA,0XD6,
0X74,0XAD,0X64,0X31,0X81,0X18,0X4B,0X83,0X4A,0X7B,0X25,0X4A,0XFB,0XDE,0XDE,0XFF,
0X9D,0XF7,0XDE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFE,0XFF,0XDF,0XFF,0X7C,0XEF,0X46,0X62,0XA8,0X72,0X84,0X39,
0XB2,0X9C,0XD6,0XA5,0X00,0XF8,0X00,0XF8,0X00,0XF8,0XBA,0XD6,0X50,0X8C,0XC2,0X18,
0XC4,0X49,0XED,0X9B,0X4A,0X6B,0X06,0X4A,0X59,0XD6,0X9D,0XF7,0X9D,0XF7,0XBE,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFE,0XFF,0XDF,0XFF,0X5C,0XEF,0XA7,0X6A,0X88,0X72,0X29,0X73,0XAD,0X7B,0XF3,0XA4,
0XB5,0XB5,0X00,0XF8,0X00,0XF8,0XBA,0XD6,0XAD,0X73,0X00,0X00,0X62,0X41,0X4A,0X7B,
0X2A,0X73,0XC9,0X62,0X55,0XAD,0X9D,0XEF,0XBE,0XF7,0XDE,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDF,0XFF,0XFE,0XFF,0XDE,0XFF,0XDF,0XFF,
0X5B,0XEF,0X2A,0X6B,0XE6,0X49,0XAD,0X83,0X27,0X4A,0XAA,0X62,0XB5,0XB5,0X00,0XF8,
0X79,0XD6,0XBA,0XD6,0XAA,0X52,0X00,0X00,0X49,0X73,0XEC,0X93,0XC9,0X72,0XAD,0X83,
0X6D,0X73,0XFF,0XFF,0XDE,0XFF,0XFF,0XFF,0XFF,0XFF,0XBF,0XFF,0XD7,0XBD,0X9E,0XF7,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDE,0XFF,0X9D,0XFF,0XFE,0XFF,
0X6F,0X94,0XF9,0XE6,0XD1,0XA4,0X00,0X08,0X14,0X95,0XB5,0XB5,0X79,0XD6,0XBA,0XD6,
0X6C,0X6B,0X00,0X00,0X66,0X5A,0X66,0X5A,0X26,0X62,0X8C,0X83,0XA9,0X5A,0X5C,0XEF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X0B,0X6B,0X89,0X5A,0X90,0X9C,0XCE,0X83,0XD6,0XC5,
0XFF,0XFF,0XBE,0XF7,0X7D,0XF7,0X1B,0XE7,0XFF,0XFF,0X38,0XC6,0X77,0XD6,0X3B,0XEF,
0XEB,0X62,0X61,0X10,0X13,0XA5,0X14,0X95,0XDB,0XDE,0XBA,0XD6,0X0F,0X84,0X00,0X00,
0XC4,0X41,0XE8,0X72,0XEC,0X9B,0X6B,0X7B,0X2B,0X6B,0X34,0XAD,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XA1,0X38,0X03,0X39,0X48,0X62,0X03,0X41,0X28,0X62,0XFF,0XFF,0X1B,0XDF,
0X65,0X21,0X44,0X29,0X13,0XA5,0XA2,0X10,0X50,0X8C,0X79,0XD6,0XA6,0X39,0XC7,0X39,
0X13,0XAD,0X14,0X95,0XFB,0XDE,0XBA,0XD6,0XB9,0XDE,0X04,0X21,0X40,0X10,0X84,0X41,
0X03,0X29,0X02,0X31,0X8B,0X7B,0X47,0X52,0X5D,0XEF,0XDE,0XFF,0XA9,0X52,0X00,0X28,
0X81,0X10,0X41,0X10,0X21,0X20,0XA3,0X28,0X28,0X52,0XC7,0X31,0XC6,0X31,0X00,0X00,
0X00,0X00,0X61,0X08,0X44,0X29,0X07,0X42,0X00,0X08,0X40,0X10,0X92,0XA4,0X14,0X95,
0X99,0XDE,0XBA,0XD6,0XBA,0XDE,0X8D,0X73,0X00,0X00,0XA2,0X18,0X00,0X00,0X24,0X31,
0X07,0X73,0X0A,0X6B,0XE3,0X18,0X24,0X21,0X03,0X19,0X20,0X08,0X00,0X00,0X20,0X00,
0X00,0X00,0XC1,0X10,0X61,0X10,0X00,0X08,0X20,0X08,0X40,0X00,0X00,0X00,0X64,0X29,
0XC3,0X18,0X00,0X00,0X25,0X29,0X44,0X31,0X0F,0X8C,0X14,0X95,0X79,0XD6,0XBA,0XD6,
0X99,0XDE,0XFA,0XDE,0X04,0X21,0XE5,0X18,0X00,0X00,0XA2,0X18,0XC9,0X72,0XA6,0X41,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X08,0X00,0X00,0X00,0X00,0X20,0X00,
0X00,0X00,0X20,0X00,0X41,0X08,0X40,0X00,0X00,0X00,0X48,0X4A,0X4C,0X6B,0X81,0X10,
0X30,0X8C,0X4C,0X73,0X8E,0X6B,0XB5,0XB5,0XBA,0XD6,0XBA,0XD6,0XBA,0XDE,0XBA,0XDE,
0X38,0XCE,0XA2,0X10,0X00,0X00,0XC3,0X20,0XC7,0X49,0X20,0X08,0X00,0X00,0X20,0X00,
0X82,0X10,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X20,0X08,0X00,0X00,0X00,0X00,0XF3,0X9C,0X3C,0XEF,0X50,0X8C,0X8D,0X7B,0X65,0X31,
0X14,0X95,0X00,0XF8,0X00,0XF8,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0XFB,0XE6,0X34,0XA5,
0X00,0X00,0X00,0X00,0XA2,0X20,0X00,0X00,0X00,0X00,0X20,0X00,0X82,0X10,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X71,0X8C,0X5C,0XEF,0X1B,0XE7,0X58,0XCE,0X07,0X42,0XE6,0X39,0XB5,0XB5,0X00,0XF8,
0X00,0XF8,0XBA,0XD6,0XFB,0XD6,0XBA,0XD6,0XB9,0XD6,0XDA,0XDE,0X17,0XC6,0XC6,0X39,
0X00,0X00,0X00,0X00,0X20,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XC7,0X39,0X58,0XCE,0X5C,0XEF,0XB9,0XD6,
0XBA,0XD6,0XF7,0XBD,0XEE,0X7B,0XB5,0XB5,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,
0XBA,0XD6,0XBA,0XD6,0XB9,0XD6,0XBA,0XD6,0XDA,0XDE,0XDA,0XDE,0X91,0X94,0XC3,0X18,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X86,0X31,0XD3,0X9C,0XDA,0XDE,0XDA,0XDE,0XDA,0XDE,0XDA,0XDE,0X79,0XCE,0XF7,0XBD,
0XB5,0XB5,0X14,0X95,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0XBA,0XD6,0XBA,0XD6,
0XBA,0XD6,0X99,0XD6,0X99,0XD6,0XBA,0XD6,0X1B,0XE7,0XDA,0XDE,0X95,0XB5,0X70,0X94,
0X8D,0X73,0X6C,0X6B,0X0B,0X63,0XEE,0X7B,0XD2,0X9C,0X58,0XCE,0X1B,0XE7,0XFB,0XDE,
0XBA,0XD6,0XDA,0XDE,0XDA,0XDE,0XBA,0XD6,0X79,0XCE,0X58,0XCE,0XBA,0XCE,0X00,0XF8,
0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0XBA,0XD6,0XDA,0XD6,0XBA,0XD6,
0XBA,0XD6,0XBA,0XD6,0X99,0XD6,0X99,0XD6,0XB9,0XDE,0XD9,0XDE,0XD9,0XDE,0XFB,0XE6,
0XFB,0XE6,0XDA,0XDE,0XDA,0XDE,0XDA,0XDE,0XBA,0XDE,0XBA,0XD6,0XDA,0XDE,0XDA,0XDE,
0XDA,0XDE,0X99,0XD6,0X99,0XD6,0XBA,0XD6,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,
0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0XDE,
0XB9,0XD6,0X99,0XD6,0XBA,0XD6,0XBA,0XD6,0XB9,0XD6,0XBA,0XDE,0XBA,0XDE,0XBA,0XDE,
0XBA,0XDE,0XBA,0XDE,0XBA,0XDE,0XDA,0XDE,0XDA,0XDE,0XDA,0XDE,0XDA,0XDE,0XDA,0XDE,
0XBA,0XD6,0XBA,0XD6,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,
0X00,0XF8,0X00,0XF8,0X00,0XF8,0XBA,0XD6,0X9A,0XD6,0XBA,0XDE,0X9A,0XD6,0XBA,0XD6,
0XDA,0XDE,0XDA,0XDE,0XBA,0XDE,0XDA,0XDE,0XDA,0XDE,0XDA,0XDE,0XDA,0XDE,0XDA,0XDE,
0XBA,0XDE,0XDA,0XDE,0XDA,0XDE,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0X00,0XF8,
0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,
0X00,0XF8,0X00,0XF8,0XBA,0XD6,0XBA,0XD6,0X99,0XD6,0X99,0XD6,0XDA,0XD6,0XB9,0XD6,
0X9A,0XDE,0XBA,0XDE,0XDA,0XDE,0XBA,0XDE,0X9A,0XD6,0XBA,0XD6,0XDA,0XDE,0XDA,0XDE,
0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,
0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,
0X00,0XF8,0X00,0XF8,0XBA,0XD6,0X99,0XCE,0X99,0XD6,0X79,0XCE,0X99,0XD6,0X99,0XD6,
0X99,0XD6,0X99,0XD6,0X99,0XD6,0X99,0XD6,0X99,0XCE,0XBA,0XD6,0XBA,0XD6,0XBA,0XD6,
0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,
0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,
0X00,0XF8,0X00,0XF8,0X79,0XB6,0X79,0XC6,0X38,0XCE,0X58,0XCE,0X58,0XCE,0X79,0XD6,
0X58,0XCE,0X38,0XCE,0X58,0XBE,0X18,0XAE,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,
0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,
0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,
0X00,0XF8,0X00,0XF8,0X13,0X95,0XD3,0X9C,0XB2,0X9C,0X92,0X9C,0X92,0X94,0X92,0X84,
0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8,
0X00,0XF8,0X00,0XF8,0X00,0XF8,0X00,0XF8};


void App_game1(void);
void Draw_pattern(u16 x,u16 y,u16 width,u16 height,u16* pic,u16 Tcolor,u8 trans);
void Set_ball(u8 trans);
void Check_hole(u16 x,u16 y);
void Finish_game1(void);

#endif


