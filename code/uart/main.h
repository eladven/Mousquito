#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <avr/wdt.h>

#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))

#define SYSCLK  16000000

#define TRUE 1
#define FALSE 0


#endif 