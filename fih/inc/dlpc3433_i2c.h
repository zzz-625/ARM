#ifndef DLPC_3433_I2C_H__
#define DLPC_3433_I2C_H__
#include <stdint.h>

#define LEFT    0x36
#define RIGHT   0x3A

extern int DLPC_WriteI2C_Byte(uint8_t side, uint8_t RegAddr, uint8_t Data);
extern int DLPC_WriteI2C_ByteN(uint8_t side, uint8_t RegAddr, uint8_t *P_Data, uint16_t N);
extern uint8_t DLPC_ReadI2C_Byte(uint8_t side, uint8_t RegAddr);
extern void DLPC_Write_Current(int level);

#endif
