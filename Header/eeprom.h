#ifndef __EEPROM_H_
#define __EEPROM_H_

void EEPROM_WriteByte(uint8_t WordAddress, uint8_t Data);
uint8_t EEPROM_ReadByte(uint8_t WordAddress);

void EEPROM_WriteInt16(uint8_t WordAddress, int16_t Data);
int16_t EEPROM_ReadInt16(uint8_t WordAddress);

void EEPROM_WriteFloat(uint8_t WordAddress, float Data);
float EEPROM_ReadFloat(uint8_t WordAddress);

#endif
