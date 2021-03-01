#ifndef MCU_I2C_H__
#define MCU_I2C_H__

#include <stdio.h>
#include "NuMicro.h"
#include "fih_arm.h"

/**宏定义----------------------------------------------------*/

#define GPIO_PIN_RESET	0
#define GPIO_PIN_SET	1

#define IIC_SCL(pin_status)			PB5 = pin_status;
#define IIC_SDA(pin_status)			PB4 = pin_status;

#define IIC_SCL_IS_HIGH()				(PB5 != GPIO_PIN_RESET)
#define IIC_SDA_IS_HIGH()				(PB4 != GPIO_PIN_RESET)

typedef enum
{
  RESET = 0,
  SET = !RESET
} FlagStatus;


extern uint8_t iic_write_byte(uint8_t sla, uint8_t suba, uint8_t *s, uint8_t Num);
extern uint8_t iic_read_byte(uint8_t sla,uint8_t suba,uint8_t *s, uint16_t Num);

#endif
