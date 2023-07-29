#ifndef METHOD_H_INCLUDED
#define METHOD_H_INCLUDED

#include "../ProxyServer.h"

string getClientRequest(string &data, SOCKET clientSocket) {
    int BuffSize = 4096;
    char buf[BuffSize + 1];
    memset(buf, 0, sizeof(buf));

    int recvLen = recv(clientSocket, buf, BuffSize, 0);
    if (recvLen <= 0)
    {
        return "";
    }
    data += buf;
    while (recvLen >= BuffSize)
    {
        memset(buf, 0, sizeof(buf));
        recvLen = recv(clientSocket, buf, BuffSize, 0);
        data += buf;
    }
    if (data.find("GET") != string::npos || data.find("POST") != string::npos || data.find("HEAD") != string::npos)
        return data;
    else
        return "";
}

string getWebName(string data)
{
    if (data != "")
    {
        string host, port;
        int start = data.find("Host:") + 6;
        int end = data.substr(start).find("\r\n");

        int k = (data.substr(start, end)).find(':');
        if (k == string::npos || k == -1)
        {
            host = data.substr(start, end);
            port = "80";
        }
        else
        {
            host = data.substr(start, k);
            port = data.substr(start + k + 1, end - (k + 1));
        }
        return host;
    }
    else
        return "";

}

string receiveDataFromWebServerByContentLength(SOCKET serverSocket) {
    string responseData = "";

    char headerBuffer[4096];
    int bytesReceived = recv(serverSocket, headerBuffer, sizeof(headerBuffer) - 1, 0);
    cout << headerBuffer << endl;
    if (bytesReceived <= 0) {
        return "";
    }
    headerBuffer[bytesReceived] = '\0';

    string header = headerBuffer;
    size_t contentLengthPos = header.find("Content-Length:");
    if (contentLengthPos != string::npos) {
        size_t valueStartPos = header.find_first_not_of(" ", contentLengthPos + 15);
        size_t valueEndPos = header.find("\r\n", valueStartPos);
        string contentLengthValue = header.substr(valueStartPos, valueEndPos - valueStartPos);
        int contentLength = stoi(contentLengthValue);

        char buffer[4096];
        int totalReceived = 0;
        while (totalReceived < contentLength) {
            bytesReceived = recv(serverSocket, buffer, sizeof(buffer) - 1, 0);
            if (bytesReceived <= 0) {
                break;
            }
            buffer[bytesReceived] = '\0';
            responseData += buffer;
            totalReceived += bytesReceived;
        }
    }
    return responseData;
}

void sendResponseToClient(SOCKET clientSocket, const string& responseData) {
    int sendResult = send(clientSocket, responseData.c_str(), responseData.size(), 0);
    if (sendResult == SOCKET_ERROR) {
        cout << "Error sending response to client." << endl;
        closesocket(clientSocket);
        return;
    }
}

#endif // METHOD_H_INCLUDED


