#include "SSD1306.h"
#include "Soft_I2C.h"
#include "Font.h"
#include <stdlib.h>

void delay_ms(uint16_t ms);

//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127

#if OLED_BUFFER_MODE

uint8_t xdata OLED_GRAM[Max_Row / 8][Max_Column] = {0};

#endif

/**************************************************************************
 Function: Refresh the OLED screen
 Input   : none
 Output  : none
 函数功能：刷新OLED屏幕
 入口参数：无
 返回  值：无
 **************************************************************************/
void OLED_Refresh_Gram(void)
{
#if OLED_BUFFER_MODE
    uint16_t i;
    OLED_Set_Pos(0, 128);
    for (i = 0; i < Max_Row / 8 * Max_Column; i++)
    {
        OLED_WR_Byte(OLED_GRAM[0][i], OLED_DATA);
    }
#endif
}

/**
* @brief 向SSD1306写入一个字节
* @param dat:要写入的数据/命令 cmd:数据/命令标志 0,表示命令; 1,表示数据;
* @return None
*/
void OLED_WR_Byte(uint8_t dat, uint8_t cmd)
{
	if (cmd)
		I2C_mem_write(OLED_ADDRESS, OLED_WriteData_Addr, &dat, 1);
	else
		I2C_mem_write(OLED_ADDRESS, OLED_WriteCom_Addr, &dat, 1);
}

/**************************************************************************
 Function: Set the coordinates (position) displayed on the screen.
 Input   : x, y: starting point coordinates
 Output  : none
 函数功能：
 入口参数: x,y :起点坐标
 返回  值：无
 **************************************************************************/
void OLED_Set_Pos(uint8_t x, uint8_t y)
{
    OLED_WR_Byte(0xb0 + y, OLED_CMD);					 //设置纵坐标，0xb0是基础页地址，+y表示0~7页
    OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);	 //设置横坐标高四位。或上0x10，这是设置高4位列地址的命令标志
    OLED_WR_Byte((x & 0x0f), OLED_CMD);					 //设置横坐标低四位
}

/**
* @brief 开启OLED显示
* @param None
* @retval None
*/
void OLED_Display_On(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD);  //SET DCDC命令
    OLED_WR_Byte(0X14, OLED_CMD);  //DCDC ON
    OLED_WR_Byte(0XAF, OLED_CMD);  //DISPLAY ON
}

/**
* @brief 关闭OLED显示
* @param None
* @retval None
*/
void OLED_Display_Off(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD);  //SET DCDC命令
    OLED_WR_Byte(0X10, OLED_CMD);  //DCDC OFF
    OLED_WR_Byte(0XAE, OLED_CMD);  //DISPLAY OFF
}

/**
* @brief 清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样
* @param None
* @retval None
*/
void OLED_Clear(void)
{
#if OLED_BUFFER_MODE
    uint16_t i;
    for (i = 0; i < Max_Row / 8 * Max_Column; i++)
    {
        OLED_GRAM[0][i] = 0;
    }
    OLED_Refresh_Gram(); //Update the display //更新显示
#else
    uint8_t i, n;
    for (i = 0; i<8; i++)
    {
        OLED_WR_Byte(0xb0 + i, OLED_CMD);    //设置页地址（0~7）
        OLED_WR_Byte(0x00, OLED_CMD);      //设置显示位置—列低地址
        OLED_WR_Byte(0x10, OLED_CMD);      //设置显示位置—列高地址
        for (n = 0; n<Max_Column; n++)
            OLED_WR_Byte(0, OLED_DATA);
    }//更新显示
#endif
}

/**
* @brief 在指定位置显示一个字符,包括部分字符
* @param x:0~127   y:0~63   Is_Reverse:1,反白显示  0,正常显示	 charSize:选择字体 16/6
* @retval None
*/
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t charSize, uint8_t Is_Reverse)
{
    uint8_t i = 0;
    chr -= ' ';	//得到偏移后的值
    if (x > Max_Column - 1)
    {
        x = 0;
        y = y + 2;
    }
#if OLED_BUFFER_MODE
    if(charSize == 16)
    {
		for(i = 0; i < 8; i++)
		{
			OLED_GRAM[y][x+i] = Is_Reverse == 0?F8X16[chr][i]:~F8X16[chr][i];
			OLED_GRAM[y+1][x+i] = Is_Reverse == 0?F8X16[chr][i+8]:~F8X16[chr][i+8];
		}
    }
    else if(charSize == 6)
    {
		for(i=0; i<6; i++)
		{
			OLED_GRAM[y][x+i] = Is_Reverse == 0?F8X6[chr][i]:~F8X6[chr][i];
		}
    }
#else
    if (charSize == 16)
    {
        OLED_Set_Pos(x, y);
        for (i = 0; i<8; i++)
            OLED_WR_Byte(Is_Reverse == 0?F8X16[chr][i]:~F8X16[chr][i], OLED_DATA);
        OLED_Set_Pos(x, y + 1);
        for (i = 0; i<8; i++)
            OLED_WR_Byte(Is_Reverse == 0?F8X16[chr][i + 8]:~F8X16[chr][i + 8], OLED_DATA);
    }
    else if(charSize == 6)
    {
        OLED_Set_Pos(x, y);
        for (i = 0; i<6; i++)
            OLED_WR_Byte(Is_Reverse == 0?F8X6[chr][i]:~F8X6[chr][i], OLED_DATA);
    }
#endif
}

/**
* @brief m^n函数
* @param None
* @retval None
*/
uint32_t oled_pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;
    while (n--)result *= m;
    return result;
}


static uint8_t xdata temp_digits[10];
/**
* @brief 显示数字（左对齐）
* @param x,y :起点坐标
* @param num :数值
* @param Size:字体大小
* @param Is_Reverse:是否反白
*/
void OLED_ShowNum(uint8_t x, uint8_t y, int32_t num, uint8_t len, uint8_t Size, uint8_t Is_Reverse)
{
    uint8_t i = 0;
    uint8_t digit_count = 0;
    uint8_t is_negative = 0;
    uint8_t start_x = x;
    
    // 处理负数
    if (num < 0) {
        is_negative = 1;
        num = -num;
        OLED_ShowChar(x - 8, y, '-', Size, Is_Reverse);
    } else {
        OLED_ShowChar(x - 8, y, ' ', Size, Is_Reverse);
	}
    
    // 特殊处理0
    if (num == 0) {
        OLED_ShowChar(x, y, '0', Size, Is_Reverse);
        for (i = 1; i < len; i++) {
            OLED_ShowChar(x + 8 * i, y, ' ', Size, Is_Reverse);
        }
        return;
    }
    
    // 提取数字到静态数组
    digit_count = 0;
    while (num > 0 && digit_count < 10) {
        temp_digits[digit_count++] = num % 10;
        num /= 10;
    }
    
    // 显示数字（从高位到低位）
    for (i = 0; i < digit_count; i++) {
        OLED_ShowChar(x + 8 * i, y, temp_digits[digit_count - 1 - i] + '0', Size, Is_Reverse);
    }
    
    // 补空格
    for (i = digit_count; i < len; i++) {
        OLED_ShowChar(x + 8 * i, y, ' ', Size, Is_Reverse);
    }
}
/**
 * @brief 显示浮点数（放大100倍存储）
 * @param x, y 坐标
 * @param value 放大100倍的值（如 12345 表示 123.45）
 * @param Is_Reverse 是否反白
 */
void OLED_ShowFloat(uint8_t x, uint8_t y, int32_t value, uint8_t Is_Reverse)
{
    char num_buf[12];
    uint8_t i = 0, j = 0;
    char temp[12];
    uint8_t is_negative = 0;
    int32_t integer_part, decimal_part;
    
    // 处理负数，并显示负号
    if (value < 0) {
        is_negative = 1;
        value = -value;
		OLED_ShowChar(x - 8, y, '-', 6, Is_Reverse);
    }  else {
        OLED_ShowChar(x - 8, y, ' ', 6, Is_Reverse);
	}
    
    // 分离整数和小数部分（放大100倍，所以除100）
    integer_part = value / 100;
    decimal_part = value % 100;
    
    // 转换整数部分
    if (integer_part == 0) {
        temp[i++] = '0';
    } else {
        while (integer_part > 0) {
            temp[i++] = (integer_part % 10) + '0';
            integer_part /= 10;
        }
    }
    
    // 反转整数部分
    for (j = 0; j < i; j++) {
        num_buf[j] = temp[i - 1 - j];
    }
    num_buf[i] = '\0';
    
    // 显示整数部分
    OLED_ShowString(x, y, num_buf, 6, Is_Reverse);
    
    // 显示小数点
    x += 8 * i;
    OLED_ShowChar(x, y, '.', 6, Is_Reverse);
    x += 8;
    
    // 显示小数部分（保证2位）
    if (decimal_part < 10) {
        OLED_ShowChar(x, y, '0', 6, Is_Reverse);
        x += 8;
        OLED_ShowChar(x, y, decimal_part + '0', 6, Is_Reverse);
    } else {
        OLED_ShowChar(x, y, (decimal_part / 10) + '0', 6, Is_Reverse);
        x += 8;
        OLED_ShowChar(x, y, (decimal_part % 10) + '0', 6, Is_Reverse);
    }
}

/**
* @brief 显示一个字符串
* @param x,y坐标，*chr字符串地址，charSize
* @retval None
*/
void OLED_ShowString(uint8_t x, uint8_t y, char *chr, uint8_t charSize, uint8_t Is_Reverse)
{
    uint8_t j = 0;
    while (chr[j] != '\0')
    {
        OLED_ShowChar(x, y, chr[j], charSize, Is_Reverse);
        x += 8;
        if (x>120)
        {
            x = 0;
            y += 2;
        }
        j++;
    }
}

/**
* @brief 显示显示BMP图片
* @param 显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7
* @retval None
*/
void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[], uint8_t Is_Reverse)
{
    uint16_t j = 0;
    uint8_t x, y;

    if (y1 % 8 == 0) 
		y = y1 / 8;
    else 
		y = y1 / 8 + 1;
#if OLED_BUFFER_MODE
    for (y = y0; y<y1; y++)
    {
        for (x = x0; x<x1; x++)
        {
			OLED_GRAM[y][x] = Is_Reverse == 0?BMP[j++]:~BMP[j++];
        }
    }
#else
    for (y = y0; y<y1; y++)
    {
        OLED_Set_Pos(x0, y);
        for (x = x0; x<x1; x++)
        {
            OLED_WR_Byte(Is_Reverse == 0?BMP[j++]:~BMP[j++], OLED_DATA);
        }
    }
#endif
}

/**
* @brief 初始化SSD1306
* @param None
* @retval None
*/
void OLED_Init(void)
{
    OLED_WR_Byte(0xAE, OLED_CMD);//--turn off oled panel 设置显示关

    OLED_WR_Byte(0x00, OLED_CMD);//---set low column address	设置低列地址
    OLED_WR_Byte(0x10, OLED_CMD);//---set high column address 设置高列地址

    OLED_WR_Byte(0x40, OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x40~0x7F) 设置显示开始行

    OLED_WR_Byte(0xDA, OLED_CMD);//--set com pins hardware configuration 设置 COM 引脚硬件配置
    OLED_WR_Byte(0x12, OLED_CMD);

#if OLED_BUFFER_MODE
    OLED_WR_Byte(0x20, OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02) 设置内存地址模式
    OLED_WR_Byte(0x00, OLED_CMD);//

    OLED_WR_Byte(0xD3, OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)	设置显示偏移
    OLED_WR_Byte(0x00/*±8的倍数*/,OLED_CMD);
#else
    OLED_WR_Byte(0x20, OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02) 设置内存地址模式
    OLED_WR_Byte(0x02, OLED_CMD);//

    OLED_WR_Byte(0xD3, OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)	设置显示偏移
    OLED_WR_Byte(0x00/*±8的倍数*/, OLED_CMD);//-not offset
#endif
    OLED_WR_Byte(0xA1, OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    OLED_WR_Byte(0xC8, OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常

    OLED_WR_Byte(0xDB, OLED_CMD);//--set vcomh	设置 Vcomh 取消选择水平
    OLED_WR_Byte(0x30, OLED_CMD);//Set VCOM Deselect Level

    OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
    OLED_WR_Byte(0x05,OLED_CMD);//

    OLED_WR_Byte(0x81, OLED_CMD);//--set contrast control register 为 BANK0 设置对比度控制
    OLED_WR_Byte(Brightness, OLED_CMD); //1~ 255; Default 0x7f (brightness Settings, the bigger the brighter) //1~255;默认0X7F (亮度设置,越大越亮)

    OLED_WR_Byte(0xA8, OLED_CMD);//--set multiplex ratio(1 to 64)	设置复用率
    OLED_WR_Byte(0x3f, OLED_CMD);//--1/64 duty

    OLED_WR_Byte(0xd5, OLED_CMD);//--set display clock divide ratio/oscillator frequency	设置显示时钟分频率、振荡器频率
    OLED_WR_Byte(0x80, OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec

    OLED_WR_Byte(0xD9, OLED_CMD);//--set pre-charge period 设置重充电周期
    OLED_WR_Byte(0xF1, OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock

    OLED_WR_Byte(0x8D, OLED_CMD);//--set Charge Pump enable/disable 升压允许
    OLED_WR_Byte(0x14, OLED_CMD);//--set(0x10) disable

    OLED_WR_Byte(0xA4, OLED_CMD);// Disable Entire Display On (0xa4/0xa5) 全部显示开启
    OLED_WR_Byte(0xA6, OLED_CMD);// Disable Inverse Display On (0xa6/a7) 设置正常显示

    OLED_WR_Byte(0xAF, OLED_CMD);// display ON 设置显示开
    OLED_Clear();
    OLED_Set_Pos(0, 0);
}
