#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET sd;
    struct sockaddr_in sad = {0};
    char buf[100];
    int mesg_num = 1;

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

    if (connect(sd, (struct sockaddr*)&sad, sizeof(sad)) == SOCKET_ERROR) {
        printf("\nconnect failed: %d\n", WSAGetLastError());
        return 1;
    }

    printf("\nConnected to server.\n");

    printf("Enter your message (to stop, enter 'stop') :\n");
    while (1) {
        printf("%d :\t", (mesg_num++));
        if (fgets(buf, sizeof(buf), stdin) == NULL)
            break;

        buf[strcspn(buf, "\n")] = '\0';

        if (strlen(buf) == 0)
            continue;

        send(sd, buf, strlen(buf), 0);

        if (strcmpi(buf, "stop") == 0)
            break;
    }

    closesocket(sd);
    WSACleanup();
    return 0;
}

/*

To compile : gcc client.c -o client.exe -lws2_32
To run : ./client.exe

*/