/**************************************************************************//**
 * @file     main.c
 * @version  V3.00
 * @brief    Show the usage of GPIO interrupt function.
 *
 * @copyright (C) 2018 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include <stdio.h>
#include "NuMicro.h"
#include "fih_arm.h"


#define SPI_CLK_FREQ    2000000


void delay_us(int us)
{
	int i = 0;
	for (i = 0;i<us; i++)
		CLK_SysTickDelay(1);
}


void delay_ms(int ms)
{
	int i = 0;
	for (i = 0;i<ms; i++)
		CLK_SysTickDelay(1000);
}

void SYS_Init(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable HIRC */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);

    /* Waiting for HIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);

    /* Switch HCLK clock source to HIRC */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

    /* Set both PCLK0 and PCLK1 as HCLK/2 */
    CLK->PCLKDIV = (CLK_PCLKDIV_APB0DIV_DIV2 | CLK_PCLKDIV_APB1DIV_DIV2);

    /* Switch UART0 clock source to HIRC */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HIRC, CLK_CLKDIV0_UART0(1));

    /* Enable UART peripheral clock */
    CLK_EnableModuleClock(UART0_MODULE);
	
	/* Switch USB clock source to HIRC & USB Clock = HIRC / 1 */
    CLK_SetModuleClock(USBD_MODULE, CLK_CLKSEL0_USBDSEL_HIRC, CLK_CLKDIV0_USB(1));

    /* Enable USB clock */
    CLK_EnableModuleClock(USBD_MODULE);
	
	/* Switch WDT clock source to LIRC */
    CLK_SetModuleClock(WDT_MODULE, CLK_CLKSEL1_WDTSEL_LIRC, 0);

    /* Enable WDT clock */
    CLK_EnableModuleClock(WDT_MODULE);

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate PllClock, SystemCoreClock and CycylesPerUs automatically. */
    SystemCoreClockUpdate();

    /*----------------------------------------------------------------------*/
    /* Init I/O Multi-function                                              */
    /*----------------------------------------------------------------------*/
    /* Set GPB multi-function pins for UART0 RXD and TXD */
	#if 1
    SYS->GPF_MFPL = (SYS->GPF_MFPL & ~(SYS_GPF_MFPL_PF0MFP_Msk | SYS_GPF_MFPL_PF1MFP_Msk)) |
                    (SYS_GPF_MFPL_PF1MFP_UART0_RXD | SYS_GPF_MFPL_PF0MFP_UART0_TXD);
	#else
	SYS->GPB_MFPH = (SYS->GPB_MFPH & ~(SYS_GPB_MFPH_PB12MFP_Msk | SYS_GPB_MFPH_PB13MFP_Msk)) |
                    (SYS_GPB_MFPH_PB12MFP_UART0_RXD | SYS_GPB_MFPH_PB13MFP_UART0_TXD);		
	#endif

    /* Lock protected registers */
    SYS_LockReg();
}

/*----------------------------------------------------------------------*/
/* Init UART0                                                           */
/*----------------------------------------------------------------------*/
void UART0_Init(void)
{
    /* Reset UART0 */
    SYS_ResetModule(UART0_RST);

    /* Configure UART0 and set UART0 baud rate */
    UART_Open(UART0, 115200);
}

void WDT_IRQHandler(void)
{
	SYS_UnlockReg();
	// WDT timeout flag set
    if(WDT_GET_TIMEOUT_INT_FLAG())
    {
        // Reset WDT and clear time out flag
        WDT_CLEAR_TIMEOUT_INT_FLAG();
        printf("Reset WDT counter\n");
		WDT_RESET_COUNTER();
	}
	SYS_LockReg();
}

void iic_gpio_init(void)
{
	GPIO_SetMode(PB, BIT4, GPIO_MODE_OPEN_DRAIN);
    GPIO_SetMode(PB, BIT5, GPIO_MODE_OPEN_DRAIN);
}

void proj_on_init(void)
{
	GPIO_SetMode(PF, BIT5, GPIO_MODE_OUTPUT);
	PF5 = 1;
	printf("PF5 = %d \n",PF5);
}
void LT7911_Reset(void)
{
	GPIO_SetMode(PF, BIT4, GPIO_MODE_OUTPUT);
	PF4 = 0;
	printf("PF4 = %d \n",PF4);
	delay_ms(10);
}
void Host_Irq_Init(void)
{
	GPIO_SetMode(PC, BIT0, GPIO_MODE_INPUT);
	GPIO_SetMode(PC, BIT1, GPIO_MODE_INPUT);
}

void LT_Gpio_Init(void)
{
	GPIO_SetMode(PB, BIT13, GPIO_MODE_INPUT);
	GPIO_SetMode(PB, BIT14, GPIO_MODE_INPUT);
	GPIO_SetMode(PB, BIT15, GPIO_MODE_OUTPUT);
}

void Flash_Led_Gpio_Init(void)
{
	GPIO_SetMode(PB, BIT1, GPIO_MODE_OUTPUT);
	GPIO_SetMode(PB, BIT2, GPIO_MODE_OUTPUT);
}

void Gpio_Init(void)
{
	//proj_on_init();
	Host_Irq_Init();
	iic_gpio_init();
	LT_Gpio_Init();
	Flash_Led_Gpio_Init();
}

void Flash_Led_Test(void)
{
	printf("Flash_Led_Test \n");
	//while(1)
	{
		
		PB2 = 1;
		//delay_ms(5);
		while(1)
		{
		PB1 = 1;
		printf("PB2 = %d ,PB1 = %d\n",PB2, PB1);
		delay_ms(2000);

		PB1 = 0;
		
		printf("PB2 = %d ,PB1 = %d\n",PB2, PB1);
		delay_ms(2000);
		}
	}
}

void spi_init(void)
{
	SYS_UnlockReg();
	
	CLK_SetModuleClock(SPI0_MODULE, CLK_CLKSEL2_SPI0SEL_PCLK1, MODULE_NoMsk);
	CLK_EnableModuleClock(SPI0_MODULE);
	/* Setup SPI0 multi-function pins */
    /* PA.3 is SPI0_SS,   PA.2 is SPI0_CLK,
       PA.1 is SPI0_MISO, PA.0 is SPI0_MOSI*/
    SYS->GPA_MFPL = (SYS->GPA_MFPL & ~(SYS_GPA_MFPL_PA3MFP_Msk |
                                       SYS_GPA_MFPL_PA2MFP_Msk |
                                       SYS_GPA_MFPL_PA1MFP_Msk |
                                       SYS_GPA_MFPL_PA0MFP_Msk)) |
                    (SYS_GPA_MFPL_PA3MFP_SPI0_SS |
                     SYS_GPA_MFPL_PA2MFP_SPI0_CLK |
                     SYS_GPA_MFPL_PA1MFP_SPI0_MISO |
                     SYS_GPA_MFPL_PA0MFP_SPI0_MOSI);
	
	
	SYS_LockReg();
	
	/*---------------------------------------------------------------------------------------------------------*/
    /* Init SPI                                                                                                */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Configure as a master, clock idle low, 32-bit transaction, drive output on falling clock edge and latch input on rising edge. */
    /* Set IP clock divider. SPI clock rate = 2MHz */
    SPI_Open(SPI0, SPI_MASTER, SPI_MODE_0, 32, SPI_CLK_FREQ);

    /* Enable the automatic hardware slave select function. Select the SS pin and configure as low-active. */
    SPI_EnableAutoSS(SPI0, SPI_SS, SPI_SS_ACTIVE_LOW);
}

int main(void)
{
    /* Init System, IP clock and multi-function I/O. */
    SYS_Init();

    /* Init UART0 for printf */
    UART0_Init();

    printf("\n\nCPU @ %d Hz\n", SystemCoreClock);
    printf("+------------------------------------------------+\n");
    printf("|            FIH  ARM  Version  1.0              |\n");
    printf("+------------------------------------------------+\n\n");

	Gpio_Init();

		//Flash_Led_Test();
	
#if 1
	for(int i = 0; i < 3; i++)
	{
		printf("PC0 = %d, PC1 = %d i = %d\n",PC0, PC1, i);
		delay_ms(1000);
	}
	lontium_enable_DSI(1);
#else
	do
	{
		delay_ms(10);
		printf("do PB13 = %d \n",PB13);
	}while(PB13 != 1);
		
	delay_ms(600);
	
	proj_on_init();
	
	do
	{
		delay_ms(10);
		printf("do PC0 = %d, PC1 = %d \n",PC0, PC1);
	}while(PC0 != 0 || PC1 != 0);
	
	
	if(PC0 == 0 && PC1 == 0 && PB13 == 1)
	{
		printf("PC0 = %d, PC1 = %d ,PB13 = %d\n",PC0, PC1, PB13);
		lontium_enable_DSI(1);
	}
	#endif
	
	printf("PB15-->LTgpio5 = %d\n",PB15);
	
	
	{
		DLPC_Write_Current(0);
		/*
		delay_ms(2000);
		DLPC_Write_Current(1);
		delay_ms(2000);
		DLPC_Write_Current(2);
		delay_ms(2000);
		DLPC_Write_Current(3);
		delay_ms(2000);
		*/
		/*
		DLPC_Write_Current(4);
		delay_ms(2000);

		DLPC_Write_Current(5);
		delay_ms(2000);
		DLPC_Write_Current(6);
		delay_ms(2000);
		DLPC_Write_Current(7);
		delay_ms(2000);
		*/
		
	}

	mcu_version_init();
	get_dl_flag();
	
	LT_Read_Version();
	delay_ms(200);
	//LT7911_Firmware_Upgrade();
	delay_ms(200);
	LT_Read_Version();
	
	#if 1
	/* Open USB controller */
    USBD_Open(&gsInfo, HID_ClassRequest, NULL);

    /* Endpoint configuration */
    HID_Init();

	NVIC_EnableIRQ(USBD_IRQn);
    /* Start USB device */
    USBD_Start();


	
	
	delay_ms(200);
	
	Hid_Transfer_Data();
	#endif
	
	//while(1);

	SYS_UnlockReg();
	// WDT timeout every 2^14 WDT clock, enable system reset, enable wake up system
    WDT_Open(WDT_TIMEOUT_2POW16, 0, TRUE, FALSE);
	
	WDT_EnableInt();
	NVIC_EnableIRQ(WDT_IRQn);
	SYS_LockReg();

	while(1)
    {
		SYS_UnlockReg();
		WDT_RESET_COUNTER();
		SYS_LockReg();
    }
}
