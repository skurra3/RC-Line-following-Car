/*
 * IOT.c
 *
 *  Created on: Apr 19, 2025
 *      Author: skurr
 */
#include  "macros.h"
#include "ports.h"
#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"

unsigned char IOT_Data[4][11];               // Storage for parsed IOT data lines
unsigned char iot_TX_buf[64];                // Buffer for IOT transmission
unsigned char ip_address[16];
char IOT_Ring_Rx[RING_BUFFER_SIZE];

extern volatile char slow_input_down;
extern char display_line[4][11];



extern char *display[4];
extern unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern volatile char one_time;


extern char RingNCSU1[11];
extern int iot_rx_rd;         // Read index for IOT buffer
extern int iot_rx_wr;         // Write index for IOT buffer
extern int line;                       // Current line being processed
extern  int next_line;                   // Next line to process
extern int character;                  // Current character position
extern int IOT_parse;                  // Flag for parsing state
extern int test_Value;                 // Test value for LED control
extern int boot_state;                 // Boot state for IOT module
extern int ip_address_found;           // Flag for IP address found
extern int iot_index;                  // Index for IP address processing
extern int iot_tx;                     // Index for IOT transmission
extern char AT[11];               // AT command string
extern char ip_mac[11];     // Command to get IP/MAC
unsigned int iot_rx_wr_temp;
char processed_data[11];
extern char IoT_to_PC_Buffer[11];
extern int uca0_tx_index;           // Index for UCA0 transmission
extern int uca1_tx_index;

 extern char rx_buf[75];

 extern volatile char event;

extern volatile unsigned int seconds;

unsigned int flag = 0;
char command_line[33];
unsigned int command_store = 0;
unsigned char check;
unsigned char current_command = '\0';  // Currently executing command
unsigned int command_active = 0;       // Flag to indicate an active command
unsigned int start_seconds = 0;        // Time when command started
unsigned int command_phase = 0;

void IOT_Process(void) {
    unsigned int iot_rx_wr_temp = iot_rx_wr;
    int j;

    // First check if there's an active command in progress
    if (command_active) {
        ProcessCommand(current_command);
        return;
    }

    // Check if there's data to process
    if(iot_rx_wr_temp != iot_rx_rd) {
        // Read the character from the buffer
        char current_char = rx_buf[iot_rx_rd++];

        // Wrap around if needed
        if(iot_rx_rd >= sizeof(rx_buf)) {
            iot_rx_rd = 0;
        }

        // Store the character
        IOT_Data[line][character] = current_char;

        // Process character
        if (current_char == '^') {
            flag = 1;  // Mark the start of a command
        }

        if(current_char == 0x0A || current_char == 0x0D) {  // Line feed or carriage return
            // End of line processing
            IOT_Data[line][character] = '\0';  // Null-terminate

            // If this was a command line (started with ^)
            if (flag == 1) {
                strcpy(command_line, IOT_Data[line]);
                command_store = 1;
                flag = 0;  // Reset flag

                // Process the command (first char after ^)
                for (j = 0; j < 32; j++) {
                    if (command_line[j] == '^' && j+1 < 32) {
                        check = command_line[j+1];
                        current_command = check;  // Store the current command
                        command_active = 1;       // Mark that we have an active command
                        start_seconds = seconds;  // Record the starting time
                        command_phase = 0;        // Initialize phase to 0

                        // Start the command execution
                        InitializeCommand(current_command);
                        break;
                    }
                }
            }

            // Move to next line
            character = 0;
            line++;
            if(line >= 4) {
                line = 0;
            }
            next_line = line + 1;
            if(next_line >= 4) {
                next_line = 0;
            }
        } else {
            // Continue building current line
            character++;
            if(character >= 31) {
                character = 30;
            }
        }
    }
}


void InitializeCommand(char cmd) {
    switch(cmd) {
        case 'F':
            strcpy(display_line[2], "  Forward ");
            Forward_Move();
            strcpy(display_line[1], "          ");
            display_changed = TRUE;
            break;

        case 'B':
            strcpy(display_line[2], "   Back  ");
            strcpy(display_line[1], "         ");
            display_changed = TRUE;
            Back_Move();
            break;

        case 'R':
            strcpy(display_line[2], "   Right   ");
            strcpy(display_line[1], "           ");
            display_changed = TRUE;
            Clockwise();
            break;

        case 'L':
            strcpy(display_line[2], "   Left   ");
            strcpy(display_line[1], "          ");
            display_changed = TRUE;
            Counter_Clockwise();
            break;

        case 'C':
            strcpy(display_line[2], "    F+B   ");
            strcpy(display_line[1], "          ");
            display_changed = TRUE;
            Forward_Move();
            break;
        case '1':
                    strcpy(display_line[2], "    PAD   ");
                    strcpy(display_line[1], "    1     ");
                    display_changed = TRUE;
                    break;

        case '2':
                            strcpy(display_line[2], "    PAD   ");
                            strcpy(display_line[1], "    2     ");
                            display_changed = TRUE;
                            break;
        case '3':
                                    strcpy(display_line[2], "    PAD   ");
                                    strcpy(display_line[1], "    3     ");
                                    display_changed = TRUE;
                                    break;
        case '4':
                                            strcpy(display_line[2], "    PAD   ");
                                            strcpy(display_line[1], "    4     ");
                                            display_changed = TRUE;
                                            break;
        case '5':
                                                    strcpy(display_line[2], "    PAD 5 ");
                                                    strcpy(display_line[1], " ARRIVED  ");
                                                    display_changed = TRUE;
                                                    break;
        case '6':
                                                    strcpy(display_line[2], "   PAD 6  ");
                                                    strcpy(display_line[1], " ARRIVED  ");
                                                    display_changed = TRUE;
                                                    break;
        case '7':
                                                           strcpy(display_line[2], "   PAD 7  ");
                                                           strcpy(display_line[1], " ARRIVED  ");
                                                           display_changed = TRUE;
                                                           break;
        case '8':
                                                                   strcpy(display_line[2], "   PAD 8  ");
                                                                   strcpy(display_line[1], " ARRIVED  ");
                                                                   display_changed = TRUE;
                                                                   break;

        case 'P':
                                                                           strcpy(display_line[2], "    PAD 8  ");
                                                                           strcpy(display_line[1], "Executing  ");
                                                                           display_changed = TRUE;
                                                                           event = '1';
                                                                           break;
        case 'G':
                    strcpy(display_line[2], "  Fast    ");
                    strcpy(display_line[1], "          ");
                    display_changed = TRUE;
                    Forward_Move_Fast();
                    break;
        case 'Y':
                            strcpy(display_line[2], "  Fast    ");
                            strcpy(display_line[1], "          ");
                            display_changed = TRUE;
                            Back_Move_Fast();
                            break;

        default:
            command_active = 0;
            current_command = '\0';
            break;
    }
}

// Revised function to handle the command execution with proper timing
void ProcessCommand(char cmd) {
    unsigned int elapsed_time = seconds - start_seconds;

    switch(cmd) {
        case 'F':   // Forward command
            if (elapsed_time >= 1) {
                Forward_Off();
                strcpy(display_line[0], "Executed  ");
                strcpy(display_line[1], "          ");
                display_changed = TRUE;
                command_active = 0;
                current_command = '\0';
            }
            break;

        case 'B':
            if (elapsed_time >= 1) {
                Forward_Off();
                strcpy(display_line[0], "Executed  ");
                strcpy(display_line[1], "          ");
                display_changed = TRUE;
                command_active = 0;
                current_command = '\0';
            }
            break;

        case 'R':
            if (elapsed_time >= 1) {
                Forward_Off();
                strcpy(display_line[0], "Executed   ");
                strcpy(display_line[1], "           ");
                display_changed = TRUE;
                command_active = 0;
                current_command = '\0';
            }
            break;

        case 'L':
            if (elapsed_time >= 1) {
                Forward_Off();
                strcpy(display_line[0], "Executed  ");
                strcpy(display_line[1], "          ");
                display_changed = TRUE;
                command_active = 0;
                current_command = '\0';
            }
            break;

        case 'C':   // Combination: Forward then turn clockwise
            if (elapsed_time == 3 && command_phase == 0) {
                // After 2 seconds of moving forward, start turning right
                Forward_Move();
                Back_Move();
                command_phase = 1;  // Move to phase 1
                start_seconds = seconds;  // Reset timer for the next phase
            } else if (command_phase == 1 && elapsed_time >= 3) {  // 2s of turning
                Forward_Off();  // Or use a dedicated stop function
                strcpy(display_line[0], "Executed   ");
                strcpy(display_line[1], "           ");
                display_changed = TRUE;
                command_active = 0;
                current_command = '\0';
                command_phase = 0;
            }
            break;
        case '1':
                    if (elapsed_time >= 3) {
                        strcpy(display_line[0], " Executed  ");
                        strcpy(display_line[1], "    PAD 1  ");
                        display_changed = TRUE;
                        command_active = 0;
                        current_command = '\0';
                    }
                    break;
        case '2':
                            if (elapsed_time >= 3) {
                                strcpy(display_line[0], " Executed  ");
                                strcpy(display_line[1], "    PAD 2  ");
                                display_changed = TRUE;
                                command_active = 0;
                                current_command = '\0';
                            }
                            break;
        case '3':
                                    if (elapsed_time >= 3) {
                                        strcpy(display_line[0], " Executed  ");
                                        strcpy(display_line[1], "    PAD 3  ");
                                        display_changed = TRUE;
                                        command_active = 0;
                                        current_command = '\0';
                                    }
                                    break;
        case '4':
                                            if (elapsed_time >= 3) {
                                                strcpy(display_line[0], " Executed  ");
                                                strcpy(display_line[1], "    PAD 4  ");
                                                display_changed = TRUE;
                                                command_active = 0;
                                                current_command = '\0';
                                            }
                                            break;
        case '5':
                                            if (elapsed_time >= 3) {
                                                strcpy(display_line[0], " Executed  ");
                                                strcpy(display_line[1], "    PAD 5  ");
                                                display_changed = TRUE;
                                                command_active = 0;
                                                current_command = '\0';
                                            }
                                            break;
        case '6':
                                            if (elapsed_time >= 3) {
                                                strcpy(display_line[0], " Executed  ");
                                                strcpy(display_line[1], "    PAD 6  ");
                                                display_changed = TRUE;
                                                command_active = 0;
                                                current_command = '\0';
                                            }
                                            break;
        case '7':
                                            if (elapsed_time >= 3) {
                                                strcpy(display_line[0], " Executed  ");
                                                strcpy(display_line[1], "    PAD 7  ");
                                                display_changed = TRUE;
                                                command_active = 0;
                                                current_command = '\0';
                                            }
                                            break;
        case '8':
                                                    if (elapsed_time >= 3) {
                                                        strcpy(display_line[0], " Executed  ");
                                                        strcpy(display_line[1], "    PAD 8  ");
                                                        display_changed = TRUE;
                                                        command_active = 0;
                                                        current_command = '\0';
                                                    }
                                                    break;
        case 'G':
                    if (elapsed_time >= 2) {
                        Forward_Off();
                        strcpy(display_line[0], "Executed  ");
                        strcpy(display_line[1], "          ");
                        display_changed = TRUE;
                        command_active = 0;
                        current_command = '\0';
                    }
                    break;

        case 'Y':
                            if (elapsed_time >= 2) {
                                Forward_Off();
                                strcpy(display_line[0], "Executed  ");
                                strcpy(display_line[1], "          ");
                                display_changed = TRUE;
                                command_active = 0;
                                current_command = '\0';
                            }
                            break;
        default:
            command_active = 0;
            current_command = '\0';
            break;
    }
}

