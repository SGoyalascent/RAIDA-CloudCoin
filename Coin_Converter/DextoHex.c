//C program to convert Decimal to Hexadecimal

#include <stdio.h>

int main() {

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
    }


}