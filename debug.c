/*
 * Copyright (C) 2019 saybur
 * 
 * This file is part of scuznet.
 * 
 * scuznet is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * scuznet is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with scuznet.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <util/delay.h>
#include "config.h"
#include "debug.h"

void fatal(uint8_t lflash, uint8_t sflash)
{
	// disable all but high-level (/RST) interrupts
	PMIC.CTRL = PMIC_HILVLEN_bm;
	// disable the watchdog timer
	__asm__ __volatile__(
		"ldi r24, %0"		"\n\t"
		"out %1, r24"		"\n\t"
		"ldi r24, %2"		"\n\t"
		"sts %3, r24"		"\n\t"
		:
		: "M" (CCP_IOREG_gc), "i" (&CCP),
		  "M" (WDT_CEN_bm), "i" (&(WDT.CTRL))
		: "r24"
		);

	// report to the debugger
	debug(DEBUG_FATAL);
	debug_dual(lflash, sflash);

	// begin flash pattern
	led_off();
	_delay_ms(LED_BREAK);
	while (1)
	{
		for (uint8_t i = 0; i < lflash; i++)
		{
			led_on();
			_delay_ms(LED_LONG_FLASH);
			led_off();
			_delay_ms(LED_LONG_FLASH);
		}
		for (uint8_t i = 0; i < sflash; i++)
		{
			led_on();
			_delay_ms(LED_SHORT_FLASH);
			led_off();
			_delay_ms(LED_SHORT_FLASH);
		}
		_delay_ms(LED_BREAK);
	}
}
