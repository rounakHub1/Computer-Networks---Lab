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

    printf("\nWaiting for sender...\n");

    cd = accept(sd, (struct sockaddr*)&cad, &cadl);
    if (cd == INVALID_SOCKET) {
        printf("\naccept failed: %d\n", WSAGetLastError());
        return 1;
    }

    printf("\nSender connected.\n");
    int i=1;

    while (1) {
        int bytes = recv(cd, buf, sizeof(buf) - 1, 0);

        if (bytes == 0) {
            printf("\nSender disconnected.\n");
            break;
        }
        if (bytes == SOCKET_ERROR) {
            printf("\nrecv failed: %d\n", WSAGetLastError());
            break;
        }

        buf[bytes] = '\0';
        printf("\n%d > Codeword = %s\n", i++, buf);
    }

    closesocket(cd);
    closesocket(sd);
    WSACleanup();
    return 0;
}

/*

To compile : gcc CRCreceiver.c -o receiver.exe -lws2_32
To run : ./receiver.exe

*/