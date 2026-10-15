//Serial. C
#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"

char RingNCSU[11] = "NCSU #1";
int i;
int j;

extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern char display_line[4][11];

unsigned int iot_rx_rd = 0;                             // read index for IOT buffer
unsigned int iot_rx_wr = 0;                             // write index for IOT buffer
unsigned int character = 0;
unsigned int IOT_parse = 0;                             // Flag for parsing state

unsigned int testValue = 0;                            // Test LED control
unsigned int boot_state = 0;
unsigned int line = 0;
unsigned int next_line = 0;

unsigned int ip_address_found = 0;
unsigned int iot_index = 0;                             // IP address processing
unsigned int iot_tx = 0;                                // IOT transmission
char AT[] = "AT\r\n";                                   // AT command string

char ip_mac[] = "AT+CIFSR\r\n";                         // Command to get IP/MAC
unsigned int rx_cnt = 0;
char rx_buf[75];
char buzzer[75];
char termite_buffer[RING_BUFFER_SIZE];

unsigned int termite_index = 0;
unsigned int uca0_tx_index = 0;
unsigned int uca1_tx_index = 0;
unsigned int rx_index = 0;
char IoT_to_PC_Buffer[11];                              // (UCA0 -> UCA1)



void Init_Serial_UCA0(void){
//------------------------------------------------------------------------------
// TX error (%) RX error (%)
// BRCLK     Baudrate UCOS16  UCBRx  UCFx   UCSx    neg    pos    neg    pos
// 8000000      4800    1     104       2   0xD6   -0.08  0.04  -0.10   0.14
// 8000000      9600    1      52       1   0x49  -0.08   0.04  -0.10   0.14
// 8000000      19200   1      26       0   0xB6  -0.08   0.16  -0.28   0.20
// 8000000      57600   1       8       10  0xF7  -0.32   0.32  -1.00   0.36
// 8000000      115200  1       4       5   0x55  -0.80   0.64  -1.12   1.76
// 8000000      460800  0      17       0   0x4A  -2.72   2.56  -3.76   7.28
//------------------------------------------------------------------------------
// Configure eUSCI_A0 for UART mode
    UCA0CTLW0 = 0;
    UCA0CTLW0 |=  UCSWRST ;              // Put eUSCI in reset
    UCA0CTLW0 |=  UCSSEL__SMCLK;         // Set SMCLK as fBRCLK
    UCA0CTLW0 &= ~UCMSB;                 // MSB, LSB select
    UCA0CTLW0 &= ~UCSPB;                 // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
    UCA0CTLW0 &= ~UCPEN;                 // No Parity
    UCA0CTLW0 &= ~UCSYNC;
    UCA0CTLW0 &= ~UC7BIT;
    UCA0CTLW0 |=  UCMODE_0;
    // BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg pos neg pos
    // 8000000 115200 1 4 5 0x55 -0.80 0.64 -1.12 1.76
    // UCA?MCTLW = UCSx + UCFx + UCOS16
     UCA0BRW = 4 ; // 115,200 baud
     UCA0MCTLW = 0x5551 ;
     UCA0CTLW0 &= ~UCSWRST ; // release from reset
//     UCA0TXBUF = 0x00; // Prime the Pump
     UCA0IE |= UCRXIE; // Enable RX interrupt
//------------------------------------------------------------------------------
}

void Init_Serial_UCA1(void){
//------------------------------------------------------------------------------
// TX error (%) RX error (%)
// BRCLK     Baudrate UCOS16  UCBRx  UCFx   UCSx    neg    pos    neg    pos
// 8000000      4800    1     104       2   0xD6   -0.08  0.04  -0.10   0.14
// 8000000      9600    1      52       1   0x49  -0.08   0.04  -0.10   0.14
// 8000000      19200   1      26       0   0xB6  -0.08   0.16  -0.28   0.20
// 8000000      57600   1       8       10  0xF7  -0.32   0.32  -1.00   0.36
// 8000000      115200  1       4       5   0x55  -0.80   0.64  -1.12   1.76
// 8000000      460800  0      17       0   0x4A  -2.72   2.56  -3.76   7.28
//------------------------------------------------------------------------------
// Configure eUSCI_A1 for UART mode
    UCA1CTLW0 = 0;
    UCA1CTLW0 |=  UCSWRST ;              // Put eUSCI in reset
    UCA1CTLW0 |=  UCSSEL__SMCLK;         // Set SMCLK as fBRCLK
    UCA1CTLW0 &= ~UCMSB;                 // MSB, LSB select
    UCA1CTLW0 &= ~UCSPB;                 // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
    UCA1CTLW0 &= ~UCPEN;                 // No Parity
    UCA1CTLW0 &= ~UCSYNC;
    UCA1CTLW0 &= ~UC7BIT;
    UCA1CTLW0 |=  UCMODE_0;
    // BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg pos neg pos
    // 8000000 115200 1 4 5 0x55 -0.80 0.64 -1.12 1.76
    // UCA?MCTLW = UCSx + UCFx + UCOS16
     UCA1BRW = 4 ; // 115,200 baud
     UCA1MCTLW = 0x5551 ;

     UCA1CTLW0 &= ~UCSWRST ; // release from reset
//     UCA1TXBUF = 0x00; // Prime the Pump
     UCA1IE |= UCRXIE; // Enable RX interrupt
//------------------------------------------------------------------------------
}

  #pragma vector=EUSCI_A0_VECTOR
     __interrupt void eUSCI_A0_ISR(void){
         char temp;
         switch(__even_in_range(UCA0IV,0x08)){
         case 0:                                 // No interrupt
             break;

         case 2:                                 // RXIFG
             temp = UCA0RXBUF;
             UCA1TXBUF = temp;                   // Send char to UCA1

             USB_Char_Rx[pc_rx_ring_wr] = temp;
             pc_rx_ring_wr++;

             if (pc_rx_ring_wr >= RING_BUFFER_SIZE) {
                 pc_rx_ring_wr = 0;
             }

             // ***** make the IoT parser see characters typed on the PC *****
             rx_buf[iot_rx_wr] = temp;                      // feed IOT_Process()
             iot_rx_wr++;
             if (iot_rx_wr >= sizeof(rx_buf)) {
                 iot_rx_wr = 0;                            // Wrap around rx buffer
                 }


             if (temp != 0x0A && temp != 0x0D) {             // Check if the received char is not a newline (0x0A) or carriage return (0x0D)
                 if (rx_cnt < 10) {
                     display_line[1][rx_cnt] = temp;         // Store char into second line
                     rx_cnt++;                               // Increment the character counter
                     display_changed = TRUE;
                 }
             }
             else
             {
             // If newline or carriage return is received, treat it as end of message
             rx_cnt = 0;                            // Reset counter
             strcpy(display_line[0], "   SENT   ");
             display_changed = TRUE;
             }
             break;

             case 4:  // TXIFG (Transmit buffer is ready to send next byte)
                 UCA0TXBUF = IoT_to_PC_Buffer[uca0_tx_index];   // Load next byte from the transmit buffer into UART TX register
                 uca0_tx_index++;                               // Move to the next byte in the buffer

                 // Check if reached the end of the buf OR hit a null terminator
                 if (uca0_tx_index >= sizeof(IoT_to_PC_Buffer) || IoT_to_PC_Buffer[uca0_tx_index] == 0) {
                     UCA0IE &= ~UCTXIE;                     // Disable UART TX interrupt (done transmitting)
                     uca0_tx_index = 0;                     // Reset index for the next transmission
                 }
                 break;
             default:
                 break;
         }
     }


     #pragma vector=EUSCI_A1_VECTOR
     __interrupt void eUSCI_A1_ISR(void){
         char temp;
         switch(__even_in_range(UCA1IV,0x08)){
         case 0:                                // Vector 0 - no interrupt
             break;

         case 2:                                // RXIFG
             temp = UCA1RXBUF;
             UCA0TXBUF = temp;                  // Send to UCA0
             USB_Char_Rx[pc_rx_ring_wr] = temp;
             pc_rx_ring_wr++;

             if (pc_rx_ring_wr >= RING_BUFFER_SIZE) {
                 pc_rx_ring_wr = 0;  // Wrap around ring buffer
                 }
             break;

         case 4:                                 // Vector 4 - TXIFG (Transmit buffer empty for UCA1, ready for next byte)
             UCA1TXBUF = IoT_to_PC_Buffer[uca1_tx_index]; // Send the next byte from the buffer
             uca1_tx_index++;                               // Advance the buffer index

             if (uca1_tx_index >= sizeof(IoT_to_PC_Buffer) || IoT_to_PC_Buffer[uca1_tx_index] == 0) {
                 UCA1IE &= ~UCTXIE;  // Disable TX interrupt when done
                 uca1_tx_index = 0;
             }
             break;
             default: break;
         }
     }

     void Init_Trans_Conditions(void) {
         int k = 0;

         strcpy(display_line[0], " Waiting  ");         // Initial Display
         strcpy(display_line[1], "          ");
         display_changed = TRUE;

         if(UCA0BRW == 4) {
             strcpy(display_line[2], "   Baud   ");   // Baudrate label
             strcpy(display_line[3], " 115200 Hz");
         } else if(UCA0BRW == 17) {
             strcpy(display_line[2], "   Baud   ");
             strcpy(display_line[3], " 460800 Hz");
         }

         display_changed = TRUE;

         com_state = WAIT_STATE;                        // Initialize communication variables, wait, no command received, and not ready to transmit
         command_received = FALSE;
         transmit_ready = FALSE;

         uca0_tx_index = 0;
         uca1_tx_index = 0;


         for(k = 0; k < 11; k++) {                       // Null terminate
             received[k] = ' ';
             RingNCSU[k] = ' ';
             IoT_to_PC_Buffer[k] = ' ';
         }
         received[10] = '\0';
         RingNCSU[10] = '\0';
         IoT_to_PC_Buffer[10] = '\0';

     }

