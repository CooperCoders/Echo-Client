#include <iostream>
#include <winsock2.h>
#include <stdlib.h>
#define ECHOMAX 255

using namespace std;

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in echoServAddr;
    struct sockaddr_in fromAddr;
    unsigned short echoServPort;
    unsigned int fromSize;
    char *servIP;
    char *echoString;
    char echoBuffer[ECHOMAX + 1];
    int echoStringLen;
    int respStringLen;
    WSADATA wsaData;
    servIP = "192.168.1.16"; //Change this to your local IP address or the IP address of the server
    echoString = "C";
    if ((echoStringLen = strlen(echoString) + 1) > ECHOMAX)
    {
        cout << "String too long\n";
        exit(0);
    }

    /* This is the port that the echo service will use, if using your own computer it will be your local IP adress, run cmd and type ipconfig to figure out what it is
    If connecting to another computer within your local network it will be their IP address
    If trying to connect to a non local IP, you will need to make sure that the network at the given IP address is set up to recieve a message, probably via port forwarding
    If you dont understand the basics of networking, I suggest you read up on it before using this otherwise your results will not be what is expected */
    echoServPort = 12345;

    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) /* Load Winsock 2.0 DLL */
    {
        cout << "WSAStartup failed";
        exit(1);
    }

    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        cout << "Socket creation failed/n";
        exit(0);
    }

    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family = AF_INET;
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);
    echoServAddr.sin_port = htons(echoServPort);

    if (sendto(sock, echoString, echoStringLen, 0, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr)) != echoStringLen)
    {
        cout << "sendto() failed";
        exit(0);
    }


    fromSize = sizeof(fromAddr);
    if ((respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr *) (int*)&fromAddr, (int*)&fromSize)) < 0)
    {
        cout << "recvfrom failed" << WSAGetLastError();
        exit(0);
    }
    if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr)
    {
        cout << "Received a packet from unknown source\n";
        exit(0);
    }
    if (echoBuffer[respStringLen-1])
        echoBuffer[respStringLen-1] = 0;
    else
        cout << "Received: " << echoBuffer << endl;

    closesocket(sock);
    WSACleanup();
    exit(0);
}
