#include <stdio.h>
#include <string.h>

void main() {
    
    int count1=0, ln, lnc;
    char codeStream[50], choice[5];

    printf("\nEnter the codeword stream : ");
    scanf("%[^\n]s", &codeStream);
    ln=strlen(codeStream);
    char dataStream[50];

    for(int i=0; i<ln; i++) {
        if(codeStream[i]=='1')
            count1++;
    }

    printf("\nEnter your choice of parity : ");
    fflush(stdin);
    scanf("%[^\n]s", &choice);
    lnc=strlen(choice);

    fflush(stdin);
    switch(lnc) {
        case 4 :
            if(count1%2==0) {
                printf("\nAccepted...\n");
                strcpy(dataStream, codeStream);
                dataStream[ln-1]='\0';
                printf("For even parity,\n\toriginal stream is : %s\n", dataStream);
            }
            else
                printf("\nRejected ! wrong codeword received...\n");
            break;
        case 3 :
            if(count1%2==1) {
                printf("\nAccepted...\n");
                strcpy(dataStream, codeStream);
                dataStream[ln-1]='\0';
                printf("For odd parity,\n\toriginal stream is : %s\n", dataStream);
            }
            else
                printf("\nRejected ! wrong codeword received...\n");
            break;
        default :
            printf("\nWrong choice received !\n");
    }
}