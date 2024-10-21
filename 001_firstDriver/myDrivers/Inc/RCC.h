/*
 * RCC.h
 *
 *  Created on: 4 Mar 2024
 *      Author: VeliSacikara
 */

#ifndef INC_RCC_H_
#define INC_RCC_H_


#include "stm32f407xx.h"

/*
 * RCC AHB1 Peripherals Clock Control Macro Definitions
 *
 *
 */


// do-while yapisi ile tek satirda yapilan tanimlama islemi birden fazla satira genisletilmis oldu.
#define RCC_GPIOA_CLK_ENABLE()		do{		uint32_t tempValue = 0; \
											SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);  /* Ilgili bit set edildi. */\
											tempValue = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN); /* Biraz zaman gecmesini beklemek icin ayni bit icin okuma yapildi. */\
											UNUSED(tempValue); /* Kullanilmayan degisken warningi vermemesi icin bu sekilde hicbir islem yapmayan macro tanimlandi. */\
									  }while(0)

#define RCC_GPIOB_CLK_ENABLE()		do{		uint32_t tempValue = 0; \
										  	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN); \
										  	tempValue = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN); \
										  	UNUSED(tempValue); \
									}while(0)

#define RCC_GPIOC_CLK_ENABLE()		do{		uint32_t tempValue = 0; \
										  	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN); \
										  	tempValue = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN); \
										  	UNUSED(tempValue); \
									}while(0)

#define RCC_GPIOD_CLK_ENABLE()		do{		uint32_t tempValue = 0; \
										  	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN); \
										  	tempValue = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN); \
										  	UNUSED(tempValue); \
									}while(0)

#define RCC_GPIOE_CLK_ENABLE()		do{		uint32_t tempValue = 0; \
										  	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOEEN); \
										  	tempValue = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOEEN); \
										  	UNUSED(tempValue); \
									}while(0)


#define RCC_GPIOA_CLK_DISABLE()		CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN)

#define RCC_GPIOB_CLK_DISABLE()		CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN)

#define RCC_GPIOC_CLK_DISABLE()		CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN)

#define RCC_GPIOD_CLK_DISABLE()		CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN)

#define RCC_GPIOE_CLK_DISABLE()		CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOEEN)






#endif /* INC_RCC_H_ */
