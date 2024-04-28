#include "GPS.h"

/******************************************************************************/
/*               Initializing UART in asynchronous mode for HC05              */
/******************************************************************************/
void init_uart1(void)
{
    TXSTA1bits.BRGH  = 1;   //Set to high speed baud rate mode.
    SPBRG1           = 51;  //Set baud rate timer for 9600 (value from datasheet table)
    TXSTA1bits.SYNC  = 0;   //Set UART1 to asynchronous comms.
    RCSTA1bits.SPEN  = 1;   //Enable UART1 and sets TX to output.
    TRISCbits.TRISC7 = 1;   //Set RX1 to input.
    TXSTA1bits.TX9   = 0;   //Set to 8-bit transmit (default).
    RCSTA1bits.RX9   = 0;   //Set to 8-bit receive (default).
    RCSTA1bits.CREN  = 1;   //Enable UART1 receiver.
    TXSTA1bits.TXEN  = 1;   //Enable UART1 transmitter.
    return;
}

/******************************************************************************/
/*                   Read one byte from UART receive buffer.                  */
/******************************************************************************/
BYTE read_byte_gps(void)
{
    BYTE content = RCREG1;
    
    //Check if receive buffer overrun error has occurred.
    if(RCSTA1bits.OERR)
    {
        RCSTA1bits.CREN = 0;    //Clear and reset CREN to clear error.
        RCSTA1bits.CREN = 1;    //UART will not operate until error is cleared.
    }
    return content;
}

//Idea: Read a string of 500 characters, and then parse it 
// Function to parse GPS data from NMEA format string
void parse_gps_data(char* latitude, char* longitude, char* time, char* speed, char* date, char* longitude_dir, char* latitude_dir) 
{
    char *token;    //Current substring
    char temp_string[40] = "";
    
    //transmit_string_usb(gps_data);
    
    // Tokenize the input string based on commas
    token = strtok(gps_data, ",");
    
    // Skip tokens until we reach the desired data
    while (token != NULL) 
    {
        //transmit_string_usb(token);
        // Check for the desired tokens and copy data to respective variables
        if(strstr(token, "GPRMC") != NULL)
        {
            token = strtok(NULL, ",");      // Skip token
            strcpy(time_str, token);            // Copy time
            
            token = strtok(NULL, ",");      // Skip token
            
            token = strtok(NULL, ",");      // Skip token
            strcpy(latitude_str, token);        // Copy latitude
            
            token = strtok(NULL, ",");      // Skip token
            strcpy(latitude_dir_str, token);    //Copy latitude direction
            
            token = strtok(NULL, ",");      // Skip token
            strcpy(longitude_str, token);       // Copy longitude
            
            token = strtok(NULL, ",");      //Increment token.
            strcpy(longitude_dir_str, token);   //Copy longitude direction.
            
            token = strtok(NULL, ",");      //Increment token.
            strcpy(speed_str, token);           //Copy speed.
            
            token = strtok(NULL, ",");
            
            token = strtok(NULL, ",");      //Increment token.
            strcpy(date_str, token);            //Copy date.
            
            break;                      // Exit loop after parsing desired data
        }
        token = strtok(NULL, ","); // Move to next token
    }
    return;
}

