/*
 * MCP2221A.c
 * ----------
 * C source file for usage of the MCP2221A usb-to-uart module. Included is setup 
 * for UART2, receiving, transmitting, decoding, and encoding code. 
 */

#include "MCP2221A.h"

/******************************************************************************/
/*                   Initializing UART in asynchronous mode                   */
/******************************************************************************/
void init_uart2(void)
{
    TXSTA2bits.BRGH  = 1;   //Set to high speed baud rate mode.
    SPBRG2           = 51;  //Set baud rate timer for 9600 (value from datasheet table)
    TXSTA2bits.SYNC  = 0;   //Set UART2 to asynchronous comms.
    RCSTA2bits.SPEN  = 1;   //Enable UART2 and sets TX to output.
    TRISDbits.TRISD7 = 1;   //Set RX1 to input.
    TXSTA2bits.TX9   = 0;   //Set to 8-bit transmit (default).
    RCSTA2bits.RX9   = 0;   //Set to 8-bit receive (default).
    RCSTA2bits.CREN  = 1;   //Enable UART2 receiver.
    TXSTA2bits.TXEN  = 1;   //Enable UART2 transmitter.
    return;
}

/******************************************************************************/
/*                   Read one byte from UART2 receive buffer.                  */
/******************************************************************************/
BYTE read_byte_usb(void)
{
    BYTE content = RCREG1;
    
    //Check if receive buffer overrun error has occurred.
    if(RCSTA2bits.OERR)
    {
        RCSTA2bits.CREN = 0;    //Clear and reset CREN to clear error.
        RCSTA2bits.CREN = 1;    //UART will not operate until error is cleared.
    }
    return content;
}

/******************************************************************************/
/*                        Transmit one byte via UART2                          */
/******************************************************************************/
void transmit_byte_usb(BYTE message)
{
    TXREG1 = message;
    return;
}

/******************************************************************************/
/*                        Transmit a string via UART2                         */
/******************************************************************************/
void transmit_string_usb(char message[])
{
    int counter;
    transmit_byte_usb('\n');
    for(counter = 0; counter <= strlen(message); counter++)
    {
        while(PIR1bits.TX1IF == FALSE);
        transmit_byte_usb(message[counter]);
    }
    transmit_byte_usb('\n');
}



/******************************************************************************/
/*                Verifies that USB message is a valid command                */
/******************************************************************************/
void parse_usb_message(char message[])
{
    BYTE counter;
    BYTE param1_full;
    BYTE length;
    
    //Parse USB code
    if(!strcmp(usb_message, MSG_REPORT))
    {
        //Send a report message.
        //transmit_string_usb(current_report)
    }
    else if(!strcmp(usb_message, MSG_REPORT_ALL))
    {
        //Send back all reports over 24 hours.
        //counter = 0;
        //while(strcmp(all_reports[counter], "") && counter < MAX_REPORTS)
        //{    
        //    transmit_string_usb(all_reports[counter]);
        //    counter++;
        //}
    }
    else if(!strcmp(usb_message, MSG_NAV))
    {
        //Send a report message
    }         
    else if(usb_message[0] == MSG_NAV[0] && usb_message[1] == MSG_NAV[1] && usb_message[2] == MSG_NAV[2])
    {
        //Parse included GPS coords and move to destination.
        //Using 7 digits longitude and latitude (8 including point)
        for(counter = 0; counter < length; counter++)
        {
            if(isdigit(message[counter]) || message[counter] == '.')
            {
                if(!param1_full)
                {
                    append_string(USB_param1, message[counter]);
                    if(!(isdigit(message[counter+1]) || message[counter+1] == '.'))
                        param1_full = TRUE;
                }
                else
                    append_string(USB_param2, message[counter]);
            }
        }
    }
    else
    {
        //Send invalid message warning
    }
    return;
}
