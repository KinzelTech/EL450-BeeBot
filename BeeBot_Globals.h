/*
 * BeeBot_Globals.h
 * ----------
 * C header file for global variables, constants, and functions needed for the
 * BeeBot project. Only place entities in this header if they are needed across
 * multiple different files of the project. 
 */

#ifndef BEEBOT_GLOBALS_H
#define	BEEBOT_GLOBALS_H

/******************************************************************************/
/*                            External Libraries                              */
/******************************************************************************/
#include <xc.h>             //XC8 default library
#include <pic18f46k22.h>    //PIC MCU default library
#include <string.h>         //C Strings library
#include <math.h>           //C Math library
#include <ctype.h>          //isdigit
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************/
/*                            Global Constants                                */
/******************************************************************************/
#define TRUE             1      //Boolean true.
#define FALSE            0      //Boolean false.
#define AUTO             1
#define MANUAL           0
#define MSG_MAX          40     //Maximum message length
#define MSG_REPORT       "R"    //Instruct the BeeBot to return current status.
#define MSG_REPORT_ALL   "RA"   //Return all hourly reports since deployment.
#define MSG_SET_HOME     "H"    //Set current location as home.
#define MSG_NAV          "NAV"  //Navigate to GPS location which follows this command.
#define MSG_STOP         "STP"  //Stop movement.
#define MSG_RESUME       "RES"  //Resume movement.
#define MSG_RECALL       "REC"  //Recall robot to home location.
#define MSG_AUTO         "A"    //Set robot into auto mode (default).
#define MSG_MANUAL       "M"    //Set robot into manual mode.
#define MSG_JOYSTICKS    "J"    //Report both joystick positions which follow.
#define _XTAL_FREQ  8000000     //Set internal clock speed to 8MHz.
#define LEFT 0
#define RIGHT 1

enum joystick_states
{
    FORWARD,
    BACKWARDS,
    NO_MOVEMENT
};

/******************************************************************************/
/*                            Custom Data Types                               */
/******************************************************************************/
typedef unsigned char BYTE; //Single byte data type

/******************************************************************************/
/*                                Structures                                  */
/******************************************************************************/
struct coords
{
    long longitude;
    long latitude;
};
typedef struct coords COORDS;

/******************************************************************************/
/*                             Global Variables                               */
/******************************************************************************/
COORDS home           = {0, 0};

COORDS waypoints[4] = {{0,0}, {0,0}, {0,0}, {0,0}}; 
COORDS current_coords = {12345678, 34512121};

int temperature = 70;
int humidity    = 20;
BYTE start      = FALSE;
BYTE mode       = AUTO;
BYTE left_joystick = 1;
BYTE right_joystick = 1;
float battery_percentage;
float battery_voltage = 0.0f;

char time_utc[10] = "";
char date_utc[10] = "";
BYTE hour_counter = 0;

/******************************************************************************/
/*                          Function Declarations                             */
/******************************************************************************/
void append_string(char str[], char ch);    //Append a character to the end of a formatted string.
void bubbleSort(BYTE arr[], BYTE n);        //Sort an array.
void itoa(int n, char s[]);                 //Integer to an equivalent string.
void reverse_string(char s[]);              //Reverse a string.
void ftoa(float num, char *str, int decimalPlaces);
void ltoa(long n, char s[]);
//void bubbleSort(BYTE arr[], BYTE n);

#endif	/* BEEBOT_GLOBALS_H */

