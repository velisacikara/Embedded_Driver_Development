/*
 * GPIO.c
 *
 *  Created on: 5 Mar 2024
 *      Author: VeliSacikara
 */

#include "GPIO.h"

/**
 * @brief GPIO_Init, configures port and pin
 * Tum bitleri dolasarak kullanicinin o pini kullanip kullanmadigini belirler, kullanicinin istedigi konfigurasyonları yapar.
 *
 * @param GPIOx = GPIO Port Base Address
 *
 *  @param GPIO_InitTypeDef_t = User Config Structures
 *
 *  @retval void
 */

void GPIO_Init(GPIO_TypeDef_t *GPIOx, GPIO_InitTypeDef_t *GPIO_ConfigStruct)
{
	uint32_t position;
	uint32_t fakePosition = 0;
	uint32_t lastPosition = 0;

	/*
	 * Her bir pin icin dongu kullanarak sahte bir position olusturduk.
	 * Kullanicinin girdigi pin numarasi ile fakePosition'i andleyerek o ilgili pinin durumunu elde ettik.
	 * fakePosition ve lastPosition birbirine esitse ilgili pin kullanici tarafindan secilmis demektir.
	 * Boylece kullanicinin hangi pinleri sectigi belirlenmis olur.
	 * Ilgili pin secilmisse konfigurasyon yapilir.
	 * Ilgili pin secilmemsse if'e girilmez, konfigurasyon yapilmaz.
	 *
	 */

	for(position = 0; position < 16; position++)
	{
		fakePosition = (0x1 << position);
		lastPosition = (uint32_t)(GPIO_ConfigStruct->pinNumber) & fakePosition;

		/*
		 * Konfigurasyon Adimlari:
		 * 1. Ilgili registeri oku, yani bir temp degiskene ata.
		 * Not: Direkt olarak register uzerinde islem yapma.
		 * 2. temp degiskendeki ilgili bitleri (orn. moder icin iki otype icin bir bit) ilk olarak temizle (&= clear islemi).
		 * 3. Kullanicidan alinan konfig bilgisini ilgili bit sayisi kadar kaydirip yaz (|= islemi).
		 * 4. temp degiskeni ilgili registera ata.
		 */

		if(fakePosition == lastPosition)
		{
			/* Mode Config */

			uint32_t tempValue = GPIOx->MODER;
			tempValue &= ~(0x3U << (position * 2 ) );
			tempValue |= (GPIO_ConfigStruct->Mode << (position * 2) );
			GPIOx->MODER = tempValue;

			/*
			 * Otype ve Ospeed konfigurasyonlari modun sadece output veya alternate function olmasi durumunda gecerlidir.
			 * diger modlar secili iken Otype ve Ospeed konfigurasyonlarinin bir onemi yoktur.
			 * Bu sebeple bu iki durumun konfigurasyonlari if blogu icerisinde tanimlandi.
			 */

			if(GPIO_ConfigStruct->Mode == GPIO_MODE_OUTPUT || GPIO_ConfigStruct->Mode == GPIO_MODE_AF)
			{
				/* Output Type Config */

				tempValue = GPIOx->OTYPER;
				tempValue &= ~(0x1U << position);
				tempValue |= (GPIO_ConfigStruct->Otype << position);
				GPIOx->OTYPER = tempValue;

				/* Output speed Config */

				tempValue = GPIOx->OSPEEDR;
				tempValue &= ~(0x3U << (position * 2) );
				tempValue |= (GPIO_ConfigStruct->Speed << (position * 2) );
				GPIOx->OSPEEDR = tempValue;
			}

			/* PuPd Config */

			tempValue = GPIOx->PUPDR;
			tempValue &= ~(0x3U << (position * 2) );
			tempValue |= (GPIO_ConfigStruct->PuPd << (position * 2) );
			GPIOx->PUPDR = tempValue;
		}
	}
}


/**
 * @brief GPIO_WritePin, makes pin High or Low
 *
 * @param GPIOx = GPIO Port Base Address
 *
 *  @param pinNumber = GPIO Pin Numbers 0 - 15
 *
 *  @param pinstate = GPIO_Pin_Set OR GPIO_Pin_Reset
 *
 *  @retval void
 */

void GPIO_WritePin(GPIO_TypeDef_t *GPIOx, uint16_t pinNumber, GPIO_PinState_t pinState)
{

	if(pinState == GPIO_Pin_Set)
	{
		GPIOx->BSRR = pinNumber;
	}
	else
	{
		GPIOx->BSRR = (pinNumber << 16U);
	}

}

/**
 * @brief GPIO_ReadPin, read the pin of GPIOx Port
 *
 * @param GPIOx = GPIO Port Base Address
 *
 *  @param pinNumber = GPIO Pin Numbers 0 - 15
 *
 *  @retval GPIO_PinState_t
 */

GPIO_PinState_t GPIO_ReadPin(GPIO_TypeDef_t *GPIOx, uint16_t pinNumber)
{
	GPIO_PinState_t bitStatus = GPIO_Pin_Reset;

	/*
	 * Input data register okundugu zaman 16 bitlik tum register okunur.
	 * Istedigimiz bitin durumunu ogrenmek icin pinNumber ile and islemi yapariz.
	 * Boylece ilgili bitin set mi yoksa reset mi oldugunu anlariz.
	 *
	 * Neden != GPIO_Pin_Reset seklinde bir kontrol yaptik?
	 * Ornegin 7. biti kontrol ettik ve o bitin 1 oldugunu ogrendik.
	 * 7. bitin 1 oldugu durumda hex karsiligi 0x80 oldu.
	 * 0x80 == GPIO_Pin_Set kontrolu 0x80 == 1 ifadesine esit olur.
	 * Bu durumda 7. bit 1 olmasina ragmen bize False doner.
	 */

	if((GPIOx->IDR & pinNumber) != GPIO_Pin_Reset)
	{
		bitStatus = GPIO_Pin_Set;
	}

	return bitStatus;
}

/**
 * @brief GPIO_LockPin, locks the pin of GPIOx Port
 * Kilitleme icin reference manual'de verilen adimlarin yapilmasi gereklidir.
 * @param GPIOx = GPIO Port Base Address
 *
 * @param pinNumber = GPIO Pin Numbers 0 - 15
 *
 * @retval void
 */

void GPIO_LockPin(GPIO_TypeDef_t *GPIOx, uint16_t pinNumber)
{
	 // 16. biti 1 olan, geri kalan kismi istenen pinler (pinNumber).
	uint32_t tempValue = (0x1 << 16) | pinNumber;

	/* Toplamda dort adımdan oluşur. */
	GPIOx->LCKR = tempValue; // LCKR[16] = '1' , LCKR[15:0] = DATA
	GPIOx->LCKR = pinNumber; // LCKR[16] = '0' , LCKR[15:0] = DATA
	GPIOx->LCKR = tempValue; // LCKR[16] = '1' , LCKR[15:0] = DATA
	tempValue = GPIOx->LCKR; // Lock register okunur.


}

/**
 * @brief GPIO_TogglePin, toggles the pin of GPIOx Port
 *
 * @param GPIOx = GPIO Port Base Address
 *
 *  @param pinNumber = GPIO Pin Numbers 0 - 15
 *
 *  @retval void
 */

void GPIO_TogglePin(GPIO_TypeDef_t *GPIOx, uint16_t pinNumber)
{
	uint32_t tempODRRegister = GPIOx->ODR; // Tum pinlerin durumlari (set veya reset) temp degiskene atilir.

	/*
	 * tempODRRegister degerinde hangi pinlerin set hangilerinin reset oldugu ogrenildi.
	 * İlk and islemi ile girilen pinlerden hangilerinin set durumda oldugu ogrenildi.
	 * Ardindan 16 bit kaydirilip BSRR registerinin reset kismina yazdirilarak reset edildi.
	 * ~tempODRRegister degerinde ise setler reset, resetler set duruma getirildi.
	 * ~tempODRRegister ile pinNumber andlenerek reset olan pinler set edilmis oldu.
	 * Bu iki deger ORlanarak toggle islemi gerceklestirilmis oldu.
	 */
	GPIOx->BSRR = ( (tempODRRegister & pinNumber) << 16U ) | (~tempODRRegister & pinNumber);
}





















