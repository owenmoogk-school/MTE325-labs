 
#include "example.h"
#include "example_usart.h"
#include "L6470.h"

int main(void)
{
  /* NUCLEO board initialization */
  NUCLEO_Board_Init();
  
  /* X-NUCLEO-IHM02A1 initialization */
  BSP_Init();
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  
  
  #ifdef NUCLEO_USE_USART
  /* Transmit the initial message to the PC via UART */
  USART_TxWelcomeMessage();
  USART_Transmit(&huart2, "X-CUBE-SPN2 v1.0.0\n\r");
  #endif

  /* Fill the L6470_DaisyChainMnemonic structure */
  Fill_L6470_DaisyChainMnemonic();
	
	/*Initialize the motor parameters */
	Motor_Param_Reg_Init();
  
  L6470_Run(L6470_ID(0), 0, 10000);
  L6470_Run(L6470_ID(1), 0, 10000);
  while (1)
  {
    USART_CheckAppCmd();
  }
}
