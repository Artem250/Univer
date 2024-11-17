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

void ClientThread(SOCKET ClientSocket, char* RecBuff, int size) {
   int retVal = 0;

   while (true)
   {
      retVal = recv(ClientSocket, RecBuff, size - 1, 0);
      if (retVal > 0)
      {
         RecBuff[retVal] = 0;
         printf("%s", RecBuff);
      }
      else
         break;
   }
}


int main(void)
{
   SetConsoleCP(1251);
   SetConsoleOutputCP(1251);

   int retVal = 0;
   char* recBuff = new char[1024];
   char* sendBuff = new char[1024];
   char* user = new char[20];

   WSADATA wsaData;
   WSAStartup(MAKEWORD(2, 2), &wsaData);

   SOCKET ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (ClientSocket == INVALID_SOCKET)
      cout << "Создание сокета не удалось. Код ошибки: " << WSAGetLastError() << endl;

   string ip;
   unsigned short port;
   cout << "ip> ";
   cin >> ip;
   cout << "port>";
   cin >> port;
   cin.ignore();

   sockaddr_in ServerAddr;
   ServerAddr.sin_family = AF_INET;
   ServerAddr.sin_addr.s_addr = inet_addr(ip.c_str());
   ServerAddr.sin_port = htons(port);

   retVal = connect(ClientSocket, (sockaddr*)&ServerAddr, sizeof(ServerAddr));
   if (retVal == SOCKET_ERROR)
   {
      cout << "Ошибка подключения. Код Ошибки: " << WSAGetLastError() << endl;
      closesocket(ClientSocket);
      WSACleanup();
      return 1;
   }

   cout << "Соединение установлено. Пожалуйста, введите ваш никнейм, не превышающий 20 символов: " << endl;
   //scanf("%s", user);
   cin >> user;
   send(ClientSocket, user, 20, 0);

   std::thread str(ClientThread, ClientSocket, recBuff, 1024);
   str.detach();

   for (bool flag = true; flag;)
   {
      gets_s(sendBuff, 1024);
      if (!strcmp(sendBuff, "/выйти"))
         flag = false;
      send(ClientSocket, sendBuff, strlen(sendBuff), 0);
   }

   delete[] user;
   delete[] sendBuff;
   delete[] recBuff;
   closesocket(ClientSocket);
   WSACleanup();
   return 1;
}
