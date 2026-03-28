#ifndef SOFT_I2C_H_
#define SOFT_I2C_H_

#include <REGX52.H>
#include "stdint.h"

// I2C引脚：SCL、SDA、GND、VCC
sbit I2C_SCL = P2^1;
sbit I2C_SDA = P2^0;

//I2C所有操作函数				 
void I2C_start(void);				//发送I2C开始信号
void I2C_stop(void);	  			//发送I2C停止信号
uint8_t I2C_wait_ack(void); 		//I2C等待ACK信号
void I2C_ack(void);					//I2C发送ACK信号

void I2C_write_byte(uint8_t txd);	//I2C发送一个字节
uint8_t I2C_read_byte(uint8_t ack);	//I2C读取一个字节

void I2C_nack(void);				//I2C不发送ACK信号
uint8_t I2C_mem_write(uint8_t DevAddress, uint8_t MemAddress, uint8_t *pData, uint16_t Len);	//I2C对指定器件、指定寄存器连续写入
uint8_t I2C_mem_read(uint8_t DevAddress, uint8_t MemAddress, uint8_t *pBuffer, uint16_t Len);	//I2C对指定器件、指定寄存器连续读取

#endif