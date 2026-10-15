/*
 * motors.c
 *
 *  Created on: Feb 4, 2025
 *      Author: skurr
 */

#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"

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

extern volatile char event;
extern volatile unsigned int countz1;
extern volatile unsigned int countz3;
extern volatile unsigned int ADC_Left_Detect;
extern volatile unsigned int ADC_Right_Detect;



void Forward_Off(void){
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM duty cycle
        LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM duty cycle
        RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM duty cycle
        LEFT_REVERSE_SPEED = WHEEL_OFF;
}

void Forward_Move_Right(void){
    RIGHT_FORWARD_SPEED = PERCENT_100; // P6.1 Right Forward PWM duty cycle
    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM duty cycle
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM duty cycle
    LEFT_REVERSE_SPEED = WHEEL_OFF;
}

void Forward_Move_Left(void){
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM duty cycle
    LEFT_FORWARD_SPEED = PERCENT_100; // P6.2 Left Forward PWM duty cycle
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM duty cycle
    LEFT_REVERSE_SPEED = WHEEL_OFF;

}

void Back_Move(void){
    RIGHT_FORWARD_SPEED = 40000; // P6.1 Right Forward PWM duty cycle
    LEFT_FORWARD_SPEED = 40000; // P6.2 Left Forward PWM duty cycle
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM duty cycle
    LEFT_REVERSE_SPEED = WHEEL_OFF;
}


void Forward_Move(void){
    RIGHT_FORWARD_SPEED = 0; // P6.1 Right Forward PWM duty cycle
    LEFT_FORWARD_SPEED = 0;  // P6.2 Left Forward PWM duty cycle
    RIGHT_REVERSE_SPEED = 40000; // P6.3 Right Reverse PWM duty cycle
    LEFT_REVERSE_SPEED = 40000;
}

void Forward_Move_Fast(void){
    RIGHT_FORWARD_SPEED = 0; // P6.1 Right Forward PWM duty cycle
    LEFT_FORWARD_SPEED = 0;  // P6.2 Left Forward PWM duty cycle
    RIGHT_REVERSE_SPEED = PERCENT_100; // P6.3 Right Reverse PWM duty cycle
    LEFT_REVERSE_SPEED = PERCENT_100;
}

void Back_Move_Fast(void){
    RIGHT_FORWARD_SPEED = PERCENT_100; // P6.1 Right Forward PWM duty cycle
    LEFT_FORWARD_SPEED = PERCENT_100;  // P6.2 Left Forward PWM duty cycle
    RIGHT_REVERSE_SPEED = 0; // P6.3 Right Reverse PWM duty cycle
    LEFT_REVERSE_SPEED = 0;
}

void Forward_Slow(void){
    RIGHT_FORWARD_SPEED = 0; // P6.1 Right Forward PWM duty cycle
    LEFT_FORWARD_SPEED = 0;  // P6.2 Left Forward PWM duty cycle
    RIGHT_REVERSE_SPEED = SLOW; // P6.3 Right Reverse PWM duty cycle
    LEFT_REVERSE_SPEED = SLOW;
}


void Clockwise(void){
    RIGHT_FORWARD_SPEED = 0; // P6.1 Right Forward PWM duty cycle
    LEFT_FORWARD_SPEED = 7500;  // P6.2 Left Forward PWM duty cycle
    RIGHT_REVERSE_SPEED = 7500; // P6.3 Right Reverse PWM duty cycle
    LEFT_REVERSE_SPEED = WHEEL_OFF;

}

void Counter_Clockwise(void){
    RIGHT_FORWARD_SPEED = 7500; // P6.1 Right Forward PWM duty cycle
    LEFT_FORWARD_SPEED = WHEEL_OFF;  // P6.2 Left Forward PWM duty cycle
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM duty cycle
    LEFT_REVERSE_SPEED = 7500;
}


///////////////////////////////// P7 motor functions
void LeftAd(void){
    LEFT_FORWARD_SPEED = 0;
            RIGHT_REVERSE_SPEED = 0;
        LEFT_REVERSE_SPEED = 0;
        RIGHT_FORWARD_SPEED = SLOW;
}

void RightAd(void){
        LEFT_REVERSE_SPEED = 0;
        RIGHT_FORWARD_SPEED = 0;
        LEFT_FORWARD_SPEED = SLOW;
        RIGHT_REVERSE_SPEED = 0;
}


void lineDetect(void){
    if((ADC_Right_Detect >= 300)||(ADC_Left_Detect >= 300)){
            event = '2';
    }
}





void circle(void){
    if((ADC_Right_Detect >= 200)||(ADC_Left_Detect >= 200)){
        Forward_Slow();
    }else if((ADC_Right_Detect < 200)&&(ADC_Left_Detect > 200)){
        LeftAd();
    }else if((ADC_Left_Detect < 200)&&(ADC_Right_Detect > 200)){
        RightAd();
    }else if((ADC_Left_Detect < 200)&&(ADC_Right_Detect < 200)) {
        RightAd();
     }
  }


void wait(void){
    strcpy(display_line[3], "   wait   ");
    LEFT_FORWARD_SPEED = 0;
    RIGHT_REVERSE_SPEED = 20000;
    LEFT_REVERSE_SPEED = 13000;
    RIGHT_FORWARD_SPEED = 0;
    if(countz3 >= 60){
            event = '2';
    }
}

void run(void){
    strcpy(display_line[3], "   run    ");
    Forward_Slow();
    if((ADC_Right_Detect > 300)||(ADC_Left_Detect > 300)) {
        countz3 = 0;
        event = '3';
    }
}

void Semi1(void){
    strcpy(display_line[3], "   SEMI   ");
    LEFT_FORWARD_SPEED = 0;
    RIGHT_REVERSE_SPEED = 20000;
    LEFT_REVERSE_SPEED = 13000;
        RIGHT_FORWARD_SPEED = 0;
        if((ADC_Left_Detect >= 300)||(ADC_Right_Detect >= 300) ){
                    Forward_Off();
                    event = '3';
                    countz3 = 0;

        }
}

void stop(void){
    strcpy(display_line[3], "   stop   ");
    Forward_Off();
    if(countz3 >= 100){
        event = '4';
    }
}

void find(void){
    strcpy(display_line[3], "   find   ");
        LEFT_REVERSE_SPEED = SLOW;
        RIGHT_FORWARD_SPEED = SLOW;
        if((ADC_Left_Detect >= 300)||(ADC_Right_Detect >= 300) ){
            Forward_Off();
            event = '5';
            countz3 = 0;
        }
}

void finish(void){
    strcpy(display_line[3], "   circle   ");
    circle();
    if(countz3 == 1400){
        event = '6';
        countz3 = 0;
    }
}


void turn(void){
Clockwise();
    if(countz3 > 20){
    event = '7';
    countz3 = 0;
    }
}

void center(void){
    strcpy(display_line[3], "  center  ");
    Forward_Slow();
    if(countz3 >= 55){
        event = '8';
    }
}

void end(void){
    strcpy(display_line[3], "  finish  ");
    Forward_Off();
}





