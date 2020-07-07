#include <stdbool.h>

#include <stm32f2xx_hal.h>
#include <stm32f2xx_hal_rcc.h>
#include <stm32f2xx_hal_gpio.h>

#define LED3_GPIO_CLK_ENABLE	__HAL_RCC_GPIOC_CLK_ENABLE
#define LED3_GPIO_PORT		GPIOC
#define LED3_GPIO_PIN		GPIO_PIN_13

void device_init(void)
{
	HAL_Init();

	LED3_GPIO_CLK_ENABLE();

	GPIO_InitTypeDef gpio_led;

	gpio_led.Pin = LED3_GPIO_PIN;
	gpio_led.Mode = GPIO_MODE_OUTPUT_PP;

	HAL_GPIO_Init(LED3_GPIO_PORT, &gpio_led);
}

void device_led_on(void)
{
	HAL_GPIO_WritePin(LED3_GPIO_PORT, LED3_GPIO_PIN, GPIO_PIN_SET);
}
