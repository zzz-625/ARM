#include <stdio.h>
#include "NuMicro.h"
#include "fih_arm.h"

#define NUM_BRIGHTNESS_LEVELS 8
static const uint16_t default_drive_strengths[NUM_BRIGHTNESS_LEVELS][3] = {
    /*r, b, g*/
    {100, 100, 100},
    {200, 200, 200},
    {300, 300, 300},
    {400, 400, 400},
    {500, 500, 500},
	{600, 600, 600},
	{700, 700, 700},
	{750, 750, 750},
};

int DLPC_WriteI2C_Byte(uint8_t side, uint8_t RegAddr, uint8_t Data)
{
	return iic_write_byte(side, RegAddr, &Data, 1);	
}

int DLPC_WriteI2C_ByteN(uint8_t side, uint8_t RegAddr, uint8_t *P_Data, uint16_t N)
{
	return iic_write_byte(side, RegAddr, P_Data, N);	
}		


uint8_t DLPC_ReadI2C_Byte(uint8_t side, uint8_t RegAddr)
{
	uint8_t p_data=0;
	if(iic_read_byte(side, RegAddr, &p_data, 1) != 1 )
	{
			return 0;
	}
	return p_data;
}

void DLPC_Write_Current(int level)
{
	uint8_t buffer[6] = {0};
	printf("DLPC_Write_Current level = %d \n",level);
	buffer[0] = default_drive_strengths[level][0] & 0xFF;
	buffer[1] = (default_drive_strengths[level][0] >> 8) & 0x03;
	buffer[2] = default_drive_strengths[level][1] & 0xFF;
	buffer[3] = (default_drive_strengths[level][1] >> 8) & 0x03;
	buffer[4] = default_drive_strengths[level][2] & 0xFF;
	buffer[5] = (default_drive_strengths[level][2] >> 8) & 0x03;
	
	DLPC_WriteI2C_ByteN(0x36, 0x54, buffer, 6);
	DLPC_WriteI2C_ByteN(0x3A, 0x54, buffer, 6);
}

