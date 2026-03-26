/*************************************
 * IIC，没啥好说
 *************************************/
 
#include "Soft_I2C.h"
#include "stdint.h"
#include <intrins.h>

#define I2C_TIMEOUT_TIMES 100	//超时倍数

#define	HIGH 	1
#define LOW		0

void I2C_timeout_delay(void)
{
    _nop_();
    _nop_();
    _nop_();
}

void I2C_delay(void)
{
    // 超时延时，比普通延时稍长
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
}

// SCL拉高
void I2C_SCL_H(void)
{
	I2C_SCL = HIGH;
}

// SCL拉低
void I2C_SCL_L(void)
{
	I2C_SCL = LOW;
}

// SDA拉高
void I2C_SDA_H(void)
{
	I2C_SDA = HIGH;
}

// SDA拉低
void I2C_SDA_L(void)
{
	I2C_SDA = LOW;
}

uint8_t I2C_SDA_Read(void)
{
	return I2C_SDA;
}

///////////////////////////////////////

/**
 *@brief I2C_start 开始通讯
 *@note SCK高电平时SDA下降沿
 *@param 无
***/
void I2C_start(void)
{
	I2C_SDA_H();
	I2C_SCL_H();
	I2C_delay();
	
	I2C_SDA_L();
	I2C_delay();
	I2C_SCL_L();
}

/**
 *@brief I2C_stop 开始通讯
 *@note SCK高电平时SDA上升沿
 *@param 无
***/
void I2C_stop(void)
{
	I2C_SDA_L();
	I2C_SCL_H();
	I2C_delay();
	
	I2C_SDA_H();
	I2C_delay();
	I2C_SCL_L();
}

/**
 *@brief I2C_ack 应答
 *@note 接受完一字节后SDA丢低电平
 *@param 无
***/
void I2C_ack(void)
{
    I2C_SCL_L();
    I2C_SDA_L();
    I2C_delay();

    I2C_SCL_H();
    I2C_delay();
    I2C_SCL_L();
	I2C_SDA_H();
}

/**
 *@brief I2C_nack 应答
 *@note 接受完一字节后SDA丢低电平
 *@param 无
***/
void I2C_nack(void)
{
    I2C_SCL_L();
    I2C_SDA_H();	//SDA为高电平
    I2C_delay();

    I2C_SCL_H();
    I2C_delay();
    I2C_SCL_L();
}

/**
 *@brief I2C_wait_ack
 *@note 等待从机应答
 *@return 1为接受失败，0为应答成功
***/
uint8_t I2C_wait_ack(void)
{
	uint16_t time_temp = 0;

    I2C_SCL_H();
    I2C_delay();
    while(I2C_SDA_Read())				//等待SDA为低电平
    {
        time_temp++;
		I2C_timeout_delay();
        if(time_temp > I2C_TIMEOUT_TIMES)	//超时则强制结束I2C通信
        {
            I2C_stop();
            return 1;
        }
    }
    I2C_SCL_L();
    return 0;
}

/**
 *@brief I2C_write_byte
 *@note 发送一个字节
 *@param 你需要发送的字节
***/
void I2C_write_byte(uint8_t dat)
{
	uint8_t i = 0;
	
	I2C_SCL_L();
	for(i = 0; i < 8; i++) { // 高位先行
		if(dat & 0x80)	I2C_SDA_H();
		else	I2C_SDA_L();
		
		dat <<= 1;
		
		I2C_delay();
		I2C_SCL_H(); // 中场休息
		I2C_delay();
		I2C_SCL_L();
		I2C_delay();
	}
}

/**
 *@brief I2C_read_byte
 *@note 读一个字节
 *@return ack = 1时，发送ACK，ack=0，发送nACK
***/
uint8_t I2C_read_byte(uint8_t ack)
{
	uint8_t i = 0, receive = 0;
	
	for(i = 0; i < 8; i++) {
		I2C_SCL_L();
		I2C_delay();
		I2C_SCL_H();
		receive <<= 1;
		if (I2C_SDA_Read())
			receive++;
		I2C_delay();
	}
	if(!ack)
		I2C_nack();
	else
		I2C_ack();
	
	return receive;
}

///////
/////// 下面就都是完整过程了
///////


/**
 *@brief I2C_mem_write
 *@note 指定器件、指定寄存器连续写入
 *@param 器件地址、器件寄存器地址、待写入数据首地址，待输入数据长度
 *@return 0: 成功 1: 失败
***/
uint8_t I2C_mem_write(uint8_t DevAddress, uint8_t MemAddress, uint8_t *pData, uint16_t Len)
{
	I2C_start();
	I2C_write_byte(DevAddress << 1);
	if(I2C_wait_ack())
		return 1;
	I2C_write_byte(MemAddress);
	if(I2C_wait_ack())
		return 1;
	while(Len--)
	{
		I2C_write_byte(*pData++);
		if(I2C_wait_ack())
			return 1;
	}
	I2C_stop();
	return 0;
}

/**
 *@brief I2C_mem_read
 *@note I2C对指定器件、寄存器连续读取
 *@param 器件地址、器件寄存器地址、数据缓冲区首地址、数据长度
 *@return 0: 成功 1: 失败
***/
uint8_t I2C_mem_read(uint8_t DevAddress, uint8_t MemAddress, uint8_t *pBuffer, uint16_t Len)
{				  
    I2C_start();  
	I2C_write_byte(DevAddress << 1);		//发送写命令	   
	if(I2C_wait_ack())
		return 1;
	
    I2C_write_byte(MemAddress); 			//发送字地址  
	if(I2C_wait_ack())
		return 1;   
	
	I2C_start();  	 	   
	I2C_write_byte(DevAddress << 1 | 1); 	//进入接收模式         			   
	if(I2C_wait_ack())
		return 1;
	
	while(Len--)
	{
		*pBuffer++ = I2C_read_byte(Len!=0);	//读取字节	
	}
    I2C_stop();								//产生一个停止条件  
	return 0;
}




/*

/**
 *@brief I2C_write_bit
 *@note 写入8位寄存器的一个为
 *@param 器件地址、器件寄存器地址、写入的比特位（0~7）、写入数据
 *@return 0: 成功 1: 失败
***/
/*
uint8_t I2C_write_bit(uint8_t DevAddress, uint8_t addr, uint8_t bitNum, uint8_t Data)
{
	uint8_t b;
	if (!I2C_mem_read(DevAddress, addr, &b, 1))
	{
		b = (Data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
		return I2C_mem_write(DevAddress, addr, &b, 1);	//写入数据
	}
	else
		return 1;
}
*/

/**
 *@brief I2C_write_bits
 *@note 写入8位寄存器的多个位
 *@param 器件地址、器件寄存器地址、第一个写入位、写入长度（1~8）、写入数据
 *@return 0: 成功 1: 失败
***/
/*
uint8_t I2C_write_bits(uint8_t DevAddress, uint8_t addr, uint8_t bitStart, uint8_t length, uint8_t Data)
{
	//      010 要写入的值
	// 76543210 比特位
	//    xxx   args: bitStart=4, length=3
	// 00011100 掩码字节
	// 10101111 原始值（样本）
	// 10100011 原始值 & ~掩码
	// 10101011 掩码 | 原始值
	uint8_t b, mask = 0;
	if (!I2C_mem_read(DevAddress, addr, &b, 1))
	{
		mask = (((1 << length) - 1) << (bitStart - length + 1));	//掩码
		Data <<= (bitStart - length + 1);	//把写入的数据移动到位
		Data &= mask;
		b &= ~(mask);
		b |= Data;

		return I2C_mem_write(DevAddress, addr, &b, 1);	//写入数据
	}
	else
		return 1;
}
*/

/**
 *@brief I2C_read_bit
 *@note 读取一个位从8位器件的寄存器。
 *@param 器件地址、器件寄存器地址、位的位置来读取（0-7）、数据存储地址
 *@return 0: 成功 1: 失败
***/
/*
uint8_t I2C_read_bit(uint8_t DevAddress, uint8_t addr, uint8_t bitNum, uint8_t *Data)
{
	uint8_t b;
	if (!I2C_mem_read(DevAddress, addr, &b, 1))
	{
		*Data = b & (1 << bitNum);
		return 0;
	}
	else
	{
		return 1;
	}
}
*/

/**读取8位寄存器的多个位。
* @参数 DevAddress	I2C从器件地址
* @参数 addr    I2C从器件内部地址
* @参数 bitStart第一位的位置读取（0-7）
* @参数 length  位读取@参数长度数（不超过8）
* @参数 *data   数据存储地址（即'101'任何bitStart位置读取将等于0X05）
* @返回值（0=成功）
*/
/**
 *@brief I2C_read_bits
 *@note 读取8位寄存器的多个位。
 *@param 器件地址、器件寄存器地址、第一位读取位置（0-7）、读取长度、数据存储地址
 *@return 0: 成功 1: 失败
***/
/*
uint8_t I2C_read_bits(uint8_t DevAddress, uint8_t addr, uint8_t bitStart, uint8_t length, uint8_t *Data)
{
	// 01101001 读取字节
	// 76543210 比特位
	//    xxx   args: bitStart=4, length=3
	//    010   masked
	//   -> 010 shifted
	uint8_t b, mask = 0;
	if (!I2C_mem_read(DevAddress, addr, &b, 1))
	{

		mask = ((1 << length) - 1) << (bitStart - length + 1);
		b &= mask;
		b >>= (bitStart - length + 1);
		*Data = b;
		return 0;
	}
	else
		return 1;
}
*/




