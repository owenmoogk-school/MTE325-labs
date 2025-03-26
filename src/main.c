
#include "example.h"
#include "example_usart.h"
#include "L6470.h"

uint32_t readADC(uint32_t channel)
{
  ADC_ChannelConfTypeDef channelConfig;
  channelConfig.Channel = channel;
  channelConfig.Rank = 1;
  channelConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;

  HAL_ADC_ConfigChannel(&hadc1, &channelConfig);

  HAL_ADC_Start(&hadc1);
  HAL_ADC_PollForConversion(&hadc1, 1000);
  return HAL_ADC_GetValue(&hadc1);
}

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
  else
  {
    L6470_Run(L6470_ID(0), 0, 10000);
    L6470_Run(L6470_ID(1), 0, 10000);
  }

  int32_t volatile adc_value = 0; // Variable to store ADC reading
  int32_t operating_adc_value = 0;
  int32_t const MOTOR_CHANGE_THRESHOLD = 100;
  int32_t const ZERO_SPEED_THRESHOLD = 300;
  int32_t const X_SPEED_MULTIPLIER = 13;
  int32_t const Y_SPEED_MULTIPLIER = 13;
  volatile uint16_t adc_values[2];                // Buffer for ADC readings (0-4095 for 12-bit)
  int32_t const ZERO_SPEED_VALUE = 2048; // half of the adc max value

  while (1)
  {
    USART_CheckAppCmd();
    adc_values[0] = readADC(ADC_CHANNEL_8);
    adc_values[1] = readADC(ADC_CHANNEL_4);

    // if (abs(adc_value - operating_adc_value) > MOTOR_CHANGE_THRESHOLD)
    // {
    //   operating_adc_value = adc_value;

    //   if (abs(operating_adc_value - ZERO_SPEED_VALUE) < ZERO_SPEED_THRESHOLD)
    //   {
    //     L6470_HardStop(L6470_ID(0));
    //   }

    //   else
    //   {
    //     eL6470_DirId_t direction = 0;
    //     if ((operating_adc_value - ZERO_SPEED_VALUE) > 0)
    //     {
    //       direction = 1;
    //     }
    //     L6470_Run(L6470_ID(0), direction, abs((operating_adc_value - ZERO_SPEED_VALUE)) * X_SPEED_MULTIPLIER);
    //   }
    // }

    // y logic
    if (abs(adc_value - operating_adc_value) > MOTOR_CHANGE_THRESHOLD)
    {
      operating_adc_value = adc_values[0];

      if (abs(operating_adc_value - ZERO_SPEED_VALUE) < ZERO_SPEED_THRESHOLD)
      {
        L6470_HardStop(L6470_ID(1));
      }

      else
      {
        eL6470_DirId_t direction = 0;
        if ((operating_adc_value - ZERO_SPEED_VALUE) > 0)
        {
          direction = 1;
        }
        L6470_Run(L6470_ID(1), direction, abs((operating_adc_value - ZERO_SPEED_VALUE)) * Y_SPEED_MULTIPLIER);
      }
    }
  }
}
