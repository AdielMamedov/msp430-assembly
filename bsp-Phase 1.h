#ifndef _bsp_H
#define _bsp_H

#include <msp430g2553.h>

#define DataSegStart 0x0200
#define CodeSegStart 0xC000
#define StackTosStart 0x0400

#define   debounceVal      250
#define   delay70          119
#define   delay30          51
#define   onehz           5120
#define   towhz           10240

//Leds abstraction
#define LedsArrPort &P1OUT
#define LedsArrDir  &P1DIR
#define LedsArrSel  &P1SEL

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
#define PB3    0x08



#endif
