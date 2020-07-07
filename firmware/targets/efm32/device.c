#include <stdbool.h>

#include <em_device.h>
#include <em_chip.h>
#include <em_cmu.h>
#include <em_crypto.h>

#define LED0_PORT	gpioPortF
#define LED0_PIN	4

#define LED1_PORT	gpioPortF
#define LED1_PIN	5

void device_init(void)
{
	/* Chip errata */
	CHIP_Init();

	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_CRYPTO, true);
}

void device_led_on(void)
{
	GPIO_PinModeSet(LED0_PORT, LED0_PIN, gpioModePushPull, 0);  
	GPIO_PinOutSet(LED0_PORT, LED0_PIN);
}
