
#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"



// Function Prototypes
void main(void);
void Init_Conditions(void);
void Display_Process(void);
void Init_LEDs(void);
void Carlson_StateMachine(void);



  // Global Variables
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
extern unsigned char iot_TX_buf[64];


volatile unsigned int seconds;
unsigned int starting = 1;


volatile char event = '0';
extern volatile int sw2_position;

unsigned int Last_Time_Sequence; // a variable to identify Time_Sequence has changed
unsigned int cycle_time; // is a new time base used to control making shapes
unsigned int time_change; // is an identifier that a change has occurred
unsigned int countz = 0;
extern volatile unsigned int countz1;
extern volatile unsigned int countz2;

extern volatile unsigned int ADC_Left_Detect;
extern volatile unsigned int ADC_Right_Detect;

extern volatile char RingNCSU[11];

extern int rx_done;

extern unsigned int ip_address_found;

unsigned int wheel_move;
char forward;
extern volatile unsigned int clock;



//void main(void){
void main(void){
    PM5CTL0 &= ~LOCKLPM5;
    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings

    Init_Ports();                        // Initialize Ports
    Init_Clocks();                       // Initialize Clock System
    Init_Conditions();                   // Initialize Variables and Initial Conditions
    Init_LCD();                          // Initialize LCD
    Init_Timer_B0();
    Init_ADC();
    Init_Timer_B3();
    Init_Serial_UCA0();
    Init_Serial_UCA1();



    P2OUT &= ~RESET_LCD;
    //Place the contents of what you want on the display, in between the quotes
    //Limited to 10 characters per line
    strcpy(display_line[0], "          ");
    strcpy(display_line[1], "          ");
    strcpy(display_line[2], "          ");
    strcpy(display_line[3], "          ");

    HEXtoBCD(clock);
        adc_line4(0,3);

    Init_Trans_Conditions();

    display_changed = TRUE;
    Display_Update(0,0,0,0);

    wheel_move = 0;
    forward = TRUE;


    while(ALWAYS) {                      // Can the Operating system run
        Display_Process();                 // Update Display
        P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF
        one_time = 1;

        Process_AT_Commands();
        IOT_Process();

        if(countz == 4){
                countz = 0;
                HEXtoBCD(clock);
                adc_line4(3,3);
            }


        if (starting){
            seconds = 0;
            P3OUT  &= ~IOT_EN;
            starting = 0;
            P3OUT  &= ~IOT_LINK_GRN;
        }

        if (seconds >= 1){
            P3OUT  |= IOT_EN;
            P3OUT  |= IOT_LINK_GRN;
        }
        switch(event){
                case '1':
                    wait();
                    break;
                case '2':
                    Semi1();
                    break;
                case '3':
                    stop();
                    break;
                case '4':
                    find();
                    break;
                case '5':
                    finish();
                    break;
                case '6':
                       turn();
                       break;
                case '7':
                        center();
                        break;
                case '8':
                    end();

                default:
                    break;
                }

    }
}

  // -----------------------------------------------------------------------






