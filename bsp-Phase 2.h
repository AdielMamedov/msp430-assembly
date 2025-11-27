#ifndef _bsp_H
#define _bsp_H

#include <msp430g2553.h>

#define DataSegStart 0x0200
#define CodeSegStart 0xC000
#define StackTosStart 0x0400

#define   debounceVal      250

#define del200us 210        
#define del15ms  15728
#define del5ms   5242


//LCD abstraction
#define LCDArrPort      &P1IN
#define LCDArrPortOut   &P1OUT
#define LCDArrPortDir   &P1DIR
#define LCDArrPortsel   &P1SEL

#define ENctrl 	 0x20
#define RSctrl 	 0x40
#define RWctrl 	 0x80

//Push-Buttons Abstractions
#define PBArrPort      &P2IN
#define PBArrPortOut   &P2OUT
#define PBTypeOfTriger &P2IES
#define PBEnableIE     &P2IE
#define PBFlugPend     &P2IFG
#define PBArrPortDir   &P2DIR
#define PBArrPortsel   &P2SEL
#define PB0    0x01
#define PB1    0x02
#define PB2    0x04


#endif
