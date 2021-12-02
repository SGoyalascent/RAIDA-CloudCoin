//C program to convert Decimal to Hexadecimal

#include <stdio.h>

int main() {
   
   
    unsigned char buf[] = {116,131,27,188,155,90,121,30,246,108,181,251,170,91,4,123,206,62,66,27,192,181};
    int n = sizeof buf << 1;
    char hexstr[n + 1];
    const char xx[]= "0123456789ABCDEF";
    
    char* xp = hexstr;
    const char *bb = buf;
    while (--n >= 0) {
        xp[n] = xx[(bb[n>>1] >> ((1 - (n&1)) << 2)) & 0xF]; }
    hexstr[n] = 0; 
    printf("%s\n", hexstr);

    /*
    unsigned char buf[] = {0, 1, 10, 11};
    char hex_str[9];
    for (int i = 0; i < 4; i++)
    {
        //printf("%02X", buf[i]);
        sprintf(hex_str, "%02x", buf[i]);
        printf("%s", hex_str);
    }
    printf("\n");
    //printf("%s\n", hex_str);
    
    
    
    */



   int i;
char* buf2 = stringbuf;
char* endofbuf = stringbuf + sizeof(stringbuf);
for (i = 0; i < x; i++)
{
    /* i use 5 here since we are going to add at most 
       3 chars, need a space for the end '\n' and need
       a null terminator */
    if (buf2 + 5 < endofbuf)
    {
        if (i > 0)
        {
            buf2 += sprintf(buf2, ":");
        }
        buf2 += sprintf(buf2, "%02X", buf[i]);
    }
}
buf2 += sprintf(buf2, "\n");
   
   
   
    /*
    int decimal_num = 20000;
    int quotient, remainder;
    char hex_num[100];

    quotient = decimal_num;
    int i = 0;
    while(quotient != 0) {
        remainder = quotient % 16;
        if(remainder < 10) {
            hex_num[i++] = 48 + remainder;
        }
        else  {
            hex_num[i++] = 55 + remainder;
        }
        quotient = quotient/16;
    }

    for(int j = i; j >=0; j++) {
        printf("%c",hex_num[j]);
    } */

}

/*
    unsigned char buffer[22] = {116,131,27,188,155,90,121,30,246,108,181,251,170,91,4,123,206,62,66,27,192,181};
    unsigned char ticket[45];  //74831BBC9B5A791EF66CB5FBAA5B047BCE3E421BC0B5
    int k = 0;
    for(int i =0 ; i< 22; i++) {
        
        int num = buffer[i];
        char hex[]= {0,0}; 
    
        int quotient, remainder;
        quotient = num/16;
    
        if (quotient<10) {
            hex[0] = 48 + quotient;
        }
        else {
            hex[0] = 55 + quotient;
        }
    
        remainder = num%16;
    
        if (remainder<10) {
            hex[1] = 48 + remainder;
        }
        else {
            hex[1] = 55 + remainder;
        }
        
        for(int j = 0; j < 2; j++) {
            ticket[k++] = hex[j];
        }
    }
    ticket[44] = '\0';
    printf("%s", ticket);

*/

/*

    unsigned char buf[] = {116,131,27,188,155,90,121,30,246,108,181,251,170,91,4,123,206,62,66,27,192,181};
    int n = sizeof buf << 1;
    char hexstr[n + 1];
    const char xx[]= "0123456789ABCDEF";
    
    char* xp = hexstr;
    const char *bb = buf;
    while (--n >= 0) {
        xp[n] = xx[(bb[n>>1] >> ((1 - (n&1)) << 2)) & 0xF]; }
    hexstr[n] = 0; 
    printf("%s\n", hexstr);
*/