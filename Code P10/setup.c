/*
 * setup.c
 *
 *  Created on: Apr 21, 2025
 *      Author: skurr
 */

#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"

extern volatile unsigned int seconds;


volatile char slow_input_down;
extern char display_line[4][11];
extern char *display[4];
unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
unsigned int Time_Sequence;
extern volatile char one_time;
unsigned int test_value;
char chosen_direction;
char change;
extern int iot_tx;
extern char buzzer[75];
extern unsigned char iot_TX_buf[64];


extern char IoT_to_PC_Buffer[11];
extern int uca0_tx_index;
 char rx_buffer[11];
extern int iot_rx_rd;
extern int iot_rx_wr;


extern char rx_buf[75];

#define AT_CMD_COUNT 4 //changed
#define CMD_IDLE 0
#define CMD_SENDING 1
#define CMD_WAITING 2

// AT command sequence
const char* at_commands[AT_CMD_COUNT] = {
    "AT+SYSSTORE=0\r\n",
    "AT+CIPMUX=1\r\n",
    "AT+CIPSERVER=1,9031\r\n",
    "AT+CIFSR\r\n"
};
unsigned int cmd_index = 0;      // Current command index
unsigned int cmd_state = CMD_IDLE;      // Command state
unsigned int cmd_timer = 0;      // Timer for command sequence
unsigned int cmd_delay = 2;      // Delay between commands (in seconds)
unsigned int cmd_timeout = 5;    // Timeout for waiting for response (in seconds)

// Function to directly send a command to UCA0 (IoT module)
void Send_Command_To_IoT(const char* command) {
    int i = 0;

    // First disable TX interrupt if it's active
    UCA0IE &= ~UCTXIE;

    // Wait for any previous transmission to complete
    while (UCA0STATW & UCBUSY);

    // Send each character in the command
    while (command[i] != '\0') {
        UCA0TXBUF = command[i];
        // Wait for character to be transmitted
        while (!(UCA0IFG & UCTXIFG));
        i++;
    }

    // Display the command being sent
    strcpy(display_line[2], " Sending:");

    // Show abbreviated command on display (remove \r\n and limit length)
    char cmd_display[11] = "          ";
    strncpy(cmd_display, command, 10);
    // Remove \r\n from display
    for (i = 0; i < 10; i++) {
        if (cmd_display[i] == '\r' || cmd_display[i] == '\n')
            cmd_display[i] = ' ';
    }
    strcpy(display_line[3], cmd_display);
    display_changed = TRUE;
}

// Function to start the AT command sequence
void Start_AT_Command_Sequence(void) {
    if (cmd_state == CMD_IDLE) {
        // Initialize sequence
        cmd_index = 0;
        cmd_state = CMD_SENDING;

        // Display status
        strcpy(display_line[0], " WiFi Init");
        strcpy(display_line[1], " Starting ");
        display_changed = TRUE;

        // Clear any pending interrupts
        UCA0IFG &= ~(UCRXIFG | UCTXIFG);

        // Send first command
        Send_Command_To_IoT(at_commands[cmd_index]);

        // Start timer for timeout
        cmd_timer = seconds;
        cmd_state = CMD_WAITING;
    }
}

// Process AT command sequence - call this from your main loop
void Process_AT_Commands(void) {
    if (cmd_state == CMD_WAITING) {
        // Check for "OK" response in the rx_buffer
        int ok_found = 0;
        int i;
        for (i = 0; i < sizeof(rx_buffer) - 1; i++) {
            if (rx_buffer[i] == 'O' && rx_buffer[i+1] == 'K') {
                ok_found = 1;
                break;
            }
        }

        // If "OK" found or timeout occurred
        if (ok_found || (seconds - cmd_timer) >= cmd_timeout) {
            if (ok_found) {
                // Success case
                strcpy(display_line[1], " CMD OK   ");
            } else {
                // Timeout case
                strcpy(display_line[1], " CMD TO   ");
            }
            display_changed = TRUE;

            // Wait a bit before sending next command
            cmd_timer = seconds;
            cmd_state = CMD_SENDING;
        }
    } else if (cmd_state == CMD_SENDING) {
        // Check if it's time to send the next command
        if ((seconds - cmd_timer) >= cmd_delay) {
            // Move to next command
            cmd_index++;

            // If we've sent all commands
            if (cmd_index >= AT_CMD_COUNT) {
                // Sequence complete
                strcpy(display_line[0], "Complete! ");
                strcpy(display_line[1], "          ");
                strcpy(display_line[2], "          ");
                strcpy(display_line[3], "          ");
                display_changed = TRUE;

                cmd_state = CMD_IDLE;

                /* test code */

                int i = 0;
                int buf_idx = 0;
                int ip_idx = 0;
                char ip_str_part1[10] = {0};
                char ip_str_part2[10] = {0};

                for(i = 0 ; i < sizeof(rx_buf) ; i++) {
                       if(rx_buf[buf_idx] == 'I' && rx_buf[buf_idx + 1] == 'P') {
                           strcpy(display_line[1], "IP RECV");
                           ip_idx = buf_idx;
                       }
                       buf_idx++;
                }
                ip_idx += 4; // go to the first number in IP address

                // store first numbers in first buffer
                for(i = 0 ; i < 9; i++) {
                    ip_str_part1[i] = rx_buf[ip_idx++];
                }

                // store second half of IP into second buffer
                for(i = 0 ; i < 9; i++) {
                    ip_str_part2[i] = rx_buf[ip_idx++];
                }

                // null terminate both strings for strcpy to work
                ip_str_part1[9] = '\0';
                ip_str_part2[9] = '\0';

                // string copy the two buffers into the display LCD
                strcpy(display_line[2], ip_str_part1);
                strcpy(display_line[3], ip_str_part2);


                /* end of test code */

                return;
            }

            // Send next command
            Send_Command_To_IoT(at_commands[cmd_index]);

            // Start timer for timeout
            cmd_timer = seconds;
            cmd_state = CMD_WAITING;
        }
    }
}

// Clear the rx_buffer function
void Clear_RX_Buffer(void) {
    int i;
    for (i = 0; i < sizeof(rx_buffer); i++) {
        rx_buffer[i] = 0;
    }
    iot_rx_rd = 0;
    iot_rx_wr = 0;
}
