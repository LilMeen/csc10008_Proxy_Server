#ifndef PROXYSERVER_H_INCLUDED
#define PROXYSERVER_H_INCLUDED

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string>
#include <string.h>
#include <vector>
#include <fstream>
#include <thread>
using namespace std;

#include "Method/Method.h"
#include "Cache/Cache.h"
#include "Whitelist/Whitelist.h"

void handleClient(SOCKET clientSocket) {
	string data = "";
	string webName = getWebName(getClientRequest(data, clientSocket));
	if (webName != "" && webName.find("detectportal") == string::npos)
	{
		cout << webName << "\n";
		data += "Connection: close\r\n";
		cout << data << endl;

		SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (serverSocket == INVALID_SOCKET) {
			cout << "Error creating server socket." << endl;
			return;
		}

		struct hostent* hostentData;
		hostentData = gethostbyname(webName.c_str());
		if (hostentData == NULL) {
			cout << "Error resolving hostname." << endl;
			closesocket(serverSocket);
			return;
		}

		SOCKADDR_IN serverAddr;
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(80);
		serverAddr.sin_addr.s_addr = *((unsigned long*)hostentData->h_addr);

		if (connect(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) != SOCKET_ERROR)
		{
			cout << "Connected to " << webName << "\nHost IP address: " << inet_ntoa(serverAddr.sin_addr) << "\n";

			//string httpRequest = data + "\r\n\r\n"; 
			string httpRequest = data;
			int sendResult = send(serverSocket, httpRequest.c_str(), httpRequest.size(), 0);
			if (sendResult == SOCKET_ERROR)
			{
				cout << "Error sending request to server." << endl;
				closesocket(serverSocket);
				return;
			}
			
			cout << "Request is sent to server! \n\n";

			string s = receiveDataFromWebServerByContentLength(serverSocket);
			cout << s << endl;
			cout << "===========================================" << endl;

			sendResponseToClient(clientSocket, s);
		}	
	}
}

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

		handleClient(clientSocket);
	}

    closesocket(ProxyServer);
    WSACleanup();
}
#endif // PROXYSERVER_H_INCLUDED
