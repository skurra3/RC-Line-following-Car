///*
// * shapes.c
// *
// *  Created on: Feb 10, 2025
// *      Author: skurr
// */
//
//#include  "msp430.h"
//#include  <string.h>
//#include  "functions.h"
//#include  "LCD.h"
//#include  "ports.h"
//#include  "macros.h"
//
//
//volatile char slow_input_down;
//extern char display_line[4][11];
//extern char *display[4];
//unsigned char display_mode;
//extern volatile unsigned char display_changed;
//extern volatile unsigned char update_display;
//extern volatile unsigned int update_display_count;
//extern volatile unsigned int Time_Sequence;
//extern volatile char one_time;
//unsigned int test_value;
//char chosen_direction;
//char change;
//
//
//unsigned int time_change;
//unsigned int cycle_time;
//unsigned int state = 'W';
//unsigned int right_motor_count;
//unsigned int left_motor_count;
//unsigned int delay_start;
//unsigned int segment_count;
//unsigned int tricount;
//unsigned int eightcount = 0;
//
//
//
////STATEMACHINES
/////
//
//////STRAIGHT LINE
//void Run_Straight(void){
//    switch(state){
//        case WAIT: // Begin
//            wait_case();
//            break; //
//        case START: // Begin
//            start_case();
//            break; //
//        case RUN: // Run
//            run_case();
//            break; //
//        case END: // End
//            end_case();
//            break; //
//default: break;
//    }
//}
//
//void Run_Triangle(void){
//    switch(state){
//        case WAIT: // Begin
//            wait_case();
//            break; //
//        case START: // Begin
//            start_case();
//            break; //
//        case RUN: // Run
//            triangle_run_case();
//            break; //
//        case END: // End
//            end_case();
//            break; //
//default: break;
//    }
//}
//
//void Run_Eight(void){
//    switch(state){
//        case WAIT: // Begin
//            wait_case();
//            break; //
//        case START: // Begin
//            start_case();
//            break; //
//        case RUN: // Run
//            eight_run_case();
//            break; //
//        case END: // End
//            end_case();
//            break; //
//default: break;
//    }
//}
//
//
//
//void wait_case(void){
//    if(time_change){
//        time_change = 0;
//        if(delay_start++ >= WAITING2START){
//            delay_start = 0;
//            state = START;
//        }
//    }
//}
//
//void start_case(void){
//    cycle_time = 0;
//    right_motor_count = 0;
//    left_motor_count = 0;
//    Forward_Move();
//    segment_count = 0;
//    state = RUN;
//}
//
//void run_case(void){
//    if(time_change){
//        time_change = 0;
//        if(segment_count <= TRAVEL_DISTANCE){
//            if(right_motor_count++ >= RIGHT_COUNT_TIME){
//                P6OUT &= ~R_FORWARD;
//            }
//            if(left_motor_count++ >= LEFT_COUNT_TIME){
//                P6OUT &= ~L_FORWARD;
//            }
//            if(cycle_time >= WHEEL_COUNT_TIME){
//                cycle_time =     0;
//                right_motor_count = 0;
//                left_motor_count = 0;
//                segment_count++;
//                Forward_Move();
//            }
//}else{
//    state = END;
//        }
//    }
//}
//
//void triangle_straight(void){
//    if(time_change){
//        time_change = 0;
//        if(segment_count <= 6){//distance
//            if(right_motor_count++ >= 30){
//                P6OUT &= ~R_FORWARD;
//            }
//            if(left_motor_count++ >= 30){
//                P6OUT &= ~L_FORWARD;
//            }
//            if(cycle_time >= 30){
//                cycle_time =     0;
//                right_motor_count = 0;
//                left_motor_count = 0;
//                segment_count++;
//                Forward_Move();
//            }
//}else{
//        segment_count = 0;
//        tricount++;
//        }
//    }
//}
//
//void triangle_corner(void){
//    if(time_change){
//        time_change = 0;
//        if(segment_count <= 22){//distance
//            if(right_motor_count++ >= 10){
//                P6OUT &= ~R_FORWARD;
//            }
//            if(left_motor_count++ >= 0){
//                P6OUT &= ~L_FORWARD;
//            }
//            if(cycle_time >= 10){
//                cycle_time =     0;
//                right_motor_count = 0;
//                left_motor_count = 0;
//                segment_count++;
//                Forward_Move();
//            }
//}else{
//    segment_count = 0;
//    tricount++;
//        }
//    }
//}
//
//void eight_1(void){
//    if(time_change){
//        time_change = 0;
//        if(segment_count <= 42){//distance
//            if(right_motor_count++ >= 20){
//                P6OUT &= ~R_FORWARD;
//            }
//            if(left_motor_count++ >= 2){
//                P6OUT &= ~L_FORWARD;
//            }
//            if(cycle_time >= 20){
//                cycle_time =     0;
//                right_motor_count = 0;
//                left_motor_count = 0;
//                segment_count++;
//                Forward_Move();
//            }
//}else{
//        segment_count = 0;
//        eightcount++;
//        }
//    }
//}
//
//void eight_2(void){
//    if(time_change){
//        time_change = 0;
//        if(segment_count <= 45){//distance
//            if(right_motor_count++ >= 2){
//                P6OUT &= ~R_FORWARD;
//            }
//            if(left_motor_count++ >= 20){
//                P6OUT &= ~L_FORWARD;
//            }
//            if(cycle_time >= 20){
//                cycle_time =     0;
//                right_motor_count = 0;
//                left_motor_count = 0;
//                segment_count++;
//                Forward_Move();
//            }
//}else{
//        segment_count = 0;
//        eightcount++;
//        }
//    }
//}
//
//void triangle_run_case(void){
//if(tricount == 0){
//    triangle_straight();
//}else if(tricount == 1){
//    triangle_corner();
//}else if(tricount == 2){
//    triangle_straight();
//}else if(tricount == 3){
//    triangle_corner();
//}else if(tricount == 4){
//    triangle_straight();
//}else if(tricount == 5){
//    triangle_corner();
//}else if(tricount == 6){
//    triangle_straight();
//}else if(tricount == 7){
//        triangle_corner();
//}else if(tricount == 8){
//        triangle_straight();
//}else if(tricount == 9){
//        triangle_corner();
//}else if(tricount == 10){
//        triangle_straight();
//}else{
//    state = END;
//    }
//}
//
//void eight_run_case(void){
//if(eightcount == 0){
//    eight_1();
//}else if(eightcount == 1){
//    eight_2();
//}else if(eightcount == 2){
//    eight_1();
//}else if(eightcount == 3){
//    eight_2();
//}else{
//    state = END;
//    }
//}
//
//void end_case(void){
//    Forward_Off();
//    state = WAIT;
//    event = NONE;
//}
//
//
