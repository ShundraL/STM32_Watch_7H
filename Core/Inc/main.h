/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define HOUR_LIMIT   24
#define MINUTE_LIMIT 60
#define SECOND_LIMIT 60
#define SEC_CNT		 100
#define HALF_SEC_CNT SEC_CNT/2
extern int8_t second, minute, hour;
extern uint16_t flags;
extern uint8_t kbd_scan;
extern TIM_HandleTypeDef htim1;
extern int8_t input_position;


/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Key2_Pin GPIO_PIN_1
#define Key2_GPIO_Port GPIOF
#define IRQ_Pin GPIO_PIN_0
#define IRQ_GPIO_Port GPIOA
#define DATA_Pin GPIO_PIN_2
#define DATA_GPIO_Port GPIOA
#define WR_Pin GPIO_PIN_3
#define WR_GPIO_Port GPIOA
#define RD_Pin GPIO_PIN_4
#define RD_GPIO_Port GPIOA
#define CS_Pin GPIO_PIN_5
#define CS_GPIO_Port GPIOA
#define Key1_Pin GPIO_PIN_6
#define Key1_GPIO_Port GPIOA
#define Key3_Pin GPIO_PIN_7
#define Key3_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */
/* flags register bits definitions*/
#define ONE_SEC 	0
#define DOTS    	1
#define KEYB		2
#define BEEP_MODE	3
#define SETUP		4
#define BEEP_SYNC	5
#define HALF_SEC	6
#define EDIT_TIME	7


#define KEY1		0
#define KEY2		1
#define KEY3		2
#define SETUP_TMR 	7


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
