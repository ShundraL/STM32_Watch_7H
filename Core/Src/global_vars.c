/*
 * globals_var.c
 *
 *  Created on: Aug 13, 2025
 *      Author: oleksandr.levchenko
 */
#include "main.h"
#include "display.h"
#include "stm32f0xx_hal.h"
#include "global_vars.h"

int8_t second = 41;
int8_t minute = 02 ;
int8_t hour = 22;
int8_t interrupt_cnt = 100;
int8_t beep_duration;
int8_t kbd_temp = 0;
int8_t kbd_code = 0;
int8_t setup_tmr = 0;
uint32_t system_time = 0;

uint16_t flags = 0;
uint8_t kbd_scan = 0;
int8_t input_position = HOUR_H;
uint8_t display_buffer[4]={0,0,0,0};
uint8_t temperature_buffer[4]={DASH,DASH,DASH,DEGREE};





