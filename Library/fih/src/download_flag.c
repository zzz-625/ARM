#include <stdio.h>
#include "NuMicro.h"
#include "download_flag.h"
#include "fih_arm.h"


void dl_flag_init(void)
{
	data_flash.dl_flags = 0;
	
	printf("[dl_flag_init] download flags = %d \n",data_flash.dl_flags);
}

void set_boot_flags(void)
{
	uint32_t   au32Config[2];

	SYS_UnlockReg();
	
	FMC_Open();
    if (FMC_ReadConfig(au32Config, 2) < 0)
    {
        printf("\nRead User Config failed!\n");
    }
	FMC_ENABLE_CFG_UPDATE();
	

    au32Config[0] &= 0xFFFFFF3E;
	
	printf("au32Config[0] = %x\n",au32Config[0]);
	

	if (FMC_WriteConfig(au32Config, 2) < 0)
	{
        printf("FMC_WriteConfig fail \n");
	}

	//Perform chip reset to make new User Config take effect
    SYS->IPRST0 = SYS_IPRST0_CHIPRST_Msk;

    /* Disable FMC ISP function */
    FMC_Close();

    /* Lock protected registers */
    SYS_LockReg();
}

int get_dl_flag(void)
{
	SYS_UnlockReg();
	
	FMC_Open();
	
	data_flash.dl_flags = FMC_Read(DATA_FLASH_VER_BASE+24);
	
	printf("[get_dl_flag] data_flash.dl_flags = %d \n",data_flash.dl_flags);
	
	/* Disable FMC ISP function */
    FMC_Close();

    /* Lock protected registers */
    SYS_LockReg();
	
	return data_flash.dl_flags;
}
void set_dl_flag(void)
{
	SYS_UnlockReg();
	
	FMC_Open();
	#if 0
	uint32_t    u32Addr;
	for (u32Addr = DATA_FLASH_DL_FLAG_BASE; u32Addr < DATA_FLASH_DL_FLAG_END; u32Addr += FMC_FLASH_PAGE_SIZE)
    {
        printf("set_dl_flag  Flash test address: 0x%x    \n", u32Addr);
        // Erase page
        FMC_Erase(u32Addr);
	}
	#endif
	printf("[set_dl_flag] before data_flash.dl_flags = %d \n",data_flash.dl_flags);
	
	FMC_Write(DATA_FLASH_VER_BASE+24, data_flash.dl_flags);
	printf("[set_dl_flag] after data_flash.dl_flags = %d \n",data_flash.dl_flags);
	
	/* Disable FMC ISP function */
    FMC_Close();

    /* Lock protected registers */
    SYS_LockReg();
}
