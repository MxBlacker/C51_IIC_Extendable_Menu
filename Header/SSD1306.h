#ifndef __OLED_H
#define __OLED_H

#include <stdint.h>
#include <REGX52.H>

#define OLED_BUFFER_MODE	0
#define Max_Column			128
#define Max_Row				64
#define	Brightness			0xFF

#define OLED_ADDRESS 0x3C	//7位地址
//设备地址

#define OLED_WriteCom_Addr	0x00	//从机写指令地址
#define OLED_WriteData_Addr	0x40	//从机写数据地址

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

/* OLED控制用函数 */
void OLED_Clear(void);																																											 /* OLED清屏 */
void OLED_Display_On(void);                                                         /* OLED开 */
void OLED_ShowPosture(void);																																								 /* 提示信息 */
void OLED_Display_Off(void);                                                        /* OLED关 */
void OLED_Init(void);                                                               /* OLED初始化 */
void OLED_WR_Byte(uint8_t dat, uint8_t cmd);	                                    /* 写字节 */
void OLED_Set_Pos(uint8_t x, uint8_t y);											/* 设置坐标 */
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t charSize, uint8_t Is_Reverse);            /* 显示字符 */
void OLED_ShowString(uint8_t x, uint8_t y, char *p, uint8_t charSize, uint8_t Is_Reverse);	            /* 显示字符串 */
void OLED_ShowNum(uint8_t x, uint8_t y, int32_t num, uint8_t len, uint8_t Size, uint8_t Is_Reverse);   /* 显示数字 */
void OLED_ShowFloat(uint8_t x, uint8_t y, int32_t value, uint8_t Is_Reverse);
void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[], uint8_t Is_Reverse);	/* 显示图片 */

void OLED_Refresh_Gram(void);

#endif  /* __OLED_H */
