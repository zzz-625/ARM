#include <stdio.h>
#include "NuMicro.h"
#include "fih_arm.h"


/**
	* @brief				HDMI Write IIC
	*	@param				None
	* @retval 			None
	* @note					Get slave whether ack
	*/	
int LT_WriteI2C_Byte(uint8_t RegAddr, uint8_t Data)
{
	return iic_write_byte(LT7911, RegAddr, &Data, 1);	
}

int LT_WriteI2C_ByteN(uint8_t RegAddr, uint8_t *P_Data, uint16_t N)
{
	return iic_write_byte(LT7911, RegAddr, P_Data, N);	
}		

/**
	* @brief				HDMI Read IIC
	*	@param				None
	* @retval 			None
	* @note					Get slave whether ack
	*/	
uint8_t LT_ReadI2C_Byte(uint8_t RegAddr)
{
	uint8_t p_data=0;
	if(iic_read_byte(LT7911, RegAddr, &p_data, 1) != 1 )
	{
		printf("iic_read_byte error\n");
		return 0;
	}
	return p_data;
}

int LT_ReadI2C_ByteN(uint8_t RegAddr, uint8_t *p_data, uint16_t N)
{
	if( iic_read_byte(LT7911, RegAddr, p_data, N) != 1)
	{
		return 0;
	}
	return 1;
}

void LtChipID(void)
{
    uint8_t i,j;
	LT_WriteI2C_Byte(0xff,0x80);
	LT_WriteI2C_Byte(0xee,0x01);
	LT_WriteI2C_Byte(0xff,0xa0);

	i = LT_ReadI2C_Byte(0x00);
	j = LT_ReadI2C_Byte(0x01);
#if 1 //def LT_IIC_DEBUG
    printf("chip id is ox%2x%2x\n",i,j);
#endif
}

void LT_Read_Version(void)
{
    uint8_t i,j;
	LT_WriteI2C_Byte(0xff,0x80);
	LT_WriteI2C_Byte(0xee,0x01);
	LT_WriteI2C_Byte(0xff,0xD2);

	i = LT_ReadI2C_Byte(0x10);
	j = LT_ReadI2C_Byte(0x11);
#if	1//def LT_IIC_DEBUG
    printf("version is 0x%2x, 0x%2x\n",i, j);
#endif
}

uint8_t lontium_enable_DSI( uint8_t enable )
{
    uint8_t ret = 0;

	printf("enable = %d \n",enable);
    ret = LT_WriteI2C_Byte(0xFF, 0x80); // write bank to 0x80
    ret &= LT_WriteI2C_Byte(0xee, 0x01); // enable external I2C
	
    ret &= LT_WriteI2C_Byte(0xFF, 0x90); // write bank to 0x86
	ret = LT_ReadI2C_Byte(0x0A);
	printf("before  0x900A = 0x%x \n",ret);

    ret &= LT_WriteI2C_Byte(0x0A, enable ? 0xFF : 0xFE ); // bit0: 0=reset mipi dphy, 1=release mipi dphy

    ret &= LT_WriteI2C_Byte(0xFF, 0x90); // write bank to 0x86
	ret = LT_ReadI2C_Byte(0x0A);
	printf("after  0x900A = 0x%x \n",ret);
	
    ret &= LT_WriteI2C_Byte(0xFF, 0x80); // write bank to 0x80

    ret &= LT_WriteI2C_Byte(0xee, 0x00); // disable external I2C

    return ret;
}

