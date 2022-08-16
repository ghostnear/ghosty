#include "math.h"
#include "string.h"

// Int to ASCII.
// Guaranteed to be unsafe in case the str pointer isn't set up properly.
void itoa(i32 n, i8* str)
{
    // Save the sign if needed
    bool sign = (n < 0);
    if(sign) n = -n;

    // Convert the positive number
    i32 i = 0;
    for( ; (n /= 10) > 0; i++)
        str[i] = '0' + n % 10;

    // Add sign if needed
    if(sign) str[i++] = '-';
    str[i] = '\0';

    // Reverse the result to be in the normal order
    strrev(str);
}

// Appends a character at the end of a string.
void strapp(i8* str, i8 ch)
{
    i32 n = strlen(str);
    str[n] = ch;
    str[n + 1] = '\0';
}

// Reverse a null-terminated string.
void strrev(i8* str)
{
    i32 n = strlen(str);
    i8 aux;
    for(int i = 0; 2 * i < n; i++)
    {
        // TODO: make a swap function or smth
        aux = str[i];
        str[i] = str[n - i];
        str[n - i] = aux;
    }
}

// Truncates a string by one character.
// Wrapper over strntrn
void strtrn(i8* str)
{
    strntrn(str, 1);
}


// Truncate a string by n characters.
// TODO: potentially unsafe: bounds checking
void strntrn(i8* str, i32 n)
{
    str[strlen(str) - n] = '\0';
}

// Compare two strings
i32 strcmp(i8* a, i8* b)
{
    i32 i = 0;
    for(; a[i] && b[i] && a[i] == b[i]; i++);
    return sgn(a[i] - b[i]);
}

// Get the size of a string.
// TODO: potentially unsafe: no-null termination.
i32 strlen(i8* str)
{
    i32 r = 0;
    while(str[r++]);
    return r;
}
