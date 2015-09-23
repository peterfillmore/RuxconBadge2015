#include "stm32f0xx_conf.h"

void SysTick_Handler(void) {
  static uint16_t tick = 0;

  switch (tick++) {
  	case 50:
        GPIOA->ODR ^=(1<<8); 
        break;
  	case 100:
        GPIOA->ODR ^=(1<<9); 
        break;
  	case 150:
  		tick = 0;
        GPIOA->ODR ^=(1<<10); 
        //GPIOA->ODR ^= 0x700;
  		//GPIOA->ODR ^= (1 << 9);
  		//GPIOA->ODR ^= (1 << 10);
  		//GPIOB->ODR ^= (1 << 5);
  		break;
  }
}

int main(void)
{

	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; 	// enable the clock to GPIOC
	//RCC->AHBENR |= RCC_AHBENR_GPIOBEN; 	// enable the clock to GPIOC
						//(RM0091 lists this as IOPCEN, not GPIOCEN)
    //GPIOB->MODER = (1 << 10);
    GPIOA->MODER |= 0x150000;
    //GPIOA->MODER = (1 << 16);
    //GPIOA->MODER = (1 << 18);
    //GPIOA->MODER = (1 << 20);
    //GPIOA->ODR = (1 << 8);
    GPIOA->PUPDR |= 0x2a0000; 
    //GPIOA->PUPDR = (1<<21); 
    //GPIOA->PUPDR = (1<<23); 
  	GPIOA->ODR |= 0x700;
  	//GPIOA->ODR = (1 << 10);
  	//GPIOA->ODR = (1 << 11);
    //GPIOA->OTYPER = (1 << 8);
    //GPIOA->OTYPER = (1 << 9);
    //GPIOA->OTYPER = (1 << 10);

	SysTick_Config(SystemCoreClock/100);

	while(1);

}
