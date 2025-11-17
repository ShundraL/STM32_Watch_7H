/*
 * globals_var.h
 *
 *  Created on: Nov 17, 2025
 *      Author: oleksandr.levchenko
 */

#ifndef INC_GLOBAL_VARS_H_
#define INC_GLOBAL_VARS_H_
#include "stm32f0xx_hal.h"

extern uint8_t temperature_buffer[4];
extern uint8_t display_buffer[4];

extern uint16_t flags;
extern uint8_t kbd_scan;
extern int8_t input_position;

extern int8_t second,minute,hour;
extern int8_t interrupt_cnt;
extern int8_t beep_duration;
extern int8_t kbd_temp, kbd_code;
extern int8_t setup_tmr;
extern uint32_t system_time;

#endif /* INC_GLOBAL_VARS_H_ */
