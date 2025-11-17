/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include "display.h"
#include "global_vars.h"

//#define PCB_Rev_V1

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */
static void Beep_Enable(int8_t duration);
static void Beep_Disable(void);
static void Separator_Blink(void);
static void Clock_Routine(void);
static void Beep_Routine(void);
static void Keyboard_Routine(void);
static void Input_data(int8_t position);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* Stop SysTick & disable interrupt */
  SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */

  Delay(3);
  Send_command(BIAS_COM);
  Send_command(SYS_DIS);
#ifndef PCB_Rev_V1
  Send_command(XTAL_32K);
#endif
  Send_command(TONE_2K);
  Send_command(FREQ_1HZ);
  Send_command(WDT_DIS);
  Send_command(TIMER_DIS);
  Send_command(CLR_TIMER);
  Send_command(TIMER_EN);
  Send_command(IRQ_EN);

  Send_command(SYS_EN);
  Send_command(LCD_ON);

  MX_TIM1_Init();
  HAL_TIM_Base_Start_IT(&htim1);

  Clear_display();
  Display_Upd(FULL_UPD);
  Beep_Enable(7);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	go_to_sleep();
	if (flags & (1<<BEEP_SYNC))
	{
		flags &=~(1<<BEEP_SYNC);
		if (flags & (1<<BEEP_MODE))
		{
			Beep_Routine();
		}
	}
	if (flags & (1<<KEYB))
	{
		Keyboard_Routine();
	}
	if (!(flags & (1<<SETUP)))
	{
		if (flags & (1<<HALF_SEC))
		{
			flags &=~(1<<HALF_SEC);
			Separator_Blink();
		}
		if (flags & (1<<ONE_SEC))
		{
			flags &=~(1<<ONE_SEC);
			Clock_Routine();
		} // ONE_SEC loop
	} //NOT SETUP MODE loop
	else
	{

		if(!(flags & (1<<EDIT_TIME)))
		{
			setup_tmr = SETUP_TMR;
			flags |= (1<<EDIT_TIME);
			Input_data(HOUR_L);
			Send_data(DDOT|MASK_A);		// it shows DDOT
		}
		if (flags & (1<<HALF_SEC))
		{
			flags &=~(1<<HALF_SEC);
			Hide_segment();
		}
		if(flags & (1<<ONE_SEC))
		{
			Show_segment();
			flags &=~(1<<ONE_SEC);
			setup_tmr--;
			if(!setup_tmr)
			{
				flags &= ~(1<<SETUP);
				flags &= ~(1<<EDIT_TIME);
				Display_Upd(FULL_UPD);
			}
		}
	} //SETUP MODE loop
  } //endless loop
}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1)
    {
    	system_time++;
    	interrupt_cnt++;
    	if(interrupt_cnt == SEC_CNT)
    	{
    		interrupt_cnt = 0;
    		flags |=(1<<ONE_SEC);
    		flags |=(1<<HALF_SEC);
    	}
    	if(interrupt_cnt == HALF_SEC_CNT)
    	{
    		flags |=(1<<HALF_SEC);
    	}
       	flags |=(1<<BEEP_SYNC);
       	kbd_scan =
		(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) << KEY1) |
		(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) << KEY2) |
	    (!HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_1) << KEY3);
       	if (kbd_scan != 0)
       	{
       		if (!(flags & (1<<BEEP_MODE)))
       		{
           		Beep_Enable(3);
       		}
       		else
       		{
       			beep_duration++;
       		}
       		flags |=(1<<KEYB);
       	}
    }
};


void go_to_sleep(void)
{
//    HAL_SuspendTick();                     // Stop SysTick if not needed for wake-up
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;     // Select SLEEP mode (not deep sleep)
    __WFI();                                // Wait For Interrupt
//    HAL_ResumeTick();                       // Resume SysTick after wake-up
};

void Beep_Enable(int8_t duration)
{
	beep_duration = duration;
	Send_command(TONE_ON);
	flags |=(1<<BEEP_MODE);
};

void Beep_Disable(void)
{
	flags &=~(1<<BEEP_MODE);
	Send_command(TONE_OFF);
};

void Separator_Blink(void)
{
	if (flags & (1<<DOTS))
	{
		flags &=~(1<<DOTS);
		Send_data(DDOT|MASK_B);
	}
	else
	{
		flags |=(1<<DOTS);
		Send_data(DDOT|MASK_A);
	}
};
void Clock_Routine(void)
{
	second++;
	if (second == SECOND_LIMIT)
	{
		second = 0;
		minute++;
		if (minute == MINUTE_LIMIT)
		{
			minute = 0;
			hour++;
			if ((hour>=8) & (hour<22))
			{
				Beep_Enable(3);
			}
			if (hour == HOUR_LIMIT)
			{
				hour = 0;
			}
			Display_Upd(FULL_UPD);
		}
		else
		{
			Display_Upd(HALF_UPD);
		}
	}
};

void Beep_Routine(void)
{
	beep_duration--;
	if(!beep_duration)
	{
		Beep_Disable();
	}
};

void Keyboard_Routine(void)
{
   	flags &=~(1<<KEYB);
   	flags |= (1<<SETUP);
};

static void Input_data(int8_t position)
{
	input_position = position;
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_2);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_16);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {

  }
  LL_SetSystemCoreClock(4000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 500;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 9;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

  /**/
  LL_GPIO_ResetOutputPin(DATA_GPIO_Port, DATA_Pin);

  /**/
  LL_GPIO_ResetOutputPin(WR_GPIO_Port, WR_Pin);

  /**/
  LL_GPIO_ResetOutputPin(RD_GPIO_Port, RD_Pin);

  /**/
  LL_GPIO_ResetOutputPin(CS_GPIO_Port, CS_Pin);

  /**/
  GPIO_InitStruct.Pin = Key2_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(Key2_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = IRQ_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(IRQ_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = Bat_Level_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(Bat_Level_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = DATA_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(DATA_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = WR_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(WR_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = RD_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(RD_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = CS_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(CS_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = Key1_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(Key1_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = Key3_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(Key3_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = TEMP_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(TEMP_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
//void go_to_stop_mode(void)
//{
//    HAL_SuspendTick();
//    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
//    HAL_ResumeTick();
//}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
