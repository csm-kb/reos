//
// Created by amadeus on 1/13/21.
//

#ifndef REOS_INTERRUPTS_H
#define REOS_INTERRUPTS_H

/* Keyboard interrupt asm wrapper */
extern void keyboard_handler();

/* Clock interrupt asm wrapper */
extern void rtc_handler();

/* PIT interrupt asm wrapper */
extern void pit_handler();

/* System Call asm wrapper */
extern void system_call_handler();

#endif //REOS_INTERRUPTS_H
