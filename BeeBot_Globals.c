/*
 * BeeBot_Globals.h
 * ----------
 * C source file for global functions needed for the BeeBot project. Only place 
 * function definitions in this header if they are needed across multiple 
 * different files of the project. 
 */

#include "BeeBot_Globals.h"

/******************************************************************************/
/*               Append a character upon the end of a string                  */
/******************************************************************************/
void append_string(char *str, char ch)
{
    unsigned int length = strlen(str);
    if(length < MSG_MAX - 1)
    {
        str[length]     = ch;
        str[length + 1] = '\0';
    }
    else
        strcpy(str, "");
    return;
}
