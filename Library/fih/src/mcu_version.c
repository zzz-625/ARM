#include <stdio.h>
#include "stdint.h"
#include "NuMicro.h"
#include "fih_arm.h"


Data_Flash_T data_flash;

void Data_Flash_Value_Init(void)
{
	data_flash.cali_flag = 0;
	data_flash.h1 = 0;
	data_flash.v1 = 0;
	data_flash.h2 = 0;
	data_flash.v2 = 0;
	
	data_flash.mcu_version_flag = 0;
	data_flash.mcu_version = MCU_VERSION;
	data_flash.lt7211b_fwversion = LT7211B_VERSION;

	data_flash.dl_flags = 0;
	
#ifdef MCU_VERSION_DEBUG
	printf("[value init] cali_flag = %d,left[%d,%d],right[%d,%d],mcu_version_flag = %d, mcu_version = %s,7211_version = %s\n", \
	data_flash.cali_flag, data_flash.h1, data_flash.v1, data_flash.h2, data_flash.v2, \
	data_flash.mcu_version_flag, data_flash.mcu_version,data_flash.lt7211b_fwversion);
#endif
}

static int  set_data_flash_base(uint32_t u32DFBA)
{
    uint32_t   au32Config[2];

    if (FMC_ReadConfig(au32Config, 2) < 0)
    {
	#ifdef MCU_VERSION_DEBUG
        printf("\nRead User Config failed!\n");
	#endif
        return -1;
    }

    if ((!(au32Config[0] & 0x1)) && (au32Config[1] == u32DFBA))
        return 0;

    FMC_ENABLE_CFG_UPDATE();

    au32Config[0] &= ~0x1;
    au32Config[1] = u32DFBA;

    if (FMC_WriteConfig(au32Config, 2) < 0)
        return -1;
#ifdef MCU_VERSION_DEBUG
    printf("\nSet Data Flash base as 0x%x.\n", u32DFBA);
#endif

    // Perform chip reset to make new User Config take effect
    SYS->IPRST0 = SYS_IPRST0_CHIPRST_Msk;
    return 0;
}

void Fmc_Test(void)
{
	uint32_t    i, u32Data;

    u32Data = FMC_ReadCID();
#ifdef MCU_VERSION_DEBUG
    printf("  Company ID ............................ [0x%08x]\n", u32Data);
#endif

    u32Data = FMC_ReadPID();
#ifdef MCU_VERSION_DEBUG
    printf("  Product ID ............................ [0x%08x]\n", u32Data);
#endif

    for (i = 0; i < 3; i++)
    {
        u32Data = FMC_ReadUID(i);
	#ifdef MCU_VERSION_DEBUG
        printf("  Unique ID %d ........................... [0x%08x]\n", i, u32Data);
	#endif
    }

    for (i = 0; i < 4; i++)
    {
        u32Data = FMC_ReadUCID(i);
	#ifdef MCU_VERSION_DEBUG
        printf("  Unique Customer ID %d .................. [0x%08x]\n", i, u32Data);
	#endif
    }
	
    /* Read User Configuration */
#ifdef MCU_VERSION_DEBUG
    printf("  User Config 0 ......................... [0x%08x]\n", FMC_Read(FMC_CONFIG_BASE));
    printf("  User Config 1 ......................... [0x%08x]\n", FMC_Read(FMC_CONFIG_BASE+4));
#endif

    /* Read Data Flash base address */
    u32Data = FMC_ReadDataFlashBaseAddr();
#ifdef MCU_VERSION_DEBUG
    printf("  Data Flash Base Address ............... [0x%08x]\n", u32Data);	
#endif
}


void MCU_FMC_Init(void)
{
	uint32_t    u32Addr;
	SYS_UnlockReg();

    /* Enable FMC ISP function */
    FMC_Open();
	if (set_data_flash_base(DATA_FLASH_CALI_BASE) < 0)
    {
        printf("Failed to set Data Flash base address!\n");
        goto lexit;
    }
			
	for (u32Addr = DATA_FLASH_CALI_BASE; u32Addr < DATA_FLASH_VER_END; u32Addr += FMC_FLASH_PAGE_SIZE)
	{
	#ifdef MCU_VERSION_DEBUG
		printf("MCU_FMC_Init Flash test address: 0x%x    \n", u32Addr);
	#endif

		// Erase page
		FMC_Erase(u32Addr);
	}
		/* Read BS */
	
#ifdef MCU_VERSION_DEBUG
    printf("  Boot Mode ............................. ");
#endif
    if (FMC_GetBootSource() == 0)
	{
	#ifdef MCU_VERSION_DEBUG
        printf("[APROM]\n");
	#endif
	}
    else
    {
	#ifdef MCU_VERSION_DEBUG
        printf("[LDROM]\n");
        printf("  WARNING: The driver sample code must execute in AP mode!\n");
	#endif
        //goto lexit;
    }

    Fmc_Test();
	Data_Flash_Value_Init();
		
lexit:

    /* Disable FMC ISP function */
    FMC_Close();

    /* Lock protected registers */
    SYS_LockReg();
#ifdef MCU_VERSION_DEBUG
    printf("\nMCU FMC INIT Completed.\n");
#endif
}

void Set_Mcu_Version(void)
{
	SYS_UnlockReg();

    /* Enable FMC ISP function */
    FMC_Open();
	uint32_t    u32Addr;
	for (u32Addr = DATA_FLASH_VER_BASE; u32Addr < DATA_FLASH_VER_END; u32Addr += FMC_FLASH_PAGE_SIZE)
	{
	#ifdef MCU_VERSION_DEBUG
		printf("Set_Mcu_Version Flash test address: 0x%x    \n", u32Addr);
	#endif

		// Erase page
		FMC_Erase(u32Addr);
	}
	
	data_flash.mcu_version_flag = FMC_Read(DATA_FLASH_VER_BASE);
	
	if (1 !=data_flash.mcu_version_flag)
	{
		data_flash.mcu_version_flag = 1;
		FMC_Write(DATA_FLASH_VER_BASE, data_flash.mcu_version_flag);
	}
	
	FMC_Write(DATA_FLASH_VER_BASE + 4, data_flash.mcu_version);
	FMC_Write(DATA_FLASH_VER_BASE + 14, data_flash.lt7211b_fwversion);
	FMC_Write(DATA_FLASH_VER_BASE + 24, data_flash.dl_flags);
	printf("[set] data_flash.dl_flags = %d \n",data_flash.dl_flags);
	
#ifdef MCU_VERSION_DEBUG
	printf("[set] mcu_version_flag = %d, mcu_version = %s,7211_version = %s\n",data_flash.mcu_version_flag, \
		data_flash.mcu_version, data_flash.lt7211b_fwversion);
#endif

    /* Disable FMC ISP function */
    FMC_Close();

    /* Lock protected registers */
    SYS_LockReg();
#ifdef MCU_VERSION_DEBUG
    printf("\nSet_Mcu_Version Completed.\n");
#endif
}

void Get_Mcu_Version(void)
{
	SYS_UnlockReg();

    /* Enable FMC ISP function */
    FMC_Open();
	
	data_flash.mcu_version_flag = FMC_Read(DATA_FLASH_VER_BASE);
	if (1 == data_flash.mcu_version_flag)
	{
		data_flash.mcu_version = FMC_Read(DATA_FLASH_VER_BASE+4);
		data_flash.lt7211b_fwversion = FMC_Read(DATA_FLASH_VER_BASE+14);
	}
	else
	{
		data_flash.mcu_version = NULL;
		data_flash.lt7211b_fwversion = NULL;
	}
	
	data_flash.dl_flags = FMC_Read(DATA_FLASH_VER_BASE + 24);
	
	printf("[get] data_flash.dl_flags = %d \n",data_flash.dl_flags);
#ifdef MCU_VERSION_DEBUG
	printf("[get] mcu_version_flag = %d, mcu_version = %s, lt7211_version = %s\n",data_flash.mcu_version_flag, \
		data_flash.mcu_version, data_flash.lt7211b_fwversion);
#endif

    /* Disable FMC ISP function */
    FMC_Close();

    /* Lock protected registers */
    SYS_LockReg();
#ifdef MCU_VERSION_DEBUG
    printf("\nGet_Mcu_Version Completed.\n");
#endif
}

void mcu_version_init(void)
{
	MCU_FMC_Init();
	Set_Mcu_Version();
	Get_Mcu_Version();
#ifdef MCU_VERSION_DEBUG
	printf("[main] cali_flag = %d, left[%d : %d],right[%d : %d], mcu_version_flag = %d, mcu_version = %s,7211_version = %s\n", \
		data_flash.cali_flag, data_flash.h1, data_flash.v1, data_flash.h2, data_flash.v2, \
		data_flash.mcu_version_flag,data_flash.mcu_version,data_flash.lt7211b_fwversion);
#endif
}

