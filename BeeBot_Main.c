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
#include "GPS.h"

/******************************************************************************/
/*                                 States                                     */
/******************************************************************************/

#define INIT 2       //System startup and initialization.
#define WAITING 3    //No movement, waiting on commands from user.
#define DEBUG 4      //Debug mode while USB is plugged in.
#define DEPLOYED 5    //Deployment mode while in the field.
#define LOW_BATTERY 200

#define RIGHT_MOTOR_1 LATDbits.LD1 
#define RIGHT_MOTOR_2 LATAbits.LA4 
#define LEFT_MOTOR_1  LATEbits.LE0 
#define LEFT_MOTOR_2  LATAbits.LA3 
#define FORWARDS    0
#define BACKWARDS   1
#define STOPPED     2
#define LEFT_MOTOR  0
#define RIGHT_MOTOR 1
#define STROBE      LATAbits.LA0

/******************************************************************************/
/*                            Global Constants                                */
/******************************************************************************/

#define UART_TERMINATOR '\n' //Terminator character for UART.
#define MAX_REPORTS 24       //Number of reports to save.


/******************************************************************************/
/*                            Global Variables                                */
/******************************************************************************/
unsigned int timer_tick = 0;                        //Increments per every Timer0 interrupt.
volatile int* adc_out   = (volatile int *) 0xFC4;   //Reference to ADC output.
BYTE warning_sent       = FALSE;                    //Indicates if battery warning has been sent.
BYTE coord_mode         = 0;                        //Coordinates to edit.
unsigned int gps_counter = 0;                       //
unsigned long th_timer_tick = 0;
unsigned long j_timer_tick = 0;
unsigned long last_joystick_tick = 0;
unsigned long hourly_timer_tick = 0;
char reports[60][9];

/******************************************************************************/
/*                          Function Declarations                             */
/******************************************************************************/
void init_pins          (void);         //Initialize all pins to initial values.
void init_interrupts    (void);         //Initialize all program interrupts.
void parse_MRF_message  (void);
BYTE get_battery_level  (void);
void init_adc           (void);
long extractHexadecimalValue(const char *str);
float adc_to_actual     (BYTE adc_value);
float calculate_battery_percentage(float voltage);
void update_coords      (void);
void get_temperature_humidity(void);
void send_hourly_report (void);
void stop_motor         (BYTE motor);
void move_motor         (BYTE motor, BYTE dir);
void minutes_to_degrees(COORDS* coordinates);

/******************************************************************************/
/*                            Inline Functions                                */
/******************************************************************************/
void set_strobe(void) {LATAbits.LA0 = !LATAbits.LA0;}
void appendHexToBuffer(long value, char *buffer) { sprintf(buffer + strlen(buffer), "%lx", value);}

/******************************************************************************/
/*                               Main Function                                */
/******************************************************************************/
void main(void) 
{
    char temp_string[40] = "";
    
    //Clock setup
    OSCCON = 0b01100111;
    __delay_ms(200);
    
    //Place all initialization code here.
    init_pins      ();
    init_uart1     ();
    init_uart2     ();
    init_spi       ();
    init_adc       ();
    init_MRF89XAM  ();
    init_interrupts();
    stop_motor(LEFT_MOTOR);
    stop_motor(RIGHT_MOTOR);
    mode = AUTO;
    
    while(TRUE)
    {
        //Process messages
        if(MRF_message_received)
        {
            __delay_ms(25);                
            parse_MRF_message();
            MRF_message_received = FALSE;
            strcpy(MRF_message, "");
        }
        else if(usb_message_received)
        {
            __delay_ms(10);
            parse_usb_message(usb_message);
            usb_message_received = FALSE;
            strcpy(usb_message, "");
        }
        
        //Movement and deployment.
        if(mode == AUTO)
        {
            //Autonomous navigation here.
           stop_motor(LEFT_MOTOR);
           stop_motor(RIGHT_MOTOR);
           STROBE = 0;
        }
        
        //Manual movement code
        else if(mode == MANUAL)
        {
            //Turn off motors if over 1s since last signal.
            if(j_timer_tick - last_joystick_tick > 1000)
            {
                left_joystick = right_joystick = 1;
            }
            
            
            //Left joystick controls forwards, backwards movement
            if(left_joystick > 200)                     //Forward
            {
                STROBE = 1;
                move_motor(RIGHT_MOTOR, FORWARDS);
                //stop_motor(LEFT_MOTOR);
                //__delay_ms(3);                          //Compensation for pull
                move_motor(LEFT_MOTOR, FORWARDS);
                transmit_string_usb("FORWARDS\n");
            }
            else if(left_joystick == 0)                 //Backwards
            {    
                if(right_joystick <= 1 || right_joystick >= 200)
                {
                    STROBE = 1;
                    move_motor(LEFT_MOTOR, BACKWARDS);
                    move_motor(RIGHT_MOTOR, BACKWARDS);
                    transmit_string_usb("BACKWARDS\n");
                }
            }
            else                                        //Stop
            {
                STROBE = 0;
                stop_motor(LEFT_MOTOR);
                stop_motor(RIGHT_MOTOR);
                transmit_string_usb("STOPPING\n");
            }
            
            
            if(right_joystick > 200)                    //Left Turn
            {
                if(left_joystick > 200)                 
                {
                    stop_motor(LEFT_MOTOR);             
                    __delay_ms(60);
                    move_motor(LEFT_MOTOR, FORWARD);
                }
                else if(left_joystick == 0)             
                {
                    stop_motor(RIGHT_MOTOR);         
                    __delay_ms(60);
                    move_motor(RIGHT_MOTOR, BACKWARDS);
                }
                else                                    //CCW Turn
                {
                    STROBE = 1;
                    move_motor(RIGHT_MOTOR, FORWARDS);
                    move_motor(LEFT_MOTOR, BACKWARDS);
                }
                transmit_string_usb("RIGHT TURN\n\n");
            }
            else if(right_joystick == 0)                //Right Turn
            {
                if(left_joystick <= 1 || left_joystick >= 200)
                {
                    if(left_joystick > 200)
                    {
                        stop_motor(RIGHT_MOTOR);        
                        __delay_ms(60);
                        move_motor(RIGHT_MOTOR, FORWARD);
                    }
                    else if(left_joystick == 0)
                    {
                        stop_motor(LEFT_MOTOR);         
                        __delay_ms(60);
                        move_motor(LEFT_MOTOR, BACKWARDS);
                    }
                    else                                //CW Turn
                    {
                        STROBE = 1;
                        move_motor(RIGHT_MOTOR, BACKWARDS);
                        move_motor(LEFT_MOTOR, FORWARDS);
                    }
                    transmit_string_usb("LEFT TURN\n\n");
                }
            }
        }
        else if(!start)
        {
            //Stopped and deployment code here.
        }
        
        //Check or update certain parameters every 5s
        if(th_timer_tick >= 5000)
        {
            th_timer_tick = 0;
            
            //Update temperature and humidity readings.
            get_temperature_humidity();
            
            //Check if battery level is low.
            battery_voltage = adc_to_actual(get_battery_level());               //Get the current battery voltage.
            battery_percentage = calculate_battery_percentage(battery_voltage); //Get the current battery percentage.
            if(battery_voltage <= 9.3)
            {
                if(!warning_sent)
                {
                    transmit_string_usb("LOW BATTERY");
                    transmit_string_MRF89XAM("LOWBATT");
                }
            }
            
            //Update time and date info
            strncpy(time_utc, time_str, 6);
            strncpy(date_utc, date_str, 6);
            
            //Send a report every hour
            if(hourly_timer_tick >= 3100000)
            {
                hourly_timer_tick = 0;
                hour_counter++;
                send_hourly_report();
            }
        }
    }
    return;
}

/******************************************************************************/
/*                            Initialize all pins                             */
/******************************************************************************/
void init_pins(void)
{
    //USB Code
    TRISDbits.RD6 = 1;   //Set USB_STAT as input.
    TRISDbits.RD6 = 1;   //Set TX as input.
    TRISDbits.RD7 = 1;   //Set RX as input.
   
    //MRF89XAM Code
    TRISBbits.RB1 = 1;  //MRF IRQ0
    TRISBbits.RB2 = 1;  //MRF IRQ1
    TRISCbits.RC0 = 0;  //MRF CSCON
    TRISCbits.RC1 = 0;  //MRF CSDATA
    TRISCbits.RC2 = 1;  //MRF Reset
    TRISCbits.RC3 = 0;  //MRF SCK1
    TRISCbits.RC4 = 1;  //MRF SDI1 (MISO)
    TRISCbits.RC5 = 0;  //MRF SDO1 (MOSI)
    LATCbits.LC0  = 1;  //CSCON is default high
    LATCbits.LC1  = 1;  //CSDATA is default high
    
    //Strobe Light
    TRISAbits.RA0 = 0;
    LATAbits.LA0  = 0;
    
    //Battery Level Indicator
    TRISDbits.RD2 = 1;
    
    //Motors & Ultrasonic Sensors
    TRISAbits.RA3 = 0;  //Motor 1 direction
    TRISAbits.RA4 = 0;  //Motor 2 direction
    TRISAbits.RA5 = 0;  //Mux S0
    TRISAbits.RA6 = 0;  //Mux S1
    TRISAbits.RA7 = 0;  //TRIG
    TRISBbits.RB4 = 1;  //ECHO
    //TRISDbits.RD1 = 1;  //PWM Motor 1
    //TRISEbits.RE0 = 1;  //PWM Motor 2
    TRISDbits.RD1 = 0;  //PWM Motor 1
    TRISEbits.RE0 = 0;  //PWM Motor 2
    LATAbits.LA3  = 0;
    LATAbits.LA4  = 0;
    LATAbits.LA5  = 0;
    LATAbits.LA6  = 0;
    LATAbits.LA7  = 0;
    
    //GPS
    TRISCbits.RC6 = 1;  //GPS TX
    TRISAbits.RA2 = 0;  //GPS EN
    LATAbits.LA2  = 0;
    
    //No analog, all digital
    ANSELA = 0x00;
    ANSELB = 0x00;
    ANSELC = 0x00;
    ANSELD = 0x04;  //Except for battery level indicator.
    ANSELE = 0x00;
    return;
}

/******************************************************************************/
/*                             Initializing ADC                               */
/******************************************************************************/
void init_adc(void)
{
    ADCON2bits.ACQT  = 2;       //Set acquisition time to 2 cycles.
    ADCON2bits.ADFM  = 0;       //Right justify ADC output.
    ADCON2bits.ADCS  = 5;       //Set ADC clock source as Fosc/16 (Table 17-1).
    ADCON1bits.PVCFG = 0;       //ADC positive reference set to VDD.
    ADCON1bits.NVCFG = 0;       //ADC negative reference set to VSS.
    return;
}

/******************************************************************************/
/*                   Read analog values from the joysticks                    */
/******************************************************************************/
BYTE get_battery_level(void)
{
    BYTE result;
    
    //Turn on ADC
    PIR1bits.ADIF   = 0;      //Clearing ADC interrupt flags
    ADCON0bits.CHS  = 22;     //Connect to analog channel 22   
    ADCON0bits.ADON = 1;      //Turn on ADC
    
    //Get ADC reading
    ADCON0bits.GODONE = 1;              //Start sampling process
    while(PIR1bits.ADIF != 1) {}        //Wait for sampling to end
    PIR1bits.ADIF = 0;                  //Clear ADC flag    
    result = (BYTE) *adc_out;           //Read ADC value
   
    ADCON0bits.ADON = 0;                //Turn off ADC
    return result;
}

/******************************************************************************/
/*                       Initialize program interrupts                        */
/******************************************************************************/
void init_interrupts(void)
{
    RCONbits.IPEN    = 0;   //Disable priority levels for interrupts.
    INTCONbits.GIE   = 0;   //Disable all interrupts.
    
    //UART1 Interrupts
    PIE1bits.RC1IE   = 1;   //Enable UART1 receive interrupt.
    
    //UART2 Interrupts
    PIE3bits.RC2IE   = 1;   //Enable UART1 receive interrupt.
    
    //INT1 interrupts (used for IRQ0 on MRF89XAM)
    INTCON3bits.INT1IE = 1;
    INTCON2bits.INTEDG1 = 1;  //Set INT1 interrupts only on rising edge
    
    //Timer0 setup
    T0CON    	      = 0x08;     //Configure to 16bit timer, no prescaler.
    TMR0H             = 0xF8;     //Set preload to 63535.
    TMR0L    	      = 0x2F;
    INTCONbits.TMR0IE = 1;        //Enable Timer0 interrupts.
    
    //Turn interrupts on
    INTCONbits.PEIE   = 1;   //Enable all peripheral interrupts.
    INTCONbits.GIE    = 1;   //Enable all interrupts.
    T0CONbits.TMR0ON  = 1;   //Turn on Timer0.
    return;
}

/******************************************************************************/
/*               Interrupt service routine for all interrupts                 */
/******************************************************************************/
void __interrupt() ISR(void)
{
    char MRF_input = 0;
    char gps_input = 0;
    char temp_receive_string[MSG_MAX] = "";
    
    //UART1 receive interrupt (GPS))
    if(PIR1bits.RC1IF && PIE1bits.RC1IE)
    {
        gps_input = read_byte_gps();
        if(gps_counter < 300)       //Sample 300 characters of GPS
        {
            gps_data[gps_counter] = gps_input;
            gps_counter++;
        }
        else if(gps_counter < 500)  //Induce a short delay
        {
            gps_counter++;
        }
        else                        //Parse GPS data
        {
            PIE1bits.RC1IE = 0;
            parse_gps_data(latitude_str, longitude_str, time_str, speed_str, date_str, latitude_dir_str, longitude_dir_str);
            gps_counter = 0;
            update_coords();
            PIE1bits.RC1IE = 1;
        }
    }

    //UART2 Interrupts (USB))
    char usb_input = 0;
    if(PIR3bits.RC2IF && PIE3bits.RC2IE)
    {
        usb_input = read_byte_usb();
        if(usb_input == '\n')
            usb_message_received = TRUE;
        else
            append_string(usb_message, usb_input);
    }
    
    if(TMR0IE && TMR0IF) //Increment time if Timer0 is enabled and Timer0 has overflowed.
    {
        TMR0IF = 0;       //Reset Timer0 overflow flag.
        timer_tick++;     //Increment time.
        TMR0H  = 0xF8;    //Reset 16bit timer preload to 63535
        TMR0L  = 0x2F;	
        th_timer_tick++;        //Tick for temperature/humidity sensor.
        j_timer_tick++;         //Tick for joystick signal timer.
        hourly_timer_tick++;    //Tick for hourly reporting.
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
                MRF_message_received = TRUE;
            else
                MRF_message_received = FALSE;
        }
    }
    return;
}

/******************************************************************************/
/*             Parse and respond to received message from MRF89XAM            */
/******************************************************************************/
void parse_MRF_message(void) 
{
    char message_id = MRF_message[0];
    char temp_string[40] = "";
    char response[9] = "";
    BYTE counter;

    //Ignore blank messages
    if (strcmp(MRF_message, "\n\n\n\n\n\n\n\n")) 
    {
        //TEST CASE ONLY
        if (!strncmp(MRF_message, "hello", 5)) 
        {
            transmit_string_MRF89XAM("HI!");
        }
        switch (message_id) 
        {
                //Report back temp, humid, and location.
            case 'R':                
                //Transmit temperature, humidity, and battery
                strcpy(response, "RT");
                append_string(response, (char) temperature);
                strcat(response, "H");
                append_string(response, (char) humidity);
                strcat(response, "B");
                append_string(response, (char) battery_percentage);
                append_string(response, (char) hour_counter+20);
                transmit_string_usb(response);
                transmit_string_MRF89XAM(response);
                strcpy(reports[hour_counter], response);

                //Transmit longitude and latitude
                __delay_ms(100);
                strcpy(response, "O");
                appendHexToBuffer(current_coords.longitude, response);
                transmit_string_MRF89XAM(response); //Transmit as 8 digit integer.
                __delay_ms(100);
                strcpy(response, "A");
                appendHexToBuffer(current_coords.latitude, response);
                transmit_string_MRF89XAM(response); //Transmit as 8 digit integer.
                break;

                //Report back temperature, humidity, and battery from last 2 days.
            case 'L':
                for(counter = 0; counter <= hour_counter; counter++)
                {
                    transmit_string_MRF89XAM(reports[counter]);
                    __delay_ms(100);
                }
                break;
                
                //Set home to current location.
            case 'H':
                home = current_coords;
                 __delay_ms(1500);
                strcpy(response, "O");
                appendHexToBuffer(home.longitude, response);
                transmit_string_MRF89XAM(response); //Transmit as 8 digit integer.
                __delay_ms(100);
                strcpy(response, "A");
                appendHexToBuffer(home.latitude, response);
                transmit_string_MRF89XAM(response); //Transmit as 8 digit integer.
                break;

                //Start navigating back to home address.
            case 'C':
                waypoints[3] = home;
                //Transmit longitude and latitude
                __delay_ms(1500);
                strcpy(response, "O");
                appendHexToBuffer(waypoints[3].longitude, response);
                transmit_string_MRF89XAM(response); //Transmit as 8 digit integer.
                __delay_ms(100);
                strcpy(response, "A");
                appendHexToBuffer(waypoints[3].latitude, response);
                transmit_string_MRF89XAM(response); //Transmit as 8 digit integer.
                break;

                //Set Waypoint 1.
            case '1':
                transmit_string_usb("Setting Waypoint 1");
                coord_mode = 1;
                break;
                //Set Waypoint 2.
            case '2':
                transmit_string_usb("Setting Waypoint 2");
                coord_mode = 2;
                break;

                //Set Waypoint 3.
            case '3':
                transmit_string_usb("Setting Waypoint 3");
                coord_mode = 3;
                break;

                //Set final destination address.
            case 'D':
                transmit_string_usb("Setting Destination");
                coord_mode = 4;
                break;

            case 'O':
                if (coord_mode > 0 && coord_mode < 5)
                {
                    waypoints[coord_mode - 1].latitude = extractHexadecimalValue(MRF_message);
                    ltoa(waypoints[coord_mode - 1].latitude, temp_string);
                    transmit_string_usb("latitude: ");
                    transmit_string_usb(temp_string);
                }
                break;

            case 'A':
                if (coord_mode > 0 && coord_mode < 5)
                {
                    waypoints[coord_mode - 1].longitude = extractHexadecimalValue(MRF_message);
                    ltoa(waypoints[coord_mode - 1].longitude, temp_string);
                    transmit_string_usb("longitude: ");
                    transmit_string_usb(temp_string);
                }
                coord_mode = 0;
                break;

                //Start movement.
            case 'S':
                start = TRUE;
                transmit_string_usb("\n*****STARTING*****");
                break;

                //Stop movement.
            case 'X':
                start = FALSE;
                transmit_string_usb("\n*****STOPPING*****");
                break;

                //Set robot to manual mode.
            case 'M':
                mode = MANUAL;
                last_joystick_tick = j_timer_tick = 0;
                transmit_string_usb("\n*****MANUAL MODE SET*****");
                break;

                //Set robot to autonomous mode.
            case 'U':
                mode = AUTO;
                transmit_string_usb("\n*****AUTO MODE SET*****");
                break;

                //Read values from joysticks
            case 'J':
                last_joystick_tick = j_timer_tick;
                for (counter = 1; counter < 8; counter++) 
                {
                    //Read left joystick value.
                    if (counter < 4)
                        append_string(temp_string, MRF_message[counter]);
                    else if (counter == 4) 
                    {
                        //transmit_string_usb(temp_string);
                        left_joystick = (BYTE) atoi(temp_string);
                        strcpy(temp_string, "");
                        append_string(temp_string, MRF_message[counter]);
                    } 
                    else if(counter == 5)
                        append_string(temp_string, MRF_message[counter]);
                    else if (counter == 6) 
                    {
                        append_string(temp_string, MRF_message[counter]);
                        right_joystick = (BYTE) atoi(temp_string);
                        //transmit_string_usb(temp_string);
                        sprintf(temp_string, "Left Joy = %d, Right Joy = %d", left_joystick, right_joystick);
                        transmit_string_usb(temp_string);
                    }
                }
                break;
            default:
                message_id = 0x00;
                //transmit_string_MRF89XAM("UNKNOWN");
                break;
        }
    }
    return;
}

/******************************************************************************/
/*             Convert hexadecimal value in string to long value              */
/******************************************************************************/
long extractHexadecimalValue(const char *str)
{
    str++;
    long hexValue = strtol(str, NULL, 16); 
    return hexValue;
}

/******************************************************************************/
/*               Convert ADC value to equivalent battery voltage              */
/******************************************************************************/
float adc_to_actual(BYTE adc_value)
{
    float voltage = 0;
    float ref_voltage = 3.3f;
    
    voltage = ((float) adc_value) * (ref_voltage/256.0f);   //Convert to divider's output voltage.
    voltage *= 4.0f;                                         //Scale to battery voltage
    return voltage;
}

/******************************************************************************/
/*                    Determine battery level percentage                      */
/******************************************************************************/
float calculate_battery_percentage(float voltage) 
{
    // Define the voltage range and percentage range
    float minVoltage    = 9.0;
    float maxVoltage    = 12.6;
    float minPercentage = 0.0;
    float maxPercentage = 100.0;

    // Calculate the percentage using linear interpolation
    float percentage = minPercentage + ((voltage - minVoltage) / (maxVoltage - minVoltage)) * (maxPercentage - minPercentage);

    // Ensure the percentage is within the valid range
    if (percentage < minPercentage)
        percentage = minPercentage;
    else if (percentage > maxPercentage) 
        percentage = maxPercentage;
    
    return percentage;
}



/******************************************************************************/
/*                   Get temperature and humidity from DHT22                  */
/******************************************************************************/
void get_temperature_humidity(void)
{
    BYTE counter;
    BYTE th_reading[40] = {0};
    
    //Disable interrupts during processing.
    INTCONbits.GIE = 0;
    
    //Pull data line low for 1 ms.
    TRISAbits.RA1 = 0;
    LATAbits.LA1 = 0;
    __delay_ms(1);
    LATAbits.LA1 = 1;
    
    //After short delay, module should respond by pulling data line low.
    __delay_us(45);
    TRISAbits.RA1 = 1;
    if(PORTAbits.RA1 == 0)
    {
        //Wait for DHT22 to return the signal high (~80us)
        while(PORTAbits.RA1 == 0);
        //Wait for DHT22 to return the signal low (~80us)
        while(PORTAbits.RA1 == 1);
        
        //DHT22 will now start sending data (16 bits humidity, 16 bits temperature, 8 bits parity)
        //Each message is proceeded by 50us LOW signal
        //If HIGH for proceeding 70us, data == 1
        //If HIGH for only 28us, data == 0
        for(counter = 0; counter < 40; counter++)
        {
            //Wait for proceeding low to end
            while(PORTAbits.RA1 == 0);
            
            //Parse length by delaying 40us and checking if high or low.
            __delay_us(40);
            th_reading[counter] = PORTAbits.RA1;
            
            //If still high, delay until low.
            while(PORTAbits.RA1 == 1);
        }
    }
    
    //Enable interrupts, since time-sensitive processing is now over.
    INTCONbits.GIE = 1;
    
    //Process read string into actual temperature and humidity values
    //Humidity = Byte 0
    //Temperature = Byte 2
   
    temperature = humidity = 0x00;
    for(counter = 0; counter < 16; counter++)
    {
        if(th_reading[counter] == 1)
            humidity = humidity | (0x01 << (15-counter));
        if(th_reading[counter + 16] == 1)
            temperature = temperature | (0x01 << (15-counter));
    }
    
    //Scale down both values by 10 to get actual values
    temperature /= 10;
    humidity    /= 10;
    
    //Convert temperature from Celsius to Fahrenheit
    temperature = (int) ((float) temperature * 9.0f/5.0f + 32.0f);
    
    return;
}

void send_hourly_report(void) 
{
    //Send wireless report.
    strcpy(MRF_message, "R");
    parse_MRF_message();
    strcpy(MRF_message, "");
    
    
    //Send wired report.
    strcpy(usb_message, "REPORT");
    parse_usb_message(usb_message);
    strcpy(usb_message, "");
    return;
}

void move_motor(BYTE motor, BYTE dir)
{
    if(motor == RIGHT_MOTOR)
    {
        if(dir == FORWARDS)
        {
            RIGHT_MOTOR_1 = 0;
            RIGHT_MOTOR_2 = 1;
        }
        else
        {
            RIGHT_MOTOR_1 = 1;
            RIGHT_MOTOR_2 = 0;
        }
    }
    else if(motor == LEFT_MOTOR)
    {
        if(dir == FORWARDS)
        {
            LEFT_MOTOR_1 = 0;
            LEFT_MOTOR_2 = 1;
        }
        else
        {
            LEFT_MOTOR_1 = 1;
            LEFT_MOTOR_2 = 0;
        }
    }
    return;
}

void stop_motor(BYTE motor)
{
    if(motor == RIGHT_MOTOR)
        RIGHT_MOTOR_1 = RIGHT_MOTOR_2 = 0;
    else if(motor == LEFT_MOTOR)
        LEFT_MOTOR_1 = LEFT_MOTOR_2 = 0;
    return;
}

/******************************************************************************/
/*             Update current coords with most recent GPS reading             */
/******************************************************************************/
void update_coords(void)
{
    float temp_f_lat  = 0.0f;
    float temp_f_long = 0.0f;
    
    temp_f_lat  = (float) atof(latitude_str)  * 1000.0f;
    temp_f_long = (float) atof(longitude_str) * 1000.0f;
    
    current_coords.latitude  = (long) temp_f_lat;
    current_coords.longitude = (long) temp_f_long;
   
    minutes_to_degrees(&current_coords);
    
    return;
}

void minutes_to_degrees(COORDS* coordinates)
{
    long whole_long;
    long whole_lat;
    long dec_long;
    long dec_lat;   
    
    //Derive whole number portion
    whole_long = coordinates->longitude / 100000;
    whole_long *= 100000;
    whole_lat  = coordinates->latitude  / 100000;
    whole_lat  *= 100000;
    
    //Derive decimal number portion and scale by 100
    dec_long = (coordinates->longitude - whole_long) * 100;
    dec_lat  = (coordinates->latitude  - whole_lat)  * 100;
    
    //Scale decimal number portion by 1/60
    dec_long /= 60;
    dec_lat  /= 60;
    
    //Recombine whole and decimal portions
    coordinates->longitude = whole_long + dec_long;
    coordinates->latitude  = whole_lat  + dec_lat;
    return;
}