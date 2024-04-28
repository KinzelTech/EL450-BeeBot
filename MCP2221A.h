/*
 * MCP2221A.h
 * ----------
 * C header file for usage of the MCP2221A usb-to-uart module. Included is setup 
 * for UART2, receiving, transmitting, decoding, and encoding code. 
 */

#ifndef MCP2221A_H
#define	MCP2221A_H

/******************************************************************************/
/*                            Headers & Libraries                             */
/******************************************************************************/
#include "BeeBot_Globals.h"

/******************************************************************************/
/*                              Global Variables                              */
/******************************************************************************/
BYTE usb_message_received   = FALSE;            //Signals a message in UART1 receive buffer.
char usb_message[100] = "";
BYTE usb_received = FALSE;
char USB_param1[10];
char USB_param2[10];

char temperatures[25][9];
char humidities[25][9];
char batteries[25][9];
char times[25][9];



/******************************************************************************/
/*                           Function Declarations                            */
/******************************************************************************/
void init_uart2         (void);             //Initialize UART2.
BYTE read_byte_usb      (void);             //Read a byte from UART2 receive buffer.
void transmit_byte_usb  (BYTE message);     //Transmit a byte via UART2.
void transmit_string_usb(char message[]);   //Transmit a string via UART2.
void parse_usb_message  (char message[]);   //Handle a received usb message.
COORDS get_coords_usb   (void);
void insertPeriod(char *str);
//void format_coords(COORDS coordinate, char output[]);

#endif	/* MCP2221A_H */

