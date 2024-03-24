/*
 * MRF89XAM.c
 * ----------
 * C source file for usage of the MRF89XAM transceiver module. Includes SPI setup,
 * receiving, transmitting, and decoding, and encoding code.
 */

#include "MRF89XAM.h"

/******************************************************************************/
/******************************MRF89XAM FUNCTIONS******************************/
/******************************************************************************/
/******************************************************************************/
/*                     Write to register in the MRF89XAM                      */
/******************************************************************************/
void write_spi_reg(BYTE reg_address, BYTE data)
{
    BYTE address_byte = (BYTE)(reg_address << 1);
    address_byte &= 0b00111110;             //Set byte as (0 0 A4 A3 A2 A1 A0 0)
    
    BYTE dummy = 0x00;
    
    //Select command
    LATCbits.LC0 = 0;
    
    //Transmit address
    dummy = SSP2BUF;
    SSP2BUF = address_byte;
    while (!SSP2STATbits.BF);
    
    //Transmit data
    dummy = SSP2BUF;
    SSP2BUF = data;
    while (!SSP2STATbits.BF);
    dummy = SSP2BUF;
    
    //Unselect command
    LATCbits.LC0 = 1;
    return;
}

/******************************************************************************/
/*                    Read from a register in the MRF89XAM                    */
/******************************************************************************/
BYTE read_spi_reg(BYTE reg_address)
{
    BYTE data = 0x00;
    BYTE dummy;
    BYTE address_byte = (BYTE)(reg_address << 1);
    address_byte &= 0b00111110;             
    address_byte |= 0b01000000;             //Set byte as (0 1 A4 A3 A2 A1 A0 0)
    
    //Select command
    LATCbits.LC0 = 0;
    
    //Transmit address
    dummy = SSP2BUF;
    SSP2BUF = address_byte;
    while (!SSP2STATbits.BF);          //Reset flag
    
    //Transmit junk message to prompt return
    dummy = SSP2BUF;
    SSP2BUF = 0x00;
    while (!SSP2STATbits.BF);
    data = SSP2BUF;
    
    //Unselect command
    LATCbits.LC0 = 1;
    
    return data;
}

/******************************************************************************/
/*                Write data to be transmitted to the MRF89XAM                */
/******************************************************************************/
void write_spi_data(BYTE data)
{
    BYTE dummy;
    
    //Select data
    LATCbits.LC1 = 0;
    
    //Transmit data
    dummy = SSP2BUF;
    SSP2BUF = data;
    while (!SSP2STATbits.BF);          //Reset flag
    dummy = SSP2BUF;
    
    //Unselect data
    LATCbits.LC1 = 1;
    
    return;
}

/******************************************************************************/
/*                   Read data received from the MRF89XAM                     */
/******************************************************************************/
BYTE read_spi_data(void)
{
    BYTE dummy;
    BYTE data = 0x00;
    
    //Select data
    LATCbits.LC1 = 0;
    
    //Read data
    dummy = SSP2BUF;
    SSP2BUF = 0x00;
    while (!SSP2STATbits.BF);          //Reset flag
    data = SSP2BUF;
    
    //Unselect data
    LATCbits.LC1 = 1;
    
    return data;
}

/******************************************************************************/
/*                       Initialize MRF89XAM registers                        */
/******************************************************************************/
void init_MRF89XAM(void)
{
    write_spi_reg(0x00, 0b01100010);    //Write GCONREG to receive mode, 915MHz FSK
    write_spi_reg(0x01, 0b10001100);    //Write DMODREF to set to packet dmode.
    write_spi_reg(0x05, 0b00000001);    //Write FIFOCREG to set FIFO size and threshold interrupt bits (change to higher value later)
    write_spi_reg(0x0D, 0b10111000);    //Write FTXRXIREG to set all interrupts except IRQ0 transmit
    write_spi_reg(0x0E, 0b00011101);    //Write FTPRIRREG to set transmit interrupts
    write_spi_reg(0x0F, 0b00000001);    //Write TSTHIREG to set FIFO threshold value (starting with just 1, probably increase to 4)
    write_spi_reg(0x1B, 0b00111100);    //Write CLKOUTREG to disable clock out (not usable on this board)
    write_spi_reg(0x1C, 0b00000001);    //Write PLOADREG to set packet size to 1 byte (will probably increase to 4)
    return;
}

/******************************************************************************/
/*                Transmit a single byte using the MRF89XAM                   */
/******************************************************************************/
void transmit_MRF89XAM(BYTE data)
{
    data <<= data;  //Necessary evil, transmission right shifts data
    
    MRF_transmitting = TRUE;
    write_spi_reg(0x00, 0b00100010);    //Set transceiver into standby mode
    write_spi_data(data);               //Send data to module via SPI
    write_spi_reg(0x00, 0b10000010);    //Set transceiver into transmit mode
    
    while(!PORTBbits.RB5);              //Wait for transmission to start        (IRQ0=1)
    while(!PORTBbits.RB4);              //Wait for transmission to end          (IRQ1=1)
    MRF_transmitting = FALSE;
    return;
}

/******************************************************************************/
/*             Transmit a string of characters using the MRF89XAM             */
/******************************************************************************/
void transmit_string_MRF89XAM(char tx_str[])
{
    unsigned int length = strlen(tx_str);
    BYTE counter;
    
    for(counter = 0; counter < length; counter++)
    {
        transmit_MRF89XAM((BYTE) tx_str[counter]);
    }
    transmit_MRF89XAM('\n');
    return;
}

/******************************************************************************/
/*                 Receive a single byte using the MRF89XAM                   */
/******************************************************************************/
BYTE receive_MRF89XAM(void)
{
    while(PORTBbits.RB5);
    return read_spi_data();
}

/******************************************************************************/
/*              Parse MRF received message, returning its code.               */
/******************************************************************************/
void MRF_parse_message(char message[], char code[])
{
    BYTE valid = FALSE;
    BYTE counter;
    BYTE length = (BYTE) strlen(message);
    BYTE param1_full = FALSE;
    
    if(!strcmp(message, MSG_REPORT))
    {
        valid = TRUE;
        strcpy(code, message);
        //Send message here with current temperature, humidity, battery, and location data.
        //transmit_string_MRF89XAM(current_report));
    }
    else if(!strcmp(message, MSG_REPORT_ALL))
    {
        valid = TRUE;
        strcpy(code, message);
        //counter = 0;
        //while(strcmp(all_reports[counter], "") && counter < MAX_REPORTS)
        //{    
        //    transmit_string_usb(all_reports[counter]);
        //    counter++;
        //}
    }
    else if(!strcmp(message, MSG_SET_HOME))
    {
        valid = TRUE;
        strcpy(code, message);
        //Set home coordinates to current coordinates.
        //home_latitude  = get_current_latitude();
        //home_longitude = get_current_longitude();
    }
    else if(!strcmp(message, MSG_STOP))
    {
        valid = TRUE;
        strcpy(code, message);
        //Instruct the BeeBot to stop moving, and go into Waiting state.
        //previous_state = state;
        //state = WAITING;
    }
    else if(!strcmp(message, MSG_RESUME))
    {
        valid = TRUE;
        strcpy(code, message);
        //Go from Waiting to Auto state.
        //if(state != AUTO)
        //{
        //  Previous state = state.
        //  state = AUTO;
        //}
    }
    else if(!strcmp(message, MSG_RECALL))
    {
        valid = TRUE;
        strcpy(code, message);
        //Instruct the BeeBot to go into Auto state and set destination to home.
        //destination_latitude = home_latitude;
        //destination_longitude = home_longitude;
        //if(state != AUTO)
        //{
        //  Previous state = state.
        //  state = AUTO;
        //}
        
    }
    else if(message[0] == 'A' || message[0] == 'M')
    {
        valid = TRUE;
        code[0] = message[0];
        code[1] = '\0';
    }
    else if(message[0] == 'N' && message[1] == 'A' && message[2] == 'V')
    {
        strcpy(MRF_param1,  "");
        strcpy(MRF_param2,  "");
        code[0] = message[0];
        code[1] = message[1];
        code[2] = message[2];
        code[3] = '\0';
        
        //Using 7 digits longitude and latitude (8 including point)
        for(counter = 0; counter < length; counter++)
        {
            if(isdigit(message[counter]) || message[counter] == '.')
            {
                if(!param1_full)
                {
                    append_string(MRF_param1, message[counter]);
                    if(!(isdigit(message[counter+1]) || message[counter+1] == '.'))
                        param1_full = TRUE;
                }
                else
                    append_string(MRF_param2, message[counter]);
            }
        }
    }
    else if(message[0] == 'J')
    {
        strcpy(MRF_param1,  "");
        strcpy(MRF_param2,  "");
        
        //Using 4 digits joystick positions (stored in 128 value range)
        append_string(MRF_param1, message[1]);
        append_string(MRF_param2, message[2]);
        code[0] = message[0];
        code[1] = '\0';
    }
    return;
}
