/*
 * display.h
 *
 *  Created on: 7 Jul 2022
 *  Author: oleksandr.levchenko
 */

#include "stm32f0xx_ll_rcc.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_system.h"
#include "stm32f0xx_ll_exti.h"
#include "stm32f0xx_ll_cortex.h"
#include "stm32f0xx_ll_utils.h"
#include "stm32f0xx_ll_pwr.h"
#include "stm32f0xx_ll_dma.h"
#include "stm32f0xx_ll_gpio.h"

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

/* Segment RAM matching */
#define G1  (0)<<4
#define F1  (1)<<4
#define A1  (2)<<4
#define B1  (3)<<4
#define G2  (4)<<4
#define F2  (5)<<4
#define A2  (6)<<4
#define B2  (7)<<4
#define A4  (8)<<4
#define F4  (9)<<4
#define G4  (10)<<4
#define B3  (11)<<4
#define A3  (12)<<4
#define F3  (13)<<4
#define G3  (14)<<4
#define DDOT (15)<<4
#define B4  (16)<<4
#define C4  (17)<<4
#define D4  (18)<<4
#define E4  (19)<<4
#define DP3 (20)<<4
#define C3  (21)<<4
#define D3  (22)<<4
#define E3  (23)<<4
#define DP2 (24)<<4
#define C2  (25)<<4
#define D2  (26)<<4
#define E2  (27)<<4
#define DP1 (28)<<4
#define C1  (29)<<4
#define D1  (30)<<4
#define E1  (31)<<4
#define MASK_A     0b0001010000001100
#define MASK_B     0b0001010000000000
#define SEG_ON     0b0001010000001100
#define SEG_OFF    0b0001010000000000
#define BLANK      17

#define CMD        0b100
#define READ       0b110
#define WRITE      0b101
#define LCD_ON     0b100000000110
#define LCD_OFF    0b100000000100
//#define BIAS_COM 0b100000110000
//#define BIAS_COM 0b100001000000   // BIAS 1\2
#define BIAS_COM   0b100001000010     // BIAS 1\3
#define SYS_EN     0b100000000010
#define SYS_DIS    0b100000000000
#define TONE_4K    0b100010000000
#define TONE_2K    0b100011000000
#define TONE_OFF   0b100000010000
#define TONE_ON    0b100000010010
#define FULL_UPD	0
#define HALF_UPD	1
#define HOUR_H		0
#define HOUR_L		1
#define MIN_H		2
#define MIN_L		3
/* Registers definitions */

/* Function prototypes */
void Send_command(uint16_t data);
void Send_data(uint16_t data);
void Clear_display(void);
void Delay(int16_t delay);
void Write_segment_data (uint8_t digit, uint8_t segment);
void Display_Upd(int8_t mode);
void Hide_segment(void);
void Show_segment(void);


#endif /* INC_DISPLAY_H_ */
