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

/******************************************************************************/
/*                            Global Constants                                */
/******************************************************************************/
#define TRUE             1      //Boolean true.
#define FALSE            0      //Boolean false.
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

/******************************************************************************/
/*                           Custom Data Types                                */
/******************************************************************************/
typedef unsigned char BYTE; //Single byte data type

/******************************************************************************/
/*                          Function Declarations                             */
/******************************************************************************/
void append_string(char str[], char ch);    //Append a character to the end of a formatted string.

#endif	/* BEEBOT_GLOBALS_H */

