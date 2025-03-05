#include "stm32f4xx_it.h"
#include "xnucleoihm02a1_interface.h"
#include "example_usart.h"
#include "l6470.h"

void SysTick_Handler(void)
{
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
}

const int clockSpeed = 84 * 10^6;
const int debounceDelay = 150; // ms...?

void wait(float ms){
  int volatile i = 0;
  while (i < ms * clockSpeed / 2){i++;}
}

// y min
void EXTI0_IRQHandler(void)
{
  wait(debounceDelay);
  if (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0))
  {
    // L6470_Run(L6470_ID(1), 1, 10000);
    L6470_HardStop(L6470_ID(0));
    L6470_HardStop(L6470_ID(1));
  }
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

// y max
void EXTI1_IRQHandler(void)
{
  wait(debounceDelay);
  if (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1))
  {
    // L6470_Run(L6470_ID(0), 0, 10000);
    L6470_HardStop(L6470_ID(0));
    L6470_HardStop(L6470_ID(1));
  }
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}

// x min
void EXTI4_IRQHandler(void)
{
  wait(debounceDelay);
  if (!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4))
  {
    // L6470_Run(L6470_ID(0), 1, 10000);
    L6470_HardStop(L6470_ID(0));
    L6470_HardStop(L6470_ID(1));
  }
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}

// y max
void EXTI9_5_IRQHandler(void)
{
  wait(debounceDelay);
  if (!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7))
  {
    // L6470_Run(L6470_ID(1), 0, 10000);
    L6470_HardStop(L6470_ID(0));
    L6470_HardStop(L6470_ID(1));
  }
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
}

void USART2_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart2);
  USART_ITCharManager(&huart2);
}
