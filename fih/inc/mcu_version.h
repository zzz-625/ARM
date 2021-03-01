#ifndef __MCU_VERSION_H__
#define __MCU_VERSION_H__

#include "fih_arm.h"

#define DATA_FLASH_VER_BASE        0xFE00
#define DATA_FLASH_VER_END         0xFFFF

#define MCU_VERSION		"V20.00.07"
#define LT7211B_VERSION	"V10.00.02"

typedef struct
{
	uint8_t cali_flag;				//0  FC00
	int8_t h1;						//4
	int8_t v1;						//8
	int8_t h2;						//12
	int8_t v2;						//16
	
	uint32_t mcu_version_flag;		//0		FE00
	uint32_t *mcu_version;			//4
	uint32_t *lt7211b_fwversion;		//14
	
	int32_t dl_flags;				//24
}Data_Flash_T;

extern Data_Flash_T data_flash;

extern void Data_Flash_Value_Init(void);
extern void Fmc_Test(void);
extern void MCU_FMC_Init(void);
extern void Set_Mcu_Version(void);
extern void Get_Mcu_Version(void);
extern void mcu_version_init(void);

#endif
