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
    if(length < 500 - 1)
    {
        str[length]     = ch;
        str[length + 1] = '\0';
    }
    else
        strcpy(str, "");
    return;
}

/******************************************************************************/
/*                        Reverse the order of a string                       */
/******************************************************************************/
void reverse_string(char s[])
{
    int i, j;
    char c;
 
    for (i = 0, j = ((char)strlen(s))-1; i<j; i++, j--) 
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
    return;
}

/******************************************************************************/
/*                  Convert an integer to an equivalent string                */
/******************************************************************************/
void itoa(int n, char s[])
{
    int i, sign;
 
    if ((sign = n) < 0)  /* record sign */
        n = -n;          /* make n positive */
    i = 0;
    do 
    {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse_string(s);
    return;
}
 
/******************************************************************************/
/*                         Sort an array in numeric order                     */
/******************************************************************************/
/*void bubbleSort(BYTE arr[], BYTE n) 
{
    BYTE i, j, temp;
    for (i = 0; i < n-1; i++) {
        // Last i elements are already in place
        for (j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                // Swap arr[j] and arr[j+1]
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
    return;
}*/

/******************************************************************************/
/*                  Convert a double to an equivalent string                  */
/******************************************************************************/
void ftoa(float num, char *str, int decimalPlaces) 
{
    int wholePart = (int)num; // Extract whole part
    float decimalPart = num - wholePart; // Extract decimal part

    // Convert whole part to string using sprintf
    sprintf(str, "%d.", wholePart);

    // Extract digits from decimal part and add to string
    for (int i = 0; i < decimalPlaces; i++) {
        decimalPart *= 10;
        int digit = (int)decimalPart;
        str[i + (char) strlen(str)] =  '0' + (char) digit;
        decimalPart -= digit;
    }
    str[strlen(str)] = '\0'; // Null-terminate the string
    return;
}

/******************************************************************************/
/*        Convert a long integer to an equivalent string                       */
/******************************************************************************/
void ltoa(long n, char s[])
{
    long i, sign;
 
    if ((sign = n) < 0)  /* record sign */
        n = -n;          /* make n positive */
    i = 0;
    do 
    {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse_string(s);
}
