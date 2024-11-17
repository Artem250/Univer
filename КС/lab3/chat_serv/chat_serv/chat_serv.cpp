#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <thread>
#pragma comment (lib,"Ws2_32.lib")

using namespace std;

int ServerThread(SOCKET Client, sockaddr_in ClientAddr, vector<SOCKET>&Clients)
{
    int retVal = 0;
    char user[20];
    char* recBuff = new char[1024];
    char* sendBuff = new char[1024];
    char ip[INET_ADDRSTRLEN];
    bool flag = 1;
    unsigned short port = ClientAddr.sin_port;

    Clients.push_back(Client);

    inet_ntop(AF_INET, &(ClientAddr.sin_addr), ip, INET_ADDRSTRLEN);

    recv(Client, user, 20, 0);
    _snprintf_s(sendBuff, 1024, 1024, "Пользователь с ником %s подключился с ip: %s:%d\n", user, ip, port);
    printf_s("%s", sendBuff);

    for (int i = 0; i < Clients.size(); i++)
        if (Clients[i] != Client)
            send(Clients[i], sendBuff, strlen(sendBuff), 0);

    memset(sendBuff, 0, 1024);

    while (flag)
    {
        memset(recBuff, 0, 1024);
        memset(sendBuff, 0, 1024);
        retVal = recv(Client, recBuff, 1024, 0);

        if (retVal > 0)
        {
            recBuff[retVal] = 0;

            if (!strcmp(recBuff, "/выйти")) //Проверка на попытку отклю-чения
            {
                printf("Пользователь %s с ip %s покинул чат\n", user, ip);
                _snprintf_s(sendBuff, 1024, 1024, "Пользователь %s покинул чат\n", user);

                for (int i = 0; i < Clients.size(); i++)
                {
                    if (Clients.at(i) != Client)
                        send(Clients[i], sendBuff, strlen(sendBuff), 0);
                    else
                        Clients.erase(Clients.begin() + i);
                }
                break;
            }

            printf("Сообщение от %s: %s\n", user, recBuff);
            _snprintf_s(sendBuff, 1024, 1024, "%s: %s\n", user, recBuff);

            for (int i = 0; i < Clients.size(); i++)
                send(Clients[i], sendBuff, strlen(sendBuff), 0);
        }

        else
        {
            printf("Ошибка получения данных от %s. Номер ошибки: %d\n", ip, WSAGetLastError());

            for (int i = 0; i < Clients.size(); i++)
                if (Clients[i] == Client)
                    Clients.erase(Clients.begin() + i);

            closesocket(Client);
            delete[] sendBuff;
            delete[] recBuff;
            return 1;
        }
    }
}

int main(void)
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int retVal = 0;

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET ClientSocket, ServSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ServSocket == INVALID_SOCKET)
        cout << "Создание сокета не удалось. Код ошибки: " << WSAGetLastError() << endl;

    sockaddr_in ServerAddr;
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = INADDR_ANY;
    ServerAddr.sin_port = htons(2311);

    retVal = bind(ServSocket, (sockaddr*)&ServerAddr, sizeof(ServerAddr));

    if (retVal == SOCKET_ERROR)
    {
        cout << "Привязка не удалась. Код ошибки: " << WSAGetLastError() << endl;
        closesocket(ServSocket);
        WSACleanup();
        return 1;
    }

    retVal = listen(ServSocket, 10);
    if (retVal == SOCKET_ERROR)
    {
        cout << "Привязка не удалась. Код ошибки: " << WSAGetLastError() << endl;
        closesocket(ServSocket);
        WSACleanup();
        return 1;
    }

    cout << "Сервер запущен по адресу " << inet_ntoa(ServerAddr.sin_addr) << ", port " << htons(ServerAddr.sin_port) << endl;

    vector<SOCKET> clients;
 
    while (true)
    {
        sockaddr_in ClientAddr;
        int ClientAddrSize = sizeof(ClientAddr);
        ClientSocket = accept(ServSocket, (struct sockaddr*)&ClientAddr, &ClientAddrSize);
        std::thread str(ServerThread, ClientSocket, ClientAddr, std::ref(clients));
        str.detach();
    }
    closesocket(ServSocket); //Завершение работы   
    WSACleanup();
    return 0;
}
