#ifndef LT7911_UPGRADE_H__
#define LT7911_UPGRADE_H__

#include "fih_arm.h"



extern int CheckChipId(void);
extern void Config(void);
extern void ReadKey(void);
extern void BlockErase(void);
extern void Write_Frimware(void);
extern int Verify_Firmware(void);
extern void Write_Key(void);
extern void LT7911_Firmware_Upgrade(void);


#endif
