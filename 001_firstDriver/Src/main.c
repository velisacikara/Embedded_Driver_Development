

#include "stm32f407xx.h"


static void GPIO_LedConfig(); // Bir fonksiyon static tanimlanirsa sadece bulundugu dosyada kullanilabilir.
static void LockControl();


int main(void)
{


	GPIO_LedConfig();
/*
 * Eger GPIO_LockPin fonksiyonu dogru calisiyorsa butonu input modda kilitler.
 * Boylece output moda gecmez, ledler ayni sekilde yanmaya devam eder.
 * GPIO_LockPin dogru calismazsa buton output moda gecer, ledler yanmaz.
 */
	GPIO_LockPin(GPIOA, GPIO_PIN_0);

	LockControl();



	for(;;)
	{
		if(GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_Pin_Set)
		{
			GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 |GPIO_PIN_14 |GPIO_PIN_15, GPIO_Pin_Set);
		}
		else
		{
			GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 |GPIO_PIN_14 |GPIO_PIN_15, GPIO_Pin_Reset);
		}
	}
}


static void GPIO_LedConfig()
{
	RCC_GPIOD_CLK_ENABLE(); // GPIOD icin clock
	RCC_GPIOA_CLK_ENABLE(); // GPIOA icin clock

	GPIO_InitTypeDef_t GPIO_InitStruct = { 0 };

	// GPIOD icin konfigurasyon (LEDler icin).
	GPIO_InitStruct.pinNumber = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	GPIO_InitStruct.Otype = GPIO_OTYPE_PP;
	GPIO_InitStruct.PuPd = GPIO_PUPD_NOPULL;

	GPIO_Init(GPIOD, &GPIO_InitStruct);

	memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitStruct));

	// GPIOA icin konfigurasyon (User buton icin).
	GPIO_InitStruct.pinNumber = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.PuPd = GPIO_PUPD_PULLDOWN;

	GPIO_Init(GPIOA, &GPIO_InitStruct);

}

static void LockControl()
{
	GPIO_InitTypeDef_t GPIO_InitStruct = { 0 };

	/*
	 * GPIOA icin konfigurasyon.
	 * User buton output moda cekilir.
	 */

	GPIO_InitStruct.pinNumber = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT;
	GPIO_InitStruct.PuPd = GPIO_PUPD_NOPULL;

	GPIO_Init(GPIOA, &GPIO_InitStruct);
}









