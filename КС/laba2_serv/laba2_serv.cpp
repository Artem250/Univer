﻿#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#pragma comment (lib,"Ws2_32.lib")

using namespace std;

string process(string input, string ip, unsigned short port) {
   string result = input;
   string toReplace = ".";
   ostringstream oss;
   oss << ". (" << ip << " " << port << ")";
   std::string replacement = oss.str();
   size_t pos = 0;
   while ((pos = result.find(toReplace, pos)) != std::string::npos) {
      result.replace(pos, toReplace.length(), replacement);
      pos += replacement.length();
   }
   return result;
}


int main(void)
{
   SetConsoleCP(1251);
   SetConsoleOutputCP(1251);

   int retVal;

   WSADATA wsaData;
   WSAStartup(MAKEWORD(2, 2), &wsaData);

   SOCKET SendRecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
   if (SendRecvSocket == INVALID_SOCKET)
      cout << "Создание сокета не удалось. Код ошибки: " << WSAGetLastError() << endl;

   sockaddr_in ServerAddr;
   ServerAddr.sin_family = AF_INET;
   ServerAddr.sin_addr.s_addr = inet_addr("25.9.202.82");
   ServerAddr.sin_port = htons(2011);

   retVal = bind(SendRecvSocket, (sockaddr*)&ServerAddr, sizeof(ServerAddr));

   if (retVal == SOCKET_ERROR)
   {
      cout << "Привязка не удалась. Код ошибки: " << WSAGetLastError() << endl;
      closesocket(SendRecvSocket);
      WSACleanup();
      return 1;
   }

   cout << "Сервер запущен по адресу " << inet_ntoa(ServerAddr.sin_addr) << ", port " << htons(ServerAddr.sin_port) << endl;

   while (true)
   {
      sockaddr_in ClientAddr;
      int ClientAddrSize = sizeof(ClientAddr);
      char szReq[1024];
      retVal = recvfrom(SendRecvSocket, szReq, 1024, 0, (sockaddr*)&ClientAddr, &ClientAddrSize);//Пытаемся получить данные от клиента

      if (retVal == SOCKET_ERROR)
         cout << "Получить данные не удалось. Код ошибки: " << WSAGetLastError() << endl;

      cout << "Данные получены от " << inet_ntoa(ClientAddr.sin_addr) << ", port " << htons(ClientAddr.sin_port) << endl;

      szReq[retVal] = 0;
      string s = (const char*)szReq;

      if (s[0] == 's') // Команда на выключение сервера
      {
         retVal = sendto(SendRecvSocket, "Выключение сервера", 20, 0, (sockaddr*)&ClientAddr, sizeof(ClientAddr));
         closesocket(SendRecvSocket);
         break;
      }

      else //Обработка полученных предложений
      {
         char szResp[1024] = { '\0' };
         string result = process(s, inet_ntoa(ServerAddr.sin_addr), htons(ServerAddr.sin_port));

         if (result.length() > 1023)
            result = result.substr(0, 1023);

         for (int i = 0; i < result.length(); i++)
            szResp[i] = result[i];

         szResp[result.length()] = 0;

         cout << "Отправляем ответ из сервера" << endl; //Пытаемся отослать данные клиенту
         retVal = sendto(SendRecvSocket, szResp, result.length(), 0, (sockaddr*)&ClientAddr, sizeof(ClientAddr));

         if (retVal == SOCKET_ERROR)
            cout << "Невозможно отправить данные. Код ошибки: " << WSAGetLastError() << endl;
      }
   }
   closesocket(SendRecvSocket); //Завершение работы   
   WSACleanup();
   return 0;
}
