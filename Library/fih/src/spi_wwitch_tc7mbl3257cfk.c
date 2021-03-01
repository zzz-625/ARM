#include <stdio.h>
#include "NuMicro.h"
#include "fih_arm.h"

void spi_switch_tc7mbl3257cfk_gpio_init(void)
{
	GPIO_SetMode(PF, BIT15, GPIO_MODE_OUTPUT);
	GPIO_SetMode(PB, BIT3, GPIO_MODE_OUTPUT);
}

void spi_switch_to_left(void)
{
	OE = 0;
	S = 0;
}
void spi_switch_to_right(void)
{
	OE = 0;
	S = 1;
}
