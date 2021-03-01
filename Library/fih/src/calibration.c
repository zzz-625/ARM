#include <stdio.h>
#include "stdint.h"
#include "NuMicro.h"
#include "fih_arm.h"



void Set_Vcali_To_Data_Flash(void)
{
	SYS_UnlockReg();

    /* Enable FMC ISP function */
    FMC_Open();
	
	uint32_t    u32Addr;
	for (u32Addr = DATA_FLASH_CALI_BASE; u32Addr < DATA_FLASH_CALI_END; u32Addr += FMC_FLASH_PAGE_SIZE)
    {
	#ifdef OLED_CALI_DEBUG
        printf("Set_Vcali_To_Data_Flash Flash test address: 0x%x    \n", u32Addr);
	#endif

        // Erase page
        FMC_Erase(u32Addr);
	}
	
	data_flash.cali_flag = FMC_Read(DATA_FLASH_CALI_BASE);
	
	if (1 !=data_flash.cali_flag)
	{
		data_flash.cali_flag = 1;
		FMC_Write(DATA_FLASH_CALI_BASE, data_flash.cali_flag);
	}
	
	FMC_Write(DATA_FLASH_CALI_BASE + 4, data_flash.h1);
	FMC_Write(DATA_FLASH_CALI_BASE + 8, data_flash.v1);
	FMC_Write(DATA_FLASH_CALI_BASE + 12, data_flash.h2);
	FMC_Write(DATA_FLASH_CALI_BASE + 16, data_flash.v2);
	
#ifdef OLED_CALI_DEBUG
		printf("[set] cali_flag = %d, left[%d : %d],right[%d : %d]\n", \
			data_flash.cali_flag, data_flash.h1, data_flash.v1, data_flash.h2, data_flash.v2);
#endif

    /* Disable FMC ISP function */
    FMC_Close();

    /* Lock protected registers */
    SYS_LockReg();
#ifdef OLED_CALI_DEBUG
    printf("\Set_Vcali_To_Data_Flash Completed.\n");
#endif
}


void Get_Vcali_From_Data_Flash(void)
{
	SYS_UnlockReg();

    /* Enable FMC ISP function */
    FMC_Open();
	
	data_flash.cali_flag = FMC_Read(DATA_FLASH_CALI_BASE);
	if (1 == data_flash.cali_flag)
	{
		data_flash.h1 = FMC_Read(DATA_FLASH_CALI_BASE+4);
		data_flash.v1 = FMC_Read(DATA_FLASH_CALI_BASE+8);
		data_flash.h2 = FMC_Read(DATA_FLASH_CALI_BASE+12);
		data_flash.v2 = FMC_Read(DATA_FLASH_CALI_BASE+16);
	}
	else
	{
		data_flash.h1 = 0;
		data_flash.v1 = 0;
		data_flash.h2 = 0;
		data_flash.v2 = 0;
	}
	
#ifdef OLED_CALI_DEBUG
		printf("[get] cali_flag = %d, left[%d : %d],right[%d : %d]\n", \
			data_flash.cali_flag, data_flash.h1, data_flash.v1, data_flash.h2, data_flash.v2);
#endif

    /* Disable FMC ISP function */
    FMC_Close();

    /* Lock protected registers */
    SYS_LockReg();
#ifdef OLED_CALI_DEBUG
    printf("\nGet_Vcali_From_Data_Flash Completed.\n");
#endif
}
