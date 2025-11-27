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
#define LCDArrPort      &P2IN
#define LCDArrPortOut   &P2OUT
#define LCDArrPortDir   &P2DIR
#define LCDArrPortsel   &P2SEL

#define ENctrl 	 0x20
#define RSctrl 	 0x40
#define RWctrl 	 0x80

//Push-Buttons Abstractions
#define PBArrPort      &P1IN
#define PBArrPortOut   &P1OUT
#define PBTypeOfTriger &P1IES
#define PBEnableIE     &P1IE
#define PBFlugPend     &P1IFG
#define PBArrPortDir   &P1DIR
#define PBArrPortsel   &P1SEL
#define PB0    0x01
#define PB1    0x02
#define PB2    0x04




#endif
