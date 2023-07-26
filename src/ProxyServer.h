#ifndef PROXYSERVER_H_INCLUDED
#define PROXYSERVER_H_INCLUDED

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string>
using namespace std;

#include "Method/Method.h"
#include "Cache/Cache.h"
#include "Whitelist/Whitelist.h"

void runProxy (){
    WSAData wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) //to use WSAStartup, link library lws2_32 when compile by cmd.
	{
		cout << "Version not supported!\n";
		return;
	}
	else
        cout << "Create Winsock successful!\n";

    SOCKET ProxyServer;
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));

	ProxyServer = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (ProxyServer == INVALID_SOCKET){
		cout << "Invalid socket!\n";
		return;
	}

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8888);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(ProxyServer, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR){
		cout << "Cannot bind to socket!\n";
		return;
	}

	if (listen(ProxyServer, 25)){
		cout << "Listen failed!\n";
		return;
	}

	//TEST

	while (true) {
        SOCKADDR_IN clientAddr;
        int clientAddrLen = sizeof(clientAddr);
        SOCKET clientSocket = accept(ProxyServer, (SOCKADDR*)&clientAddr, &clientAddrLen);
        if (clientSocket == INVALID_SOCKET) {
            cout << "Failed to accept client connection.\n";
            continue;
        }
        //cout << "Accepted connection from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << "\n";

        int BuffSize = 4096;
        char buf[BuffSize + 1];
        memset(buf, 0, sizeof(buf));

        string data = "";

        int recvLen = recv(clientSocket, buf, BuffSize, 0);
        if (recvLen <= 0)
        {
            return;
        }
        data += buf;

        while (recvLen >= BuffSize)
        {
            memset(buf, 0, sizeof(buf));
            recvLen = recv(clientSocket, buf, BuffSize, 0);
            data += buf;
        }
        if (data.find("GET") != string::npos)
            cout << data << "\n";
    }

    closesocket(ProxyServer);
    WSACleanup();
}
#endif // PROXYSERVER_H_INCLUDED
