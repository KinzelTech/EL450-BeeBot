/*
 * MRF89XAM.c
 * ----------
 * C source file for usage of the MRF89XAM transceiver module. Includes SPI setup,
 * receiving, transmitting, and decoding, and encoding code.
 */

#include "MRF89XAM.h"

/******************************************************************************/
/*                       Initialize SPI communications                        */
/******************************************************************************/
void init_spi(void)
{
    SSP1CON1 = 0b00100001; //Enable MSSP1 and set to SPI host at 8M/16 = 500kHz  (64 for data?))
    SSP1STATbits.SMP = 0;  //Set SSP1STAT to operate SPI in 00 mode.
    SSP1STATbits.CKE = 1;
    SSP1CON1bits.CKP = 0;
    return;
}

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
    dummy = SSP1BUF;
    SSP1BUF = address_byte;
    while (!SSP1STATbits.BF);
    
    //Transmit data
    dummy = SSP1BUF;
    SSP1BUF = data;
    while (!SSP1STATbits.BF);
    dummy = SSP1BUF;
    
    //Unselect command
    LATCbits.LC0 = 1;
    return;
}

/******************************************************************************/
/*                    Read from a register in the MRF89XAM                    */
/******************************************************************************/
/*BYTE read_spi_reg(BYTE reg_address)
{
    BYTE data = 0x00;
    BYTE dummy;
    BYTE address_byte = (BYTE)(reg_address << 1);
    address_byte &= 0b00111110;             
    address_byte |= 0b01000000;             //Set byte as (0 1 A4 A3 A2 A1 A0 0)
    
    //Select command
    LATCbits.LC0 = 0;
    
    //Transmit address
    dummy = SSP1BUF;
    SSP1BUF = address_byte;
    while (!SSP1STATbits.BF);          //Reset flag
    
    //Transmit junk message to prompt return
    dummy = SSP1BUF;
    SSP1BUF = 0x00;
    while (!SSP1STATbits.BF);
    data = SSP1BUF;
    
    //Unselect command
    LATCbits.LC0 = 1;
    
    return data;
}*/

/******************************************************************************/
/*                Write data to be transmitted to the MRF89XAM                */
/******************************************************************************/
void write_spi_data(BYTE data)
{
    BYTE dummy;
    
    //Select data
    LATCbits.LC1 = 0;
    
    //Transmit data
    dummy = SSP1BUF;
    SSP1BUF = data;
    
    while (!SSP1STATbits.BF);          //Reset flag
    dummy = SSP1BUF;
    
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
    dummy = SSP1BUF;
    SSP1BUF = 0x00;
    while (!SSP1STATbits.BF);          //Reset flag
    data = SSP1BUF;
    
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
    write_spi_reg(0x01, 0b10101100);    //Write DMODREF to set to buffered dmode.
    write_spi_reg(0x05, 0b11001000);    //Write FIFOCREG to set FIFO size and threshold interrupt bits (change to higher value later)
    write_spi_reg(0x0D, 0b10111000);    //Write FTXRXIREG to set all interrupts except IRQ0 transmit
    write_spi_reg(0x0E, 0b00011101);    //Write FTPRIRREG to set transmit interrupts
    write_spi_reg(0x0F, 0b11111111);    //Write RSTHIREG to set RSSI threshold value
    write_spi_reg(0x1B, 0b00111100);    //Write CLKOUTREG to disable clock out (not usable on this board)
    write_spi_reg(0x1C, 0b00001000);    //Write PLOADREG to set packet size to 1 byte (will probably increase to 4)

    return;
}

/******************************************************************************/
/*             Transmit a string of characters using the MRF89XAM             */
/******************************************************************************/
void transmit_string_MRF89XAM(char tx_str[])
{
    unsigned int length = strlen(tx_str);
    BYTE counter;
    BYTE loop = TRUE;
    BYTE loop_counter = 0;
    
    //while(loop)
    //{
        write_spi_reg(0x00, 0b00100010);    //Set transceiver into standby mode
        MRF_transmitting = TRUE;
        INTCON3bits.INT1IE = 0;
        //if(length <= 8)
        if(length <= 8)
        {
            //for(counter = 0; counter < 8; counter++)
            for(counter = 0; counter < 8; counter++)
            {
                if(counter < length)
                    write_spi_data((BYTE)(tx_str[counter] << 1));
                else
                    write_spi_data((BYTE)('\n' << 1));
            }
        }
        write_spi_reg(0x00, 0b10000010);    //Set transceiver into transmit mode
    
        while(PORTBbits.RB1 == 0);              //Wait for transmission to start        (IRQ0=1)
        while(PORTBbits.RB2 == 0);              //Wait for transmission to end          (IRQ1=1)
    
        write_spi_reg(0x00, 0b01100010);    //Write GCONREG to receive mode, 915MHz FSK
        MRF_transmitting = FALSE;
        INTCON3bits.INT1IF = 0;
        INTCON3bits.INT1IE = 1;
    return;
}

/******************************************************************************/
/*                 Receive a single byte using the MRF89XAM                   */
/******************************************************************************/
BYTE receive_MRF89XAM(void)
{
    //while(PORTBbits.RB1);
    return read_spi_data();
}