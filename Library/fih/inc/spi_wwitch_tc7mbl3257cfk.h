#ifndef __SPI__SWITCH_TC7MBL3257CFK_H__
#define __SPI__SWITCH_TC7MBL3257CFK_H__

#define OE   PF15
#define S    PB3

extern void spi_switch_tc7mbl3257cfk_gpio_init(void);
extern void spi_switch_to_left(void);
extern void spi_switch_to_right(void);

#endif
