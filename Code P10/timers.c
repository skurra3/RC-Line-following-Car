/*
 * timers.c
 *
 *  Created on: Feb 17, 2025
 *      Author: skurr
 */

#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"



unsigned int blink_count;
extern volatile unsigned int Time_Sequence;
unsigned int one_time;
extern volatile unsigned int countz;
volatile unsigned int countz1;
volatile unsigned int countz2;
volatile unsigned int countz3;
volatile unsigned int countz4;
volatile unsigned int countz5;
extern volatile unsigned int debounce_count;

extern volatile unsigned int seconds;
volatile unsigned int millisecond_count;

extern volatile unsigned char update_display;

extern volatile unsigned int ADC_Left_Detect;
extern volatile unsigned int ADC_Right_Detect;
volatile unsigned int clock;




void Init_Timer_B0(void) {
TB0CTL = TBSSEL__SMCLK; // SMCLK source
TB0CTL |= TBCLR; // Resets TB0R, clock divider, count direction
TB0CTL |= MC__CONTINOUS; // Continuous up
TB0CTL |= ID_1; // Divide clock by 2
TB0EX0 = TBIDEX__8; // Divide clock by an additional 8
TB0CCR0 = TB0CCR0_INTERVAL; // CCR0
TB0CCTL0 |= CCIE; // CCR0 enable interrupt
 TB0CCR1 = TB0CCR1_INTERVAL; // CCR1
TB0CCTL1 |= CCIE; // CCR1 enable interrupt
TB0CCR2 = TB0CCR2_INTERVAL; // CCR2
// TB0CCTL2 |= CCIE; // CCR2 enable interrupt
TB0CTL &= ~TBIE; // Disable Overflow Interrupt
TB0CTL &= ~TBIFG; // Clear Overflow Interrupt flag
}

#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
//------------------------------------------------------------------------------
// TimerB0 0 Interrupt handler
//----------------------------------------------------------------------------
//...... Add What you need happen in the interrupt ......
countz++;  // USING TO DISPLAY ADC READINGS in Main
countz1++; //USING IN MOTORS
countz3++;//using for motors
countz4++;// Using for  display clock for P7
countz5++;
millisecond_count++;
if (millisecond_count == 20){//1 sec
             seconds++;
             clock++;
             millisecond_count = 0;
        }
if (countz5 == 4){  //200ms
             update_display = 1;
             countz5 = 0;
        }
ADCCTL0 |= ADCSC;

TB0CCR0 += TB0CCR1_INTERVAL; // Add Offset to TBCCR0
//----------------------------------------------------------------------------
}




#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void){
//----------------------------------------------------------------------------
// TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
//----------------------------------------------------------------------------
switch(__even_in_range(TB0IV,14)){
case 0: break; // No interrupt
case 2: // CCR1 Used for SW1 Debounce
if(debounce_count++ < 4){
    TB0CCTL0 &= ~CCIE;
}else {
    TB0CCTL1 &= ~CCIE;
    P4IFG &= ~SW1;
    P4IE |= SW1;
    P2IFG &= ~SW2;
    P2IE |= SW2;
    TB0CCTL0 |= CCIE;
    debounce_count =0;
}
break;
case 4: // CCR2 Used for SW2 Debounce
    if(debounce_count++ < 4){
        TB0CCTL0 &= ~CCIE;
    }else {
        TB0CCTL1 &= ~CCIE;
        P4IFG &= ~SW1;
        P4IE |= SW1;
        P2IFG &= ~SW2;
        P2IE |= SW2;
        TB0CCTL0 |= CCIE;
        debounce_count =0;
    }
break;
case 14: // overflow available for greater than 1 second timer
break;
default: break;
}
}






