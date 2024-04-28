
#ifndef GPS_H
#define	GPS_H

/******************************************************************************/
/*                            Headers & Libraries                             */
/******************************************************************************/
#include "BeeBot_Globals.h"
#include "MCP2221A.h"

void init_uart1               (void);                //Initialize UART for HC05
BYTE read_byte_gps            (void);                //Read a byte from UART1 receive buffer.
void parse_gps_data(char* latitude, char* longitude, char* time, char* speed, char* date, char* longitude_dir, char* latitude_dir);

char gps_data[500]     = "";
char latitude_str[20]      = "";
char longitude_str[20]     = "";
char time_str[20]          = "";
char speed_str[20]         = ""; 
char date_str[20]          = ""; 
char latitude_dir_str[20]  = "";
char longitude_dir_str[20] = "";


#endif	/* GPS_H */

