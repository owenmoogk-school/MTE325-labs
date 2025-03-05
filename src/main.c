 
#include "example.h"
#include "example_usart.h"
#include "L6470.h"

int main(void)
{
  /* NUCLEO board initialization */
  NUCLEO_Board_Init();
  
  /* X-NUCLEO-IHM02A1 initialization */
  BSP_Init();
  L6470_HardStop(L6470_ID(0));
  L6470_HardStop(L6470_ID(1));

  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();

#ifdef NUCLEO_USE_USART
  /* Transmit the initial message to the PC via UART */
  USART_TxWelcomeMessage();
  USART_Transmit(&huart2, "X-CUBE-SPN2 v1.0.0\n\r");
  #endif

  /* Fill the L6470_DaisyChainMnemonic structure */
  Fill_L6470_DaisyChainMnemonic();
	
	/*Initialize the motor parameters */
	Motor_Param_Reg_Init();


  if (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) || !HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7) || !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) || !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4))
  {
    L6470_HardStop(L6470_ID(0));
    L6470_HardStop(L6470_ID(1));
  }
  else{ 
    L6470_Run(L6470_ID(0), 0, 10000);
    L6470_Run(L6470_ID(1), 0, 10000);
  }

  uint32_t volatile adc_value = 0; // Variable to store ADC reading
  
  while (1)
  {
    USART_CheckAppCmd();
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 1000) == HAL_OK)
    {
      // Get ADC value (0-4095 for 12-bit resolution)
      adc_value = HAL_ADC_GetValue(&hadc1);
      char string[1000];
      itoa(adc_value, string, 16);
      USART_Transmit(&huart2, string);
      USART_Transmit(&huart2, "HELLO I AM UNDER THE WATER");
    }
  }
}
