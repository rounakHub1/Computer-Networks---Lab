#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET sd;
    struct sockaddr_in sad = {0};
    char buf[100], poly[50];
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

    printf("\nConnected to receiver.\n");

    printf("Enter your message (to stop, enter 'stop') :\n");
    while (1) {
        printf("\n%d > Dataword = ", (mesg_num++));
        if (fgets(buf, sizeof(buf), stdin) == NULL)
            break;

        buf[strcspn(buf, "\n")] = '\0';

        if (strlen(buf) == 0)
            continue;

        if (strcmpi(buf, "stop") == 0)
            break;

        printf("    Polynomial = ");
        if (fgets(poly, sizeof(poly), stdin) == NULL)
            break;

        poly[strcspn(poly, "\n")] = '\0';

        if (strlen(poly) == 0)
            continue;

        if (strcmpi(poly, "stop") == 0)
            break;

        char ch = '0';
        for(int i=0; i<strlen(poly)-1; i++) {
            strncat(buf, &ch, 1);
        }
        printf("Updated dataword = %s\n\n", buf);
        char subBuf[strlen(poly)]; subBuf[0] = '\0';
        int i=0, ln = strlen(buf);

        while(i<ln) {
            int rmain = sizeof(subBuf) - strlen(subBuf);
            for(int s=0; s<rmain && i<ln; s++) {
                strncat(subBuf, &buf[i++], 1);
            }
            int k=0;
            if(strlen(subBuf) == sizeof(subBuf)) {
                for(int j=0; j<sizeof(subBuf); j++) {
                    if(subBuf[j] != poly[j])
                        subBuf[k++] = '1';
                    else {
                        if(k != 0)
                            subBuf[k++] = '0';
                    }
                } subBuf[k]='\0';
            }
        }
        for(int i=strlen(subBuf) -1; i>=0; i--) {
            int j = strlen(buf) - strlen(subBuf) + i;
            buf[j] = subBuf[i];
        }
        send(sd, buf, strlen(buf), 0);
    }

    closesocket(sd);
    WSACleanup();
    return 0;
}