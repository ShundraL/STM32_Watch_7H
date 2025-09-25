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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include "display.h"

int8_t second = 0;
int8_t minute = 9;
int8_t hour = 22;
int8_t interrupt_cnt = 100;
int8_t beep_duration;
int8_t kbd_temp = 0;
int8_t kbd_code = 0;
int8_t setup_tmr = 0;
uint32_t system_time = 0;

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

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
//  while(1)
//  {
//  HAL_GPIO_WritePin(RD_GPIO_Port,RD_Pin,GPIO_PIN_RESET);
//  HAL_GPIO_WritePin(RD_GPIO_Port, RD_Pin,GPIO_PIN_SET);
//  }


//  Delay(3);
  Send_command(BIAS_COM);
//  Send_command(SYS_DIS);
  Send_command(TONE_2K);
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
//	go_to_stop_mode();
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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV16;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, DATA_Pin|WR_Pin|RD_Pin|CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : Key2_Pin */
  GPIO_InitStruct.Pin = Key2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(Key2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : IRQ_Pin Key1_Pin Key3_Pin */
  GPIO_InitStruct.Pin = IRQ_Pin|Key1_Pin|Key3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : Bat_Level_Pin */
  GPIO_InitStruct.Pin = Bat_Level_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Bat_Level_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : DATA_Pin WR_Pin RD_Pin CS_Pin */
  GPIO_InitStruct.Pin = DATA_Pin|WR_Pin|RD_Pin|CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : TEMP_Pin */
  GPIO_InitStruct.Pin = TEMP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(TEMP_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PA9 PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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
