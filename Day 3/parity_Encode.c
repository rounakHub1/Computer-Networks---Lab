#include <stdio.h>
#include <string.h>

void main() {

    int count1=0, ln;
    char bitStream[50];

    printf("\nEnter the bit stream : ");
    scanf("%[^\n]s", &bitStream);
    ln=strlen(bitStream);

    for(int i=0; i<ln; i++) {
        if(bitStream[i]=='1')
            count1++;
    }

    //For even parity case,
    char tempStream1[ln];
    strcpy(tempStream1, bitStream);
    switch(count1%2) {
        case 0 :
            strcat(tempStream1, "0");
            break;
        case 1 :
            strcat(tempStream1, "1");
            break;
    }
    printf("\nFor even parity,\n\tencoded stream is : %s\n", tempStream1);

    //For odd parity case,
    char tempStream2[ln];
    strcpy(tempStream2, bitStream);
    switch(count1%2) {
        case 0 :
            strcat(tempStream2, "1");
            break;
        case 1 :
            strcat(tempStream2, "0");
            break;
    }
    printf("\nFor odd parity,\n\tencoded stream is : %s\n", tempStream2);
}