#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET sd, cd;
    struct sockaddr_in sad = {0}, cad = {0};
    int cadl = sizeof(cad);
    char buf[100];
    int count1, ln, lnc;

    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("\nWSAStartup failed\n");
        return 1;
    }

    sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd == INVALID_SOCKET) {
        printf("\nsocket failed: %d\n", WSAGetLastError());
        return 1;
    }

    sad.sin_family = AF_INET;
    sad.sin_port = htons(9995);
    sad.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(sd, (struct sockaddr*)&sad, sizeof(sad)) == SOCKET_ERROR) {
        printf("\nbind failed: %d\n", WSAGetLastError());
        return 1;
    }

    if (listen(sd, 10) == SOCKET_ERROR) {
        printf("\nlisten failed: %d\n", WSAGetLastError());
        return 1;
    }

    printf("\nWaiting for client...\n");

    cd = accept(sd, (struct sockaddr*)&cad, &cadl);
    if (cd == INVALID_SOCKET) {
        printf("\naccept failed: %d\n", WSAGetLastError());
        return 1;
    }

    printf("\nClient connected.\n");

    while (1) {
        int bytes = recv(cd, buf, sizeof(buf) - 1, 0);

        if (bytes == 0) {
            printf("\nClient disconnected.\n");
            break;
        }
        if (bytes == SOCKET_ERROR) {
            printf("\nrecv failed: %d\n", WSAGetLastError());
            break;
        }

        char *encoded = strtok(buf, "|");
        char *parity  = strtok(NULL, "|");

        ln=strlen(encoded);
        lnc=strlen(parity);
        count1=0;

        for(int i=0; i<ln; i++) {
            if(encoded[i]=='1')
                count1++;
        }

        switch(lnc) {
            case 4 :
                if(count1%2==0) {
                    printf("\nAccepted...\n");
                    printf("Encoded data (even parity): %s\n", encoded);
                    encoded[ln-1]='\0';
                    printf("For even parity,\n\tClient says : %s\n", encoded);
                }
                else
                    printf("\nRejected ! wrong codeword received...\n");
                break;
            case 3 :
                if(count1%2==1) {
                    printf("\nAccepted...\n");
                    printf("Encoded data (odd parity): %s\n", encoded);
                    encoded[ln-1]='\0';
                    printf("For even parity,\n\tClient says : %s\n", encoded);
                }
                else
                    printf("\nRejected ! wrong codeword received...\n");
                break;
            default :
                printf("\nWrong choice received !\n");
        }
    }

    closesocket(cd);
    closesocket(sd);
    WSACleanup();
    return 0;
}