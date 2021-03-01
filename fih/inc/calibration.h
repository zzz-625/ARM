#ifndef __CALIBRATION_H__
#define __CALIBRATION_H__

#include <stdio.h>
#include "NuMicro.h"
#include "fih_arm.h"

#define DATA_FLASH_CALI_BASE        0xFC00
#define DATA_FLASH_CALI_END         0xFDFF

extern void Set_Vcali_To_Data_Flash(void);

extern void Get_Vcali_From_Data_Flash(void);


#endif
