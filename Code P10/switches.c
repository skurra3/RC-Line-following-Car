/*
 * switches.c
 *
 *  Created on: Feb 12, 2025
 *      Author: skurr
 */

#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"
//VARIABLES
extern char event;
extern char display_line[4][11];
int sw1_position;
extern volatile char USB_Ring_Tx[SMALL_RING_SIZE];
int sw2_position;
int okay_to_look_at_switch1;
int okay_to_look_at_switch2;
extern volatile unsigned char display_changed;
int count_debounce_SW1=0;
int count_debounce_SW2=0;
int count1;
unsigned int BLACK_LINE_L;
unsigned int BLACK_LINE_R;
unsigned int WHITE_SPACE_L;
unsigned int WHITE_SPACE_R;
extern volatile unsigned int countz1;
extern volatile unsigned char update_display;
unsigned int debounce_count;
extern volatile unsigned int ADC_Left_Detect;
extern volatile unsigned int ADC_Right_Detect;

#pragma vector=PORT4_VECTOR
__interrupt void switchP4_interrupt(void){
// Switch 1
if (P4IFG & SW1) {
P4IE &= ~SW1;
P4IFG &= ~SW1; // IFG SW1 cleared


update_display = 1;
display_changed = TRUE;

UCA1BRW = 17;
UCA1MCTLW = 0x4A00;
UCA1IE |= UCTXIE;


update_display = 1;
P2IE &= ~SW2;
P2IFG &= ~SW2;
TB0CCTL0 &= ~CCIFG;
TB0CCTL0 &= ~CCIE;
TB0CCR1 += TB0CCR1_INTERVAL;
TB0CCTL1 |= CCIE;
P6OUT &= ~LCD_BACKLIGHT;


}
}


#pragma vector=PORT2_VECTOR
__interrupt void switchP2_interrupt(void){
// Switch 2
if (P2IFG & SW2) {
    P4IE &= ~SW2;
    P4IFG &= ~SW2; // IFG SW1 cleared



    Clear_RX_Buffer();
    Start_AT_Command_Sequence();

    display_changed = TRUE;

    UCA1BRW = 4;
    UCA1MCTLW = 0x5551;
    UCA1IE |= UCTXIE;

    update_display = 1;

    P2IE &= ~SW2;
    P2IFG &= ~SW2;
    TB0CCTL0 &= ~CCIFG;
    TB0CCTL0 &= ~CCIE;
    TB0CCR1 += TB0CCR1_INTERVAL;
    TB0CCTL1 |= CCIE;
    P6OUT |= LCD_BACKLIGHT;
}
}


