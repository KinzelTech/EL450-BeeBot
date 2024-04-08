// PIC18F46K22 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config FOSC = INTIO67   // Oscillator Selection bits (Internal oscillator block)
#pragma config PLLCFG = OFF     // 4X PLL Enable (Oscillator used directly)
#pragma config PRICLKEN = ON    // Primary clock enable bit (Primary clock is always enabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRTEN = OFF     // Power-up Timer Enable bit (Power up timer disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 190       // Brown Out Reset Voltage bits (VBOR set to 1.90 V nominal)

// CONFIG2H
#pragma config WDTEN = OFF      // Watchdog Timer Enable bits (Watch dog timer is always disabled. SWDTEN has no effect.)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC1  // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<5:0> pins are configured as analog input channels on Reset)
#pragma config CCP3MX = PORTB5  // P3A/CCP3 Mux bit (P3A/CCP3 input/output is multiplexed with RB5)
#pragma config HFOFST = ON      // HFINTOSC Fast Start-up (HFINTOSC output and ready status are not delayed by the oscillator stable status)
#pragma config T3CMX = PORTC0   // Timer3 Clock input mux bit (T3CKI is on RC0)
#pragma config P2BMX = PORTD2   // ECCP2 B output mux bit (P2B is on RD2)
#pragma config MCLRE = EXTMCLR  // MCLR Pin Enable bit (MCLR pin enabled, RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled if MCLRE is also 1)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection Block 0 (Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection Block 1 (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection Block 2 (Block 2 (008000-00BFFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection Block 3 (Block 3 (00C000-00FFFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection Block 0 (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection Block 1 (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection Block 2 (Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection Block 3 (Block 3 (00C000-00FFFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection Block 0 (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection Block 1 (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection Block 2 (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection Block 3 (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)


/******************************************************************************/
/*                                Libraries                                   */
/******************************************************************************/
#include "BeeBot_Globals.h"
#include "MRF89XAM.h"
#include "MCP2221A.h"
//#include

/******************************************************************************/
/*                                 States                                     */
/******************************************************************************/
<<<<<<< Updated upstream
enum states
{
    INIT,       //System startup and initialization.
    WAITING,    //No movement, waiting on commands from user.
    AUTO,       //Autonomously moving, using obstacle avoidance and pathfinding.
    MANUAL,     //Manually controlled via controller joysticks.
    DEBUG,      //Debug mode while USB is plugged in.
    DEPLOYED    //Deployment mode while in the field.
};
=======

#define INIT 2       //System startup and initialization.
#define WAITING 3    //No movement, waiting on commands from user.
#define DEBUG 4      //Debug mode while USB is plugged in.
#define DEPLOYED 5    //Deployment mode while in the field.

>>>>>>> Stashed changes

/******************************************************************************/
/*                            Global Constants                                */
/******************************************************************************/
<<<<<<< Updated upstream
#define _XTAL_FREQ  8000000  //Set internal clock speed to 8MHz.
#define UART_TERMINATOR '\n' //Terminator character for UART.
#define MAX_REPORTS 24       //Number of reports to save.

=======

#define UART_TERMINATOR '\n' //Terminator character for UART.
#define MAX_REPORTS 24       //Number of reports to save.


>>>>>>> Stashed changes
/******************************************************************************/
/*                            Global Variables                                */
/******************************************************************************/
unsigned int timer_tick = 0;                //Increments per every Timer0 interrupt.
BYTE state = INIT;
char current_report[MSG_MAX] = "";
char all_reports[MAX_REPORTS][MSG_MAX];
<<<<<<< Updated upstream
=======
BYTE last_message_id = 0;
BYTE left_joystick = 0;
BYTE right_joystick = 0;
>>>>>>> Stashed changes

/******************************************************************************/
/*                          Function Declarations                             */
/******************************************************************************/
//Initialization Code
void init_pins      (void);         //Initialize all pins to initial values.
void init_interrupts(void);         //Initialize all program interrupts.

<<<<<<< Updated upstream
//Misc Code


=======
>>>>>>> Stashed changes
/******************************************************************************/
/*                               Main Function                                */
/******************************************************************************/
void main(void) 
{
    BYTE previous_state;
    char MRF_msg_code[4];
    //Clock setup
    OSCCON = 0b01100111;
    __delay_ms(200);
    
    //Infinite processing loop
<<<<<<< Updated upstream
=======
    
    char test_string[16] = "";
    
>>>>>>> Stashed changes
    while(TRUE)
    {
        //Put your processing code here.
        //Use functions as much as possible, instead of writing commands directly in main.
        switch(state)
        {
            case INIT:
                //Place all initialization code here.
                init_pins      ();
<<<<<<< Updated upstream
                init_interrupts();
                init_uart2     (); 
=======
                init_uart2     ();
                init_spi       ();
                init_MRF89XAM  ();
                init_interrupts();
>>>>>>> Stashed changes
                previous_state = INIT;
                state = WAITING;
                break;
            case WAITING:
<<<<<<< Updated upstream
                //Initially waiting for message to arrive.
                if(MRF_message_received)
                {
                    MRF_message_received = FALSE;
                    MRF_parse_message(MRF_message, MRF_msg_code);
=======
                
                while(TRUE)
                {   
                    if(MRF_message_received)
                    {
                        __delay_ms(10);
                        MRF_message_received = FALSE;
                        if(!strcmp(MRF_message, "hello\n\n\n"))
                        {
                            transmit_string_MRF89XAM("HI");
                        }
                        strcpy(MRF_message, "");
                    }
                    if(usb_message_received)
                    {
                        usb_message_received = FALSE;
                        transmit_string_usb(usb_message);
                        strcpy(usb_message, "");
                    }
                }
                
                //Initially waiting for message to arrive.
                if(MRF_message_received)
                {
                    LATAbits.LA0 = 1;
                    MRF_message_received = FALSE;
                    //MRF_parse_message(MRF_message, MRF_msg_code);
>>>>>>> Stashed changes
                    
                    if(!strcmp(MRF_msg_code, MSG_NAV))
                    {
                        state = AUTO;
                        previous_state = WAITING;
                    }
                    else if(!strcmp(MRF_msg_code, MSG_MANUAL))
                    {
                        state = MANUAL;
                        previous_state = WAITING;
                    }
                }
                break;
            case AUTO:
                //Place all autonomous movement and navigation code here.
                break;
            case MANUAL:
                //Place all manual control code here.
                break;
            case DEBUG:
                //Place all USB code here (except for plug-in detection).
                //Check if unplugged
                if((!PORTDbits.RD5))
                {
                    state = previous_state;
                    previous_state = DEBUG;
                }
                //Handle the usb_message
                if(strcmp(usb_message, ""))
                {
                    parse_usb_message(usb_message);
                    strcpy(usb_message, "");
                }
                break;
            case DEPLOYED:
                //Place all environmental sensor and hourly reporting code here.
                
                break;
        }
<<<<<<< Updated upstream
        
        //Check if powered USB device is connected.
        if(PORTDbits.RD5 && state != DEBUG)
        {
            previous_state = WAITING;
            state = DEBUG;
            strcpy(usb_message, "");
        }
=======
>>>>>>> Stashed changes
    }
    return;
}

<<<<<<< Updated upstream


=======
>>>>>>> Stashed changes
/******************************************************************************/
/*                            Initialize all pins                             */
/******************************************************************************/
void init_pins(void)
{
<<<<<<< Updated upstream
    TRISDbits.RD6    = 1;   //Set USB_STAT as input.
    TRISDbits.RD6    = 1;   //Set TX as input.
    TRISDbits.RD7    = 1;   //Set RX as input.
=======
    //USB Code
    TRISDbits.RD6 = 1;   //Set USB_STAT as input.
    TRISDbits.RD6 = 1;   //Set TX as input.
    TRISDbits.RD7 = 1;   //Set RX as input.
>>>>>>> Stashed changes
   
    //MRF89XAM Code
    TRISBbits.RB1 = 1;  //MRF IRQ0
    TRISBbits.RB2 = 1;  //MRF IRQ1
    TRISCbits.RC0 = 0;  //MRF CSCON
    TRISCbits.RC1 = 0;  //MRF CSDATA
    TRISCbits.RC2 = 1;  //MRF Reset
    TRISCbits.RC3 = 0;  //MRF SCK1
    TRISCbits.RC4 = 1;  //MRF SDI1 (MISO)
<<<<<<< Updated upstream
    TRISCbits.RC5 = 1;  //MRF SDO1 (MOSI)
    
    //LATbits
    LATCbits.LC0 = 1;   //CSCON is default high
    LATCbits.LC1 = 1;   //CSDATA is default high
    
=======
    TRISCbits.RC5 = 0;  //MRF SDO1 (MOSI)
    LATCbits.LC0  = 1;  //CSCON is default high
    LATCbits.LC1  = 1;  //CSDATA is default high
    
    //Strobe Light
    TRISAbits.RA0 = 0;
    LATAbits.LA0  = 0;
    
    //No analog, all digital
>>>>>>> Stashed changes
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
    ANSELD = 0;
    ANSELE = 0;
    return;
}

/******************************************************************************/
/*                       Initialize program interrupts                        */
/******************************************************************************/
void init_interrupts(void)
{
    RCONbits.IPEN    = 0;   //Disable priority levels for interrupts.
    INTCONbits.GIE   = 0;   //Disable all interrupts.
    
<<<<<<< Updated upstream
    //Place interrupt enable and setup commands here
    //...
    
    //UART2 Interrupts
    PIE3bits.RC2IE   = 1;   //Enable UART1 receive interrupt.
    //PIE3bits.TX2IE   = 1;   //Enable UART1 transmit interrupt.
                              //Only enable when needed.
    
    //Timer0 setup
    T0CON    	      = 0x08;     //Configure to 16bit timer, no prescaler.
    TMR0H             = 0xF8;     //Set preload to 63535.
    TMR0L    	      = 0x2F;
    INTCONbits.TMR0IE = 1;        //Enable Timer0 interrupts.
    
    //Turn interrupts on
    INTCONbits.PEIE  = 1;   //Enable all peripheral interrupts.
    INTCONbits.GIE   = 1;   //Enable all interrupts.
    T0CONbits.TMR0ON  = 1;  //Turn on Timer0.
=======
    //UART2 Interrupts
    PIE3bits.RC2IE   = 1;   //Enable UART1 receive interrupt.
    
    //INT1 interrupts (used for IRQ0 on MRF89XAM)
    INTCON3bits.INT1IE = 1;
    INTCON2bits.INTEDG1 = 1;  //Set INT1 interrupts only on rising edge
    
    //Turn interrupts on
    INTCONbits.PEIE   = 1;   //Enable all peripheral interrupts.
    INTCONbits.GIE    = 1;   //Enable all interrupts.
    T0CONbits.TMR0ON  = 1;   //Turn on Timer0.
>>>>>>> Stashed changes
    return;
}

/******************************************************************************/
/*               Interrupt service routine for all interrupts                 */
/******************************************************************************/
void __interrupt() ISR(void)
{
    char MRF_input = 0;
    char temp_receive_string[MSG_MAX] = "";
<<<<<<< Updated upstream
    //Add checks here for any interrupts.
    //...
    //UART2 Interrupts
    char usb_input = 0;
    if(PIR1bits.RC1IF && PIE1bits.RC1IE)
    {
        usb_input = read_byte_usb();
        append_string(usb_message, usb_input);
    }
    
    //Timer0 interrupt
    if(TMR0IE && TMR0IF) //Increment time if Timer0 is enabled and Timer0 has overflowed.
    {
        TMR0IF = 0;       //Reset Timer0 overflow flag.
        timer_tick++;     //Increment time.
        TMR0H  = 0xF8;    //Reset 16bit timer preload to 63535
        TMR0L  = 0x2F;	
    }
    
    //IRQ0 interrupt for MRF89XAM (receiving only)
    if(INTCON3bits.INT1F && !MRF_transmitting)
    {
        INTCON3bits.INT1F = 0;
        MRF_input = (char) receive_MRF89XAM();
        if(MRF_input == '\n')
        {
            MRF_message_received = TRUE;
            strcpy(MRF_message, temp_receive_string);
        }
        else
            append_string(temp_receive_string, MRF_input);
=======

    //UART2 Interrupts
    char usb_input = 0;
    if(PIR3bits.RC2IF && PIE3bits.RC2IE)
    {
        usb_input = read_byte_usb();
        if(usb_input == '\n')
            usb_message_received = TRUE;
        else
            append_string(usb_message, usb_input);
    }
    
    //IRQ0 interrupt for MRF89XAM (receiving only)
    if(INTCON3bits.INT1IE && INTCON3bits.INT1F)
    {
        INTCON3bits.INT1F = 0;
        if(MRF_message_received == FALSE)            //Don't process another message until current one is done.
        {
            MRF_input = (char) receive_MRF89XAM();
            append_string(MRF_message, MRF_input);
            if(strlen(MRF_message) == 8)
            {
                MRF_message_received = TRUE;
            }
            else
                MRF_message_received = FALSE;
        }
>>>>>>> Stashed changes
    }
    return;
}

<<<<<<< Updated upstream
=======
/******************************************************************************/
/*             Parse and respond to received message from MRF89XAM            */
/******************************************************************************/
char parse_MRF_message() 
{
    char message_id = MRF_message[0];
    char temp_string[9] = "";
    char response[9] = "";
    BYTE counter;

    //Ignore blank messages
    if (strcmp(MRF_message, "\n\n\n\n\n\n\n\n")) 
    {
        if (last_message_id == 0) 
        {
            //Parse longitude and latitude messages.
            switch (last_message_id) 
            {
                case 1:
                    waypoint1.longitude = atol(MRF_message);
                    last_message_id = 11;
                    break;
                case 11:
                    waypoint1.latitude = atol(MRF_message);
                    last_message_id = 0;
                    transmit_string_MRF89XAM("W1 Set");
                    break;
                case 2:
                    waypoint2.longitude = atol(MRF_message);
                    last_message_id = 22;
                    break;
                case 22:
                    waypoint2.latitude = atol(MRF_message);
                    last_message_id = 0;
                    transmit_string_MRF89XAM("W2 Set");
                    break;
                case 3:
                    waypoint3.longitude = atol(MRF_message);
                    last_message_id = 33;
                    break;
                case 33:
                    waypoint3.latitude = atol(MRF_message);
                    last_message_id = 0;
                    transmit_string_MRF89XAM("W3 Set");
                    break;
                case 4:
                    waypoint1.longitude = atol(MRF_message);
                    last_message_id = 11;
                    break;
                case 44:
                    waypoint1.latitude = atol(MRF_message);
                    last_message_id = 0;
                    transmit_string_MRF89XAM("Dest Set");
                    break;
            }
        } 
        else 
        {
            //Parse all other messages.
            switch (message_id) 
            {
                    //Report back temp, humid, and location.
                case 'R':
                    //Transmit temperature and humidity
                    strcpy       (response, "RT");
                    append_string(response, (char) temperature);
                    strcat       (response, "H");
                    append_string(response, (char) humidity);
                    transmit_string_MRF89XAM(response);

                    //Transmit longitude and latitude
                    ltoa( (long) (current_coords.longitude * 100000), temp_string);
                    transmit_string_MRF89XAM(temp_string); //Transmit as 8 digit integer.
                    ltoa( (long) (current_coords.latitude * 100000), temp_string);
                    transmit_string_MRF89XAM(temp_string); //Transmit as 8 digit integer.
                    break;

                    //Set home to current location.
                case 'H':
                    home = current_coords;
                    break;

                    //Start navigating back to home address.
                case 'C':
                    destination = home;
                    break;

                    //Set Waypoint 1.
                case '1':
                    last_message_id = 1;
                    break;

                    //Set Waypoint 2.
                case '2':
                    last_message_id = 2;
                    break;

                    //Set Waypoint 3.
                case '3':
                    last_message_id = 3;
                    break;

                    //Set final destination address.
                case 'D':
                    last_message_id = 4;
                    break;

                    //Start movement.
                case 'S':
                    start = TRUE;
                    break;

                    //Stop movement.
                case 'X':
                    start = FALSE;
                    break;

                    //Set robot to manual mode.
                case 'M':
                    mode = MANUAL;
                    break;

                    //Set robot to autonomous mode.
                case 'A':
                    mode = AUTO;
                    break;

                    //Read values from joysticks
                case 'J':
                    for(counter = 1; counter < 8; counter++)
                    {
                        //Read left joystick value.
                        if(counter <= 3 || counter == 5)
                            append_string(temp_string, MRF_message[counter]);
                        else if(counter == 4)
                        {
                            left_joystick = atoi(temp_string);
                            strcpy(temp_string, "");
                            append_string(temp_string, MRF_message[counter]);
                        }
                        else if(counter == 6)
                        {
                            append_string(temp_string, MRF_message[counter]);
                            right_joystick = atoi(temp_string);
                        }
                    }
                    break;

                    //Set to zero to indicate bad message.
                default:
                    message_id = 0x00;
                    transmit_string_MRF89XAM("UNKNOWN");
                    break;
            }
        }
    }
    return message_id;
}
>>>>>>> Stashed changes
