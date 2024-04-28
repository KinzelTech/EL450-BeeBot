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
    TRISDbits.TRISD7 = 1;   //Set RX2 to input.
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
    BYTE content = RCREG2;
    
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
    TXREG2 = message;
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
        while(PIR3bits.TX2IF == FALSE);
        transmit_byte_usb(message[counter]);
    }
    transmit_byte_usb('\n');
    return;
}

/******************************************************************************/
/*                Verifies that USB message is a valid command                */
/******************************************************************************/
void parse_usb_message(char message[])
{
    BYTE counter;
    BYTE param1_full;
    BYTE length;
    char response[200] = "";
    char temp_string[10] = "";
    float percentage = 0.0f;
    
    usb_message_received = FALSE;
    
    //Parse USB code
    
    if(!strncmp(usb_message, "REPORT_ALL", 10))
    {
        //Send back all reports over 24 hours.
        for(counter = 0; counter <= hour_counter; counter++)
        {
            transmit_string_usb("**********************************");
            strcpy(response,    "Report:\n");
            strcat(response,    "     Temperature = ");
            strcat(response,    temperatures[counter]);
            strcat(response,    "F");
      
            strcat(response,    "\n\n     Humidity = ");
            strcat(response,    humidities[counter]);
            strcat(response,    "%");
        
            //Report time and date
            strcat(response, "\n\n     Hour = ");
            itoa(counter, temp_string);
            strcat(response, temp_string);
            strcat(response, "\n\n     Time (UTC) = ");
            strncat(response, times[counter], 6);

            //Report battery percentage
            strcat(response, "\n\n     Battery % = ");
            strcat(response, batteries[counter]);
            transmit_string_usb(response);
        }
    }
    else if(!strncmp(usb_message, "REPORT:", 6))
    {
        //Send a report message.
        //Report temperature and humidity
        strcpy(response,    "Report:\n");
        strcat(response,    "     Temperature = ");
        itoa  (temperature, temp_string);
        
        strcpy(temperatures[hour_counter], temp_string);
        
        strcat(response,    temp_string);
        strcat(response,    "F");
        strcat(response,    "\n\n     Humidity = ");
        itoa  (humidity,    temp_string);
        
        strcpy(humidities[hour_counter], temp_string);
        
        strcat(response,    temp_string);
        strcat(response,    "%");
        
        //Report time and date
        strcat(response, "\n\n     Hour = ");
        itoa(hour_counter, temp_string);
        strcat(response, temp_string);
        strcat(response, "\n\n     Date (UTC) = ");
        strncat(response, date_utc, 6);
        strcat(response, "\n\n     Time (UTC) = ");
        
        strcpy(times[hour_counter], time_utc);
 
        strncat(response, time_utc, 6);
        
        //Report current coordinates
        strcat(response,    "\n\n     Current Location = (");
        ltoa  (current_coords.latitude, temp_string);
        insertPeriod(temp_string);
        strcat(response,    temp_string);
        strcat(response,    ", ");
        ltoa  (current_coords.longitude,  temp_string);
        insertPeriod(temp_string);
        strcat(response,    temp_string);
        strcat(response,    ")");
        transmit_string_usb(response);
        
        //Report home coordinates
        strcpy(response,    "     Home = (");
        ltoa  (home.latitude,  temp_string);
        insertPeriod(temp_string);
        strcat(response,    temp_string);
        strcat(response,    ", ");
        ltoa  (home.longitude, temp_string);
        insertPeriod(temp_string);
        strcat(response,    temp_string);
        strcat(response,    ")");
        transmit_string_usb(response);
        
        
        //Report Waypoint coordinates
        for(counter = 0; counter < 4; counter++)
        {
            strcpy(response, "     Waypoint");
            append_string(response, (char) (counter+1+48));  //Add 48 to convert to equivalent ASCII.
            
            strcat(response, " = ");
            strcat(response, "(");
            ltoa  (waypoints[counter].latitude, temp_string);
            insertPeriod(temp_string);
            strcat(response,    temp_string);
            strcat(response,    ", ");
            ltoa  (waypoints[counter].longitude,  temp_string);
            insertPeriod(temp_string);
            strcat(response,    temp_string);
            strcat(response,    ")");
            transmit_string_usb(response);
        } 
        
        //Report battery voltage and percentage
        sprintf(response, "     Battery Voltage = %.2f", battery_voltage);
        transmit_string_usb(response);
        sprintf(response, "     Battery %% = %.2f%%", battery_percentage);
        
        itoa((int)battery_percentage, temp_string);
        strcpy(batteries[hour_counter], temp_string);
        
        transmit_string_usb(response);
    }
    else if(!strncmp(usb_message, "SET_HOME", 8))
    {
        //Set home address to current location.
        transmit_string_usb("*****SETTING HOME ADDRESS*****");
        home = current_coords;
    }
    else if(!strncmp(usb_message, "RECALL", 6))
    {
        //Set destination to home address.
        transmit_string_usb("*****RECALLING TO HOME*****");
        waypoints[3] = home;
    }         
    else if(!strncmp(usb_message, "WAYPOINT1", 9))
    {
        //Set Waypoint 1 to an address.
        transmit_string_usb("*****WAYPOINT1 SET*****");
        waypoints[0] = get_coords_usb();
    }
    else if(!strncmp(usb_message, "WAYPOINT2", 9))
    {
        //Set Waypoint 2 to an address.
        transmit_string_usb("*****WAYPOINT2 SET*****");
        waypoints[1] = get_coords_usb();
    }
    else if(!strncmp(usb_message, "WAYPOINT3", 9))
    {
        //Set Waypoint 3 to an address.
        transmit_string_usb("*****WAYPOINT3 SET*****");
        waypoints[2] = get_coords_usb();
    }
    else if(!strncmp(usb_message, "DESTINATION", 11))
    {
        //Set final destination to an address.
        transmit_string_usb("*****DESTINATION SET*****");
        waypoints[3] = get_coords_usb();
    }
    else if(!strncmp(usb_message, "START", 5))
    {
        //Start navigation.
        transmit_string_usb("*****STARTING*****");
    }
    else if(!strncmp(usb_message, "STOP", 4))
    {
        //Stop movement.
        transmit_string_usb("*****STOPPING*****");
    }
    else if(!strncmp(usb_message, "AUTO", 4))
    {
        //Set BeeBot to autonomous mode.
        transmit_string_usb("*****AUTO MODE SET*****");
    }
    else if(!strncmp(usb_message, "MANUAL", 6))
    {
        //Set BeeBot to manual mode.
        transmit_string_usb("*****MANUAL MODE SET*****");
    }
    else if(!strncmp(usb_message, "STROBE", 6))
    {
        //Toggle strobe light.
        LATAbits.LA0 = !LATAbits.LA0;
    }
    else
    {
       //Send back an unrecognized message error. 
        transmit_string_usb("*****UNRECOGNIZED COMMAND*****");
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
    BYTE dec_counter = 0;
    
    char *message = usb_message;
    
    //Longitude Parsing
    while(!isdigit(*message) || (*(message+1)) == ' ')
    {
        message++;
        if(*message == '\0')
            break;
    }   
    while(isdigit(*message))
    {
        append_string(longitude, *message);
        message++;
    }   
    message++;
    dec_counter = 0;
    while(isdigit(*message) && dec_counter < 5)
    {
        append_string(longitude, *message);
        dec_counter++;
        message++;
    }
    
    //Latitude Parsing
    while(!isdigit(*message))
    {
        message++;
        if(*message == '\0')
            break;
    }
    while(isdigit(*message))
    {
        append_string(latitude, *message);
        message++;
    }
    message++;
    dec_counter = 0;
    while(isdigit(*message) && dec_counter < 5)
    {
        append_string(latitude, *message);
        dec_counter++;
        message++;
    }
    
    msg_coords.longitude = atol(longitude);
    msg_coords.latitude  = atol(latitude);
    return msg_coords;
}

/******************************************************************************/
/*                  Insert period into formatted coordinates                  */
/******************************************************************************/
void insertPeriod(char *str) 
{
    int len = (int) strlen(str);
    if (len == 8) 
    {
        // Shift characters to the right to make space for the period
        memmove(&str[4], &str[3], (unsigned int) len - 3);
        str[3] = '.';
    } 
    else if (len == 7) {
        // Shift characters to the right to make space for the period
        memmove(&str[3], &str[2], (unsigned int) len - 2);
        str[2] = '.';
    }
}