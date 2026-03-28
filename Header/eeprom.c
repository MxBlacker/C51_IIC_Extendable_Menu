#include <REGX52.H>
#include "stdint.h"
#include "Soft_I2C.h"

#define EEPROM_ADDRESS		0x50

void delay_us(uint16_t us);

void delay_ms(uint16_t ms);

// 是这样的，我在CSDN上找的代码默认是7为地址，江协的版本是8位

/*
void EEPROM_WriteByte(uint8_t WordAddress, uint8_t Data)
{
    I2C_mem_write(EEPROM_ADDRESS, WordAddress, &Data, 1);
}

uint8_t EEPROM_ReadByte(uint8_t WordAddress)
{
    uint8_t Data;
    I2C_mem_read(EEPROM_ADDRESS, WordAddress, &Data, 1);
    return Data;
}
*/

/**
 * @brief 写入16位整数（高字节在前）
 * @param WordAddress EEPROM起始地址
 * @param Data 要写入的16位整数
 */
void EEPROM_WriteInt16(uint8_t WordAddress, int16_t Data)
{
    uint8_t buffer[2];
    buffer[0] = (Data >> 8) & 0xFF;  // 高字节
    buffer[1] = Data & 0xFF;         // 低字节
    I2C_mem_write(EEPROM_ADDRESS, WordAddress, buffer, 2);
	delay_ms(10);  // 等待 EEPROM 写入完成
}

/**
 * @brief 读取16位整数
 * @param WordAddress EEPROM起始地址
 * @return 读取到的16位整数
 */
int16_t EEPROM_ReadInt16(uint8_t WordAddress)
{
    uint8_t buffer[2];
    I2C_mem_read(EEPROM_ADDRESS, WordAddress, buffer, 2);
    return (int16_t)((buffer[0] << 8) | buffer[1]);
}

/**
 * @brief 写入浮点数（使用联合体转换）
 * @param WordAddress EEPROM起始地址
 * @param Data 要写入的浮点数
 */
void EEPROM_WriteFloat(uint8_t WordAddress, float Data)
{
    uint8_t *p = (uint8_t*)&Data;
    I2C_mem_write(EEPROM_ADDRESS, WordAddress, p, sizeof(float));
	delay_ms(10);
}

/**
 * @brief 读取浮点数
 * @param WordAddress EEPROM起始地址
 * @return 读取到的浮点数
 */
float EEPROM_ReadFloat(uint8_t WordAddress)
{
    float Data = 0.0f;
    uint8_t *p = (uint8_t*)&Data;
    if (I2C_mem_read(EEPROM_ADDRESS, WordAddress, p, sizeof(float)) != 0) {
        return 1.4f;
    }
    return Data;
}