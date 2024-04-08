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
<<<<<<< Updated upstream
    TRISDbits.TRISD7 = 1;   //Set RX1 to input.
=======
    TRISDbits.TRISD7 = 1;   //Set RX2 to input.
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
    BYTE content = RCREG1;
=======
    BYTE content = RCREG2;
>>>>>>> Stashed changes
    
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
<<<<<<< Updated upstream
    TXREG1 = message;
=======
    TXREG2 = message;
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
        while(PIR1bits.TX1IF == FALSE);
        transmit_byte_usb(message[counter]);
    }
    transmit_byte_usb('\n');
=======
        while(PIR3bits.TX2IF == FALSE);
        transmit_byte_usb(message[counter]);
    }
    transmit_byte_usb('\n');
    return;
>>>>>>> Stashed changes
}



/******************************************************************************/
/*                Verifies that USB message is a valid command                */
/******************************************************************************/
void parse_usb_message(char message[])
{
    BYTE counter;
    BYTE param1_full;
    BYTE length;
<<<<<<< Updated upstream
    
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
=======
    char response[100] = "";
    char temp_string[10] = "";
    
    usb_message_received = FALSE;
    
    //Parse USB code
    if(!strncmp(usb_message, "REPORT", 6))
    {
        //Send a report message.
        strcpy(response,    "Report:");
        strcat(response,    "Temperature = ");
        itoa  (temperature, temp_string);
        strcat(response,    temp_string);
        strcat(response,    ", Humidity = ");
        itoa  (humidity,    temp_string);
        strcat(response,    temp_string);
        strcat(response,    ", Current Location = (");
        ftoa  (current_coords.longitude, temp_string, 5);
        strcat(response,    temp_string);
        strcat(response,    ", ");
        ftoa  (current_coords.latitude,  temp_string, 5);
        strcat(response,    temp_string);
        strcat(response,    ")");
        transmit_string_usb(response);
    }
    else if(!strncmp(usb_message, "REPORT_ALL", 10))
    {
        //Send back all reports over 24 hours.
    }
    else if(!strncmp(usb_message, "SET_HOME", 8))
    {
        //Set home address to current location.
        home = current_coords;
    }
    else if(!strncmp(usb_message, "RECALL", 6))
    {
        //Set destination to home address.
        destination = home;
    }         
    else if(!strncmp(usb_message, "WAYPOINT1", 9))
    {
        //Set Waypoint 1 to an address.
        waypoint1 = get_coords_usb();
    }
    else if(!strncmp(usb_message, "WAYPOINT2", 9))
    {
        //Set Waypoint 2 to an address.
        waypoint2 = get_coords_usb();
    }
    else if(!strncmp(usb_message, "WAYPOINT3", 9))
    {
        //Set Waypoint 3 to an address.
        waypoint3 = get_coords_usb();
    }
    else if(!strncmp(usb_message, "DESTINATION", 11))
    {
        //Set final destination to an address.
        destination = get_coords_usb();
    }
    else if(!strncmp(usb_message, "START", 5))
    {
        //Start navigation.
    }
    else if(!strncmp(usb_message, "STOP", 4))
    {
        //Stop movement.
    }
    else if(!strncmp(usb_message, "AUTO", 4))
    {
        //Set BeeBot to autonomous mode.
    }
    else if(!strncmp(usb_message, "MANUAL", 6))
    {
        //Set BeeBot to manual mode.
    }
    else
    {
       //Send back an unrecognized message error. 
    }
        
    
    return;
}

/******************************************************************************/
/*           Extract longitude and latitude values from usb message           */
/******************************************************************************/
COORDS get_coords_usb(void)
{
    COORDS msg_coords;
    BYTE counter       = 0;
    BYTE long_counter  = 0;
    BYTE lat_counter   = 0;
    char longitude[20] = "";
    char latitude[20]  = "";
    
    //Scroll through message until longitude is found.
    while(!isdigit(usb_message[counter]))       
        counter++;
    
    //Read longitude number string.
    while(isdigit(usb_message[counter]) || usb_message[counter] == '.')
    {
        longitude[long_counter] = usb_message[counter];
        long_counter++;
        counter++;
    }
    
    //Scroll through message until latitude is found.
    while(!isdigit(usb_message[counter]))
        counter++;
    
    //Read latitude number string.
    while(isdigit(usb_message[counter]) || usb_message[counter] == '.')
    {
        longitude[long_counter] = usb_message[counter];
        long_counter++;
        counter++;
    }
    
    //Convert to equivalent floats and return value
    msg_coords.longitude = (float) atof(longitude);
    msg_coords.latitude  = (float) atof(latitude);
    return msg_coords;
}
>>>>>>> Stashed changes
