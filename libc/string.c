#include "math.h"
#include "string.h"

// Int to ASCII.
// Guaranteed to be unsafe in case the str pointer isn't set up properly.
void itoa(int32_t n, char* str)
{
    // Save the sign if needed
    bool sign = (n < 0);
    if(sign) n = -n;

    // Convert the positive number
    int32_t i = 0;
    for( ; (n /= 10) > 0; i++)
        str[i] = '0' + n % 10;

    // Add sign if needed
    if(sign) str[i++] = '-';
    str[i] = '\0';

    // Reverse the result to be in the normal order
    strrev(str);
}

// Appends a character at the end of a string.
void strapp(char* str, char ch)
{
    int32_t n = strlen(str);
    str[n] = ch;
    str[n + 1] = '\0';
}

// Reverse a null-terminated string.
void strrev(char* str)
{
    int32_t n = strlen(str);
    char aux;
    for(int32_t i = 0; 2 * i < n; i++)
    {
        // TODO: make a swap function or smth
        aux = str[i];
        str[i] = str[n - i];
        str[n - i] = aux;
    }
}

// Truncates a string by one character.
// Wrapper over strntrn
void strtrn(char* str)
{
    strntrn(str, 1);
}


// Truncate a string by n characters.
// TODO: potentially unsafe: bounds checking
void strntrn(char* str, int32_t n)
{
    str[strlen(str) - n] = '\0';
}

// Compare two strings
int32_t strcmp(char* a, char* b)
{
    int32_t i = 0;
    for(; a[i] && b[i] && a[i] == b[i]; i++);
    return sgn(a[i] - b[i]);
}

// Get the size of a string.
// TODO: potentially unsafe: no-null termination.
int32_t strlen(char* str)
{
    int32_t r = 0;
    while(str[r++]);
    return r;
}
