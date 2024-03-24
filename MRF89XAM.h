/*
 * MRF89XAM.h
 * ----------
 * Header file for usage of the MRF89XAM transceiver module. Includes SPI setup,
 * receiving, transmitting, and decoding, and encoding code.
 */

#ifndef MRF89XAM_H
#define	MRF89XAM_H

#include "BeeBot_Globals.h"

/******************************************************************************/
/*                            Function Declarations                           */
/******************************************************************************/
void init_MRF89XAM           (void);                        //Initialize MRF89XAM transceiver.
void write_spi_reg           (BYTE reg_address, BYTE data); //Write to MRF89XAM register.
BYTE read_spi_reg            (BYTE reg_address);            //Read from MRF89XAM register.
void write_spi_data          (BYTE data);                   //Write to MRF89XAM data buffer.
BYTE read_spi_data           (void);                        //Read from MRF89XAM data buffer.
void transmit_MRF89XAM       (BYTE data);                   //Transmit single byte on MRF89XAM.
void transmit_string_MRF89XAM(char tx_str[]);               //Transmit string on MRF89XAM.
BYTE receive_MRF89XAM        (void);                        //Receive byte from MRF89XAM.
void MRF_parse_message(char message[], char code[]);        //Return message content and parameters.

/******************************************************************************/
/*                              Global Variables                              */
/******************************************************************************/
char MRF_param1[10];
char MRF_param2[10];
BYTE MRF_transmitting             = FALSE;          //Indicate if MRF89XAM is transmitting
char MRF_message[MSG_MAX] = "";                     //SPI message receive string.
BYTE MRF_message_received         = FALSE;          //True if SPI message is available.

#endif	/* MRF89XAM_H */

