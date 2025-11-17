/*
 * display.c
 *
 *  Created on: 7 Jul 2022
 *  Author: oleksandr.levchenko
 */
#include "main.h"
#include "display.h"
#include "global_vars.h"
#include "stm32f0xx_hal.h"

/* [LCD digit number][digit code]*/
uint16_t lcd_codes[][4][7] =
{
    {                                                                   // 0
        {A1|SEG_ON,B1|SEG_ON,C1|SEG_ON,D1|SEG_ON,E1|SEG_ON,F1|SEG_ON,G1|SEG_OFF},
        {A2|SEG_ON,B2|SEG_ON,C2|SEG_ON,D2|SEG_ON,E2|SEG_ON,F2|SEG_ON,G2|SEG_OFF},
        {A3|SEG_ON,B3|SEG_ON,C3|SEG_ON,D3|SEG_ON,E3|SEG_ON,F3|SEG_ON,G3|SEG_OFF},
        {A4|SEG_ON,B4|SEG_ON,C4|SEG_ON,D4|SEG_ON,E4|SEG_ON,F4|SEG_ON,G4|SEG_OFF}
    },
    {                                                                   // 1
        {A1|SEG_OFF,B1|SEG_ON,C1|SEG_ON,D1|SEG_OFF,E1|SEG_OFF,F1|SEG_OFF,G1|SEG_OFF},
        {A2|SEG_OFF,B2|SEG_ON,C2|SEG_ON,D2|SEG_OFF,E2|SEG_OFF,F2|SEG_OFF,G2|SEG_OFF},
        {A3|SEG_OFF,B3|SEG_ON,C3|SEG_ON,D3|SEG_OFF,E3|SEG_OFF,F3|SEG_OFF,G3|SEG_OFF},
        {A4|SEG_OFF,B4|SEG_ON,C4|SEG_ON,D4|SEG_OFF,E4|SEG_OFF,F4|SEG_OFF,G4|SEG_OFF},
    },
    {                                                                   // 2
        {A1|SEG_ON,B1|SEG_ON,C1|SEG_OFF,D1|SEG_ON,E1|SEG_ON,F1|SEG_OFF,G1|SEG_ON},
        {A2|SEG_ON,B2|SEG_ON,C2|SEG_OFF,D2|SEG_ON,E2|SEG_ON,F2|SEG_OFF,G2|SEG_ON},
        {A3|SEG_ON,B3|SEG_ON,C3|SEG_OFF,D3|SEG_ON,E3|SEG_ON,F3|SEG_OFF,G3|SEG_ON},
        {A4|SEG_ON,B4|SEG_ON,C4|SEG_OFF,D4|SEG_ON,E4|SEG_ON,F4|SEG_OFF,G4|SEG_ON},
    },

    {                                                                   // 3
        {A1|SEG_ON,B1|SEG_ON,C1|SEG_ON,D1|SEG_ON,E1|SEG_OFF,F1|SEG_OFF,G1|SEG_ON},
        {A2|SEG_ON,B2|SEG_ON,C2|SEG_ON,D2|SEG_ON,E2|SEG_OFF,F2|SEG_OFF,G2|SEG_ON},
        {A3|SEG_ON,B3|SEG_ON,C3|SEG_ON,D3|SEG_ON,E3|SEG_OFF,F3|SEG_OFF,G3|SEG_ON},
        {A4|SEG_ON,B4|SEG_ON,C4|SEG_ON,D4|SEG_ON,E4|SEG_OFF,F4|SEG_OFF,G4|SEG_ON},
    },
    {                                                                   // 4
        {A1|SEG_OFF,B1|SEG_ON,C1|SEG_ON,D1|SEG_OFF,E1|SEG_OFF,F1|SEG_ON,G1|SEG_ON},
        {A2|SEG_OFF,B2|SEG_ON,C2|SEG_ON,D2|SEG_OFF,E2|SEG_OFF,F2|SEG_ON,G2|SEG_ON},
        {A3|SEG_OFF,B3|SEG_ON,C3|SEG_ON,D3|SEG_OFF,E3|SEG_OFF,F3|SEG_ON,G3|SEG_ON},
        {A4|SEG_OFF,B4|SEG_ON,C4|SEG_ON,D4|SEG_OFF,E4|SEG_OFF,F4|SEG_ON,G4|SEG_ON},
    },
    {                                                                   // 5
        {A1|SEG_ON,B1|SEG_OFF,C1|SEG_ON,D1|SEG_ON,E1|SEG_OFF,F1|SEG_ON,G1|SEG_ON},
        {A2|SEG_ON,B2|SEG_OFF,C2|SEG_ON,D2|SEG_ON,E2|SEG_OFF,F2|SEG_ON,G2|SEG_ON},
        {A3|SEG_ON,B3|SEG_OFF,C3|SEG_ON,D3|SEG_ON,E3|SEG_OFF,F3|SEG_ON,G3|SEG_ON},
        {A4|SEG_ON,B4|SEG_OFF,C4|SEG_ON,D4|SEG_ON,E4|SEG_OFF,F4|SEG_ON,G4|SEG_ON},
    },
    {                                                                   // 6
        {A1|SEG_ON,B1|SEG_OFF,C1|SEG_ON,D1|SEG_ON,E1|SEG_ON,F1|SEG_ON,G1|SEG_ON},
        {A2|SEG_ON,B2|SEG_OFF,C2|SEG_ON,D2|SEG_ON,E2|SEG_ON,F2|SEG_ON,G2|SEG_ON},
        {A3|SEG_ON,B3|SEG_OFF,C3|SEG_ON,D3|SEG_ON,E3|SEG_ON,F3|SEG_ON,G3|SEG_ON},
        {A4|SEG_ON,B4|SEG_OFF,C4|SEG_ON,D4|SEG_ON,E4|SEG_ON,F4|SEG_ON,G4|SEG_ON},
    },
    {                                                                   // 7
        {A1|SEG_ON,B1|SEG_ON,C1|SEG_ON,D1|SEG_OFF,E1|SEG_OFF,F1|SEG_OFF,G1|SEG_OFF},
        {A2|SEG_ON,B2|SEG_ON,C2|SEG_ON,D2|SEG_OFF,E2|SEG_OFF,F2|SEG_OFF,G2|SEG_OFF},
        {A3|SEG_ON,B3|SEG_ON,C3|SEG_ON,D3|SEG_OFF,E3|SEG_OFF,F3|SEG_OFF,G3|SEG_OFF},
        {A4|SEG_ON,B4|SEG_ON,C4|SEG_ON,D4|SEG_OFF,E4|SEG_OFF,F4|SEG_OFF,G4|SEG_OFF},
    },
    {                                                                   // 8
        {A1|SEG_ON,B1|SEG_ON,C1|SEG_ON,D1|SEG_ON,E1|SEG_ON,F1|SEG_ON,G1|SEG_ON},
        {A2|SEG_ON,B2|SEG_ON,C2|SEG_ON,D2|SEG_ON,E2|SEG_ON,F2|SEG_ON,G2|SEG_ON},
        {A3|SEG_ON,B3|SEG_ON,C3|SEG_ON,D3|SEG_ON,E3|SEG_ON,F3|SEG_ON,G3|SEG_ON},
        {A4|SEG_ON,B4|SEG_ON,C4|SEG_ON,D4|SEG_ON,E4|SEG_ON,F4|SEG_ON,G4|SEG_ON},
    },
    {                                                                   // 9
        {A1|SEG_ON,B1|SEG_ON,C1|SEG_ON,D1|SEG_ON,E1|SEG_OFF,F1|SEG_ON,G1|SEG_ON},
        {A2|SEG_ON,B2|SEG_ON,C2|SEG_ON,D2|SEG_ON,E2|SEG_OFF,F2|SEG_ON,G2|SEG_ON},
        {A3|SEG_ON,B3|SEG_ON,C3|SEG_ON,D3|SEG_ON,E3|SEG_OFF,F3|SEG_ON,G3|SEG_ON},
        {A4|SEG_ON,B4|SEG_ON,C4|SEG_ON,D4|SEG_ON,E4|SEG_OFF,F4|SEG_ON,G4|SEG_ON},
    },
    {                                                                   // 10 space
        {A1|SEG_OFF,B1|SEG_OFF,C1|SEG_OFF,D1|SEG_OFF,E1|SEG_OFF,F1|SEG_OFF,G1|SEG_OFF},
        {A2|SEG_OFF,B2|SEG_OFF,C2|SEG_OFF,D2|SEG_OFF,E2|SEG_OFF,F2|SEG_OFF,G2|SEG_OFF},
        {A3|SEG_OFF,B3|SEG_OFF,C3|SEG_OFF,D3|SEG_OFF,E3|SEG_OFF,F3|SEG_OFF,G3|SEG_OFF},
        {A4|SEG_OFF,B4|SEG_OFF,C4|SEG_OFF,D4|SEG_OFF,E4|SEG_OFF,F4|SEG_OFF,G4|SEG_OFF},
    },
    {                                                                 // 11 H
        {A1|SEG_OFF,B1|SEG_ON,C1|SEG_ON,D1|SEG_OFF,E1|SEG_ON,F1|SEG_ON,G1|SEG_ON},
        {A2|SEG_OFF,B2|SEG_ON,C2|SEG_ON,D2|SEG_OFF,E2|SEG_ON,F2|SEG_ON,G2|SEG_ON},
        {A3|SEG_OFF,B3|SEG_ON,C3|SEG_ON,D3|SEG_OFF,E3|SEG_ON,F3|SEG_ON,G3|SEG_ON},
        {A4|SEG_OFF,B4|SEG_ON,C4|SEG_ON,D4|SEG_OFF,E4|SEG_ON,F4|SEG_ON,G4|SEG_ON},
    },
    {                                                                 // 12 L
        {A1|SEG_OFF,B1|SEG_OFF,C1|SEG_OFF,D1|SEG_ON,E1|SEG_ON,F1|SEG_ON,G1|SEG_OFF},
        {A2|SEG_OFF,B2|SEG_OFF,C2|SEG_OFF,D2|SEG_ON,E2|SEG_ON,F2|SEG_ON,G2|SEG_OFF},
        {A3|SEG_OFF,B3|SEG_OFF,C3|SEG_OFF,D3|SEG_ON,E3|SEG_ON,F3|SEG_ON,G3|SEG_OFF},
        {A4|SEG_OFF,B4|SEG_OFF,C4|SEG_OFF,D4|SEG_ON,E4|SEG_ON,F4|SEG_ON,G4|SEG_OFF},
    },
    {                                                                 // 13 G
        {A1|SEG_ON,B1|SEG_OFF,C1|SEG_ON,D1|SEG_ON,E1|SEG_ON,F1|SEG_ON,G1|SEG_OFF},
        {A2|SEG_ON,B2|SEG_OFF,C2|SEG_ON,D2|SEG_ON,E2|SEG_ON,F2|SEG_ON,G2|SEG_OFF},
        {A3|SEG_ON,B3|SEG_OFF,C3|SEG_ON,D3|SEG_ON,E3|SEG_ON,F3|SEG_ON,G3|SEG_OFF},
        {A4|SEG_ON,B4|SEG_OFF,C4|SEG_ON,D4|SEG_ON,E4|SEG_ON,F4|SEG_ON,G4|SEG_OFF},
    },
    {                                                                 // 14 a
        {A1|SEG_ON,B1|SEG_ON,C1|SEG_ON,D1|SEG_ON,E1|SEG_ON,F1|SEG_OFF,G1|SEG_ON},
        {A2|SEG_ON,B2|SEG_ON,C2|SEG_ON,D2|SEG_ON,E2|SEG_ON,F2|SEG_OFF,G2|SEG_ON},
        {A3|SEG_ON,B3|SEG_ON,C3|SEG_ON,D3|SEG_ON,E3|SEG_ON,F3|SEG_OFF,G3|SEG_ON},
        {A4|SEG_ON,B4|SEG_ON,C4|SEG_ON,D4|SEG_ON,E4|SEG_ON,F4|SEG_OFF,G4|SEG_ON},
    },
    {                                                                 // Degree
        {A1|SEG_ON,B1|SEG_ON,C1|SEG_OFF,D1|SEG_OFF,E1|SEG_OFF,F1|SEG_ON,G1|SEG_ON},
        {A2|SEG_ON,B2|SEG_ON,C2|SEG_OFF,D2|SEG_OFF,E2|SEG_OFF,F2|SEG_ON,G2|SEG_ON},
        {A3|SEG_ON,B3|SEG_ON,C3|SEG_OFF,D3|SEG_OFF,E3|SEG_OFF,F3|SEG_ON,G3|SEG_ON},
        {A4|SEG_ON,B4|SEG_ON,C4|SEG_OFF,D4|SEG_OFF,E4|SEG_OFF,F4|SEG_ON,G4|SEG_ON},
    },
    {                                                                // - Minus
        {A1|SEG_OFF,B1|SEG_OFF,C1|SEG_OFF,D1|SEG_OFF,E1|SEG_OFF,F1|SEG_OFF,G1|SEG_ON},
        {A2|SEG_OFF,B2|SEG_OFF,C2|SEG_OFF,D2|SEG_OFF,E2|SEG_OFF,F2|SEG_OFF,G2|SEG_ON},
        {A3|SEG_OFF,B3|SEG_OFF,C3|SEG_OFF,D3|SEG_OFF,E3|SEG_OFF,F3|SEG_OFF,G3|SEG_ON},
        {A4|SEG_OFF,B4|SEG_OFF,C4|SEG_OFF,D4|SEG_OFF,E4|SEG_OFF,F4|SEG_OFF,G4|SEG_ON}
    }
};


void Delay(int16_t time)
{
	int16_t k;
	while(time)
	{
		for(k=0;k<6500;k++)
		{
		}
	time--;
	}
};

void Send_data( uint16_t data)
{
	LL_GPIO_SetOutputPin(WR_GPIO_Port, WR_Pin);
	LL_GPIO_ResetOutputPin(CS_GPIO_Port, CS_Pin);
	__NOP();
	for ( int8_t i = 12; i >=0 ; i--)
	{
		  LL_GPIO_ResetOutputPin(WR_GPIO_Port, WR_Pin);
		  if (data & (1<<i))
		  {
			  LL_GPIO_SetOutputPin(DATA_GPIO_Port, DATA_Pin);
		  }
		  else
		  {
			  LL_GPIO_ResetOutputPin(DATA_GPIO_Port, DATA_Pin);
		  }
		  LL_GPIO_SetOutputPin(WR_GPIO_Port, WR_Pin);
	}
	LL_GPIO_SetOutputPin(CS_GPIO_Port, CS_Pin);
};

void Send_command( uint16_t data)
{

	LL_GPIO_SetOutputPin(WR_GPIO_Port, WR_Pin);
	LL_GPIO_ResetOutputPin(CS_GPIO_Port, CS_Pin);
	__NOP();
	for ( int8_t i = 11; i >=0 ; i--)
	{
		LL_GPIO_ResetOutputPin(WR_GPIO_Port, WR_Pin);
		if (data & (1<<i))
		{
		  LL_GPIO_SetOutputPin(DATA_GPIO_Port, DATA_Pin);
		}
		else
		{
		  LL_GPIO_ResetOutputPin(DATA_GPIO_Port, DATA_Pin);
		}

		LL_GPIO_SetOutputPin(WR_GPIO_Port, WR_Pin);
	}
	LL_GPIO_SetOutputPin(CS_GPIO_Port, CS_Pin);
};


void Clear_display ( void)
{
	/* Successive Address Writing is used */
	/* 101 WRITE command 0 ADDR 0 DATA_Pin */
	uint16_t data = 0b0001010000000000;
	LL_GPIO_SetOutputPin(WR_GPIO_Port, WR_Pin);
	LL_GPIO_ResetOutputPin(CS_GPIO_Port, CS_Pin);
	__NOP();
	for ( int8_t i = 12; i >=0 ; i--)
	{
	  LL_GPIO_ResetOutputPin(WR_GPIO_Port, WR_Pin);
	  if (data & (1<<i))
	  {
		  LL_GPIO_SetOutputPin(DATA_GPIO_Port, DATA_Pin);
	  }
	  else
	  {
		  LL_GPIO_ResetOutputPin(DATA_GPIO_Port, DATA_Pin);
	  }
	  LL_GPIO_SetOutputPin(WR_GPIO_Port, WR_Pin);
	}

	LL_GPIO_ResetOutputPin(DATA_GPIO_Port, DATA_Pin);
	LL_GPIO_SetOutputPin(WR_GPIO_Port, WR_Pin);
	for (uint8_t k=0; k<127; k++)
	{
		LL_GPIO_ResetOutputPin(WR_GPIO_Port, WR_Pin);
		LL_GPIO_SetOutputPin(WR_GPIO_Port, WR_Pin);
	}

	LL_GPIO_SetOutputPin(CS_GPIO_Port, CS_Pin);
};

void Write_segment_data(uint8_t digit, uint8_t segment)
{
	for (uint8_t i = 0; i < 7; i++)
	{
		Send_data(lcd_codes[digit][segment][i]);
	}
};

void Display_Upd(int8_t mode)
{

	if(mode == FULL_UPD)
	{
		display_buffer[HOUR_H] = hour/10;
		display_buffer[HOUR_L] = hour%10;
		display_buffer[MIN_H] = minute/10;
		display_buffer[MIN_L] = minute%10;
		for (int8_t i = HOUR_H; i < 4; i++)
		{
			Write_segment_data(display_buffer[i],i);
		}
	}
	else
	{
		display_buffer[MIN_H] = minute/10;
		display_buffer[MIN_L] = minute%10;
		for (int8_t i = MIN_H; i < 4; i++)
		{
			Write_segment_data(display_buffer[i],i);
		}
	}
};

void Hide_segment(void)
{
	Write_segment_data(BLANK,input_position);
};
void Show_segment(void)
{
	Write_segment_data(display_buffer[input_position],input_position);
};
