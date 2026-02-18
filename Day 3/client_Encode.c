#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <time.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET sd;
    struct sockaddr_in sad;

    char buf[100];
    char choice[10];
    char encoded[102];
    char packet[120];

    int mesg_num = 1;

    /* Initialize Winsock */
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    /* Create socket */
    sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd == INVALID_SOCKET) {
        printf("Socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    /* Server address setup */
    memset(&sad, 0, sizeof(sad));
    sad.sin_family = AF_INET;
    sad.sin_port = htons(9995);
    sad.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* Connect to server */
    if (connect(sd, (struct sockaddr*)&sad, sizeof(sad)) == SOCKET_ERROR) {
        printf("Connect failed: %d\n", WSAGetLastError());
        closesocket(sd);
        WSACleanup();
        return 1;
    }

    printf("Connected to server.\n");
    printf("Enter binary data (type 'stop' to quit)\n");

    time_t pivot = time(NULL);

    while (1) {
        int count1 = 0;

        printf("\n%d : ", mesg_num++);
        if (fgets(buf, sizeof(buf), stdin) == NULL)
            break;

        buf[strcspn(buf, "\n")] = '\0';

        if (_stricmp(buf, "stop") == 0)
            break;

        if (strlen(buf) == 0)
            continue;

        /* Validate binary input and count 1s */
        for (int i = 0; buf[i]; i++) {
            if (buf[i] != '0' && buf[i] != '1') {
                printf("Only binary input (0/1) allowed\n");
                goto next_message;
            }
            if (buf[i] == '1')
                count1++;
        }

        /* Parity choice */
        printf("Enter parity (even / odd): ");
        if (fgets(choice, sizeof(choice), stdin) == NULL)
            break;

        choice[strcspn(choice, "\n")] = '\0';

        strcpy(encoded, buf);

        if (_stricmp(choice, "even") == 0) {
            strcat(encoded, (count1 % 2 == 0) ? "0" : "1");
            printf("Encoded data (even parity): %s\n", encoded);
        }
        else if (_stricmp(choice, "odd") == 0) {
            strcat(encoded, (count1 % 2 == 0) ? "1" : "0");
            printf("Encoded data (odd parity): %s\n", encoded);
        }
        else {
            printf("Invalid parity choice\n");
            goto next_message;
        }

        time_t now = time(NULL);
        srand(time(0));
        int diff = difftime(now, pivot);
        diff %= 10;
        if(diff >= 0 && diff < 5)
            encoded[strlen(encoded) - 1] = (rand() % 2);

        /* Combine encoded data and choice */
        snprintf(packet, sizeof(packet), "%s|%s", encoded, choice);

        /* Send combined packet */
        if (send(sd, packet, strlen(packet) + 1, 0) == SOCKET_ERROR) {
            printf("Send failed: %d\n", WSAGetLastError());
            break;
        }

        next_message:
            continue;
    }

    closesocket(sd);
    WSACleanup();
    return 0;
}

/*

To compile : gcc client_Encode.c -o client_Encode.exe -lws2_32
To run : ./client_Encode.exe

*/