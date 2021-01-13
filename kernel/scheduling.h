//
// Created by amadeus on 1/13/21.
//

#ifndef REOS_SCHEDULING_H
#define REOS_SCHEDULING_H

#include "../libc/types.h"

/* */
#define PIT_IRQ_LINE				0
#define PIT_COMMAND_REG				0x43
#define PIT_SQUARE_WAVE_MODE_3 		0x36
#define PIT_CHANNEL_0 				0x40


/* Divisors for PIT Frequency setting
 * HZ = 1193180 / HZ_VALUE (ex: HZ = 1193180 / 20);
 */
#define _20HZ			11932
#define FREQ_MASK 		0xFF
#define _EIGHT			8


extern volatile uint8_t current_term_executing;

/* Initialize RTC */
void init_PIT(void);

/* PIT Interruption */
void PIT_interrupt_and_schedule(void);

/*Context Switch */
void doContextSwitch(int processNumber);

/* Helper function to get next process to schedule */
int get_next_scheduled_process();

#endif //REOS_SCHEDULING_H
