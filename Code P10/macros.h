/*
 * macros.h
 *
 *  Created on: Jan 29, 2025
 *      Author: skurr
 */

#ifndef MACROS_H_
#define MACROS_H_
#define ALWAYS                  (1)
#define RESET_STATE             (0)
#define RED_LED              (0x01) // RED LED 0
#define GRN_LED              (0x40) // GREEN LED 1
#define TEST_PROBE           (0x01) // 0 TEST PROBE
#define TRUE                 (0x01) //


//PROJECT 4: SHAPES////
#define NONE ('N')
#define STRAIGHT ('L')
#define CIRCLE ('C')
#define TRIANGLE ('T')
#define EIGHT ('H')
#define WAIT ('W')
#define START ('S')
#define RUN ('R')
#define END ('E')

//STRAIGHTLINE/CIRCLE//
#define WHEEL_COUNT_TIME (30)
#define RIGHT_COUNT_TIME (30)
#define LEFT_COUNT_TIME (7)
#define TRAVEL_DISTANCE (64)
#define WAITING2START (50) // default: 50


//TRIANGLE//STRAIGHT
#define RIGHT_COUNT_TIME_TRI (20)
#define LEFT_COUNT_TIME_TRI (20)
#define WHEEL_COUNT_TIME_TRI (20)
#define TRAVEL_DISTANCE_TRI (7)

//TRIANGLE//CORNER
#define RIGHT_COUNT_TIME_CNR (40)
#define LEFT_COUNT_TIME_CNR (30)
#define WHEEL_COUNT_TIME_CNR (30)
#define TRAVEL_DISTANCE_TRI_CNR (2)



//SWITCHES//
#define PRESSED (0)
#define RELEASED (1)

#define OKAY (1)
#define NOT_OKAY (0)

#define DEBOUNCE_TIME (100)
#define DEBOUNCE_RESTART (5)

// Main While loop switch cases// HOMEWORK 6 TIMERS
#define Time_Sequence_Rate (50) // 50 millisecods
#define S1250 (1250/Time_Sequence_Rate) // 1.25 seconds
#define S1000 (1000/Time_Sequence_Rate) // 1 second
#define S750 (750/Time_Sequence_Rate) // 0.75 seconds
#define S500 (500/Time_Sequence_Rate) // 0.50 seconds
#define S250 (250/Time_Sequence_Rate) // 0.25 seconds

#define ID_1 (0x0040) /* /2 */
#define ID_2 (0x0080) /* /4 */
#define ID_3 (0x00c0) /* /8 */
#define ID__2 (0x0040) /* /2 */
#define ID__4 (0x0080) /* /4 */
#define ID__8 (0x00c0) /* /8 */

#define TBIDEX_0 (0x0000) /* Divide by 1 */
#define TBIDEX_1 (0x0001) /* Divide by 2 */
#define TBIDEX_2 (0x0002) /* Divide by 3 */
#define TBIDEX_3 (0x0003) /* Divide by 4 */
#define TBIDEX_4 (0x0004) /* Divide by 5 */
#define TBIDEX_5 (0x0005) /* Divide by 6 */
#define TBIDEX_6 (0x0006) /* Divide by 7 */
#define TBIDEX_7 (0x0007) /* Divide by 8 */

#define TBIDEX__1 (0x0000) /* Divide by 1 */
#define TBIDEX__2 (0x0001) /* Divide by 2 */
#define TBIDEX__3 (0x0002) /* Divide by 3 */
#define TBIDEX__4 (0x0003) /* Divide by 4 */
#define TBIDEX__5 (0x0004) /* Divide by 5 */
#define TBID.  EX__6 (0x0005) /* Divide by 6 */
#define TBIDEX__7 (0x0006) /* Divide by 7 */
#define TBIDEX__8 (0x0007) /* Divide by 8 */


#define TB0CCR0_INTERVAL (250000) // 8,000,000 / 2 / 8 / (1 / 50msec)
#define TB0CCR1_INTERVAL (25000) // 8,000,000 / 2 / 8 / (1 / 5msec)
#define TB0CCR2_INTERVAL (25000) // 8,000,000 / 2 / 8 / (1 / 5msec)

#define FIFTY_MS_COUNT 10

#define P4PUD (P4OUT)

//PWM DEFINES
#define PWM_PERIOD (TB3CCR0)
#define LCD_BACKLITE_DIMING (TB3CCR1)
#define RIGHT_FORWARD_SPEED (TB3CCR3)   //TB3CCR3
#define LEFT_FORWARD_SPEED (TB3CCR5)  //TB3CCR5
#define RIGHT_REVERSE_SPEED (TB3CCR2) //TB3CCR2
#define LEFT_REVERSE_SPEED (TB3CCR4) //TB3CCR4

#define WHEEL_OFF (0)
#define SLOW (9500)
#define FAST (50000)
#define PERCENT_100 (50000)
#define PERCENT_80 (45000)


#define BEGINNING         (0)
#define SMALL_RING_SIZE   (16)
#define RING_BUFFER_SIZE    (16)
#define PROCESS_BUFFER_SIZE (32)


volatile unsigned char com_state;
char received[11];
volatile unsigned char command_received;
volatile unsigned char transmit_ready;

volatile unsigned int usb_rx_ring_wr;
volatile unsigned int usb_rx_ring_rd;
volatile unsigned int pc_rx_ring_wr;
volatile unsigned int pc_rx_ring_rd;

volatile char USB_Char_Rx[SMALL_RING_SIZE];
volatile char PC_Char_Rx[SMALL_RING_SIZE];
unsigned int current_baud_rate;
unsigned char NCSU2[PROCESS_BUFFER_SIZE];

#define WAIT_STATE  (0)
#define RECEIVE     (1)
#define TRANSMIT    (2)

#endif /* MACROS_H_ */
