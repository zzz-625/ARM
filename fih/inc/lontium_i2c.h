#ifndef LT_I2C_H__
#define LT_I2C_H__

#include <stdio.h>
#include "NuMicro.h"
#include "fih_arm.h"

/**宏定义----------------------------------------------------*/
#define LT7911		0x56

#define LT7911_RESET			PF4


extern int LT_WriteI2C_Byte(uint8_t RegAddr, uint8_t Data);
extern int LT_WriteI2C_ByteN(uint8_t RegAddr, uint8_t *P_Data, uint16_t N);
extern uint8_t LT_ReadI2C_Byte(uint8_t RegAddr);
extern int LT_ReadI2C_ByteN(uint8_t RegAddr, uint8_t *p_data, uint16_t N);
extern void LtChipID(void);
extern void LT_Read_Mode(void);
extern uint8_t lontium_enable_DSI(uint8_t enable);
extern void LT_Read_Version(void);

#endif
