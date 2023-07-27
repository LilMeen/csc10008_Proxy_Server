#ifndef METHOD_H_INCLUDED
#define METHOD_H_INCLUDED

#include "../ProxyServer.h"

#endif // METHOD_H_INCLUDED

void getData(string data, SOCKET clientSocket) {
    int BuffSize = 4096;
    char buf[BuffSize + 1];
    memset(buf, 0, sizeof(buf));

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

void Get_WebServer(const string& request, string& host, string& port)
{
	int start = request.find("Host:") + 6;
	int end = request.substr(start).find("\r\n");

	int k = (request.substr(start, end)).find(':');
	if (k == string::npos || k == -1)
	{
		host = request.substr(start, end);
		port = "80";
	}
	else
	{
		host = request.substr(start, k);
		port = request.substr(start + k + 1, end - (k + 1));
	}
}

bool isGetMethod(string data){
    int pos = data.find("GET");
    if (pos != string::npos)
        return true;
    return false;
}

bool isPostMethod(string data){
    int pos = data.find("POST");
    if (pos != string::npos)
        return true;
    return false;
}
