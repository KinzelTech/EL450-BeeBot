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
<<<<<<< Updated upstream
char usb_message[MSG_MAX] = "";
=======
char usb_message[100] = "";
>>>>>>> Stashed changes
BYTE usb_received = FALSE;
char USB_param1[10];
char USB_param2[10];

/******************************************************************************/
/*                           Function Declarations                            */
/******************************************************************************/
void init_uart2         (void);             //Initialize UART2.
BYTE read_byte_usb      (void);             //Read a byte from UART2 receive buffer.
void transmit_byte_usb  (BYTE message);     //Transmit a byte via UART2.
void transmit_string_usb(char message[]);   //Transmit a string via UART2.
void parse_usb_message  (char message[]);   //Handle a received usb message.
<<<<<<< Updated upstream
=======
COORDS get_coords_usb   (void);
>>>>>>> Stashed changes

#endif	/* MCP2221A_H */

