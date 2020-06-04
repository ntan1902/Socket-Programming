// Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "Client.h"
#include <afxsock.h>
#include <string.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IP_SERVER_192 "192.168.1.5"

// The one and only application object

CWinApp theApp;

using namespace std;

int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // initialize MFC and print and error on failure
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: code your application's behavior here.
            wprintf(L"Fatal Error: MFC initialization failed\n");
            nRetCode = 1;
        }
        else
        {
            // TODO: code your application's behavior here.
			CSocket client;
			int port = 1234;

			/*char sAddr[1000];
			printf("Enter IP address of server: ");
			gets_s(sAddr);
			printf("IP: %s \n", sAddr);*/

			AfxSocketInit(nullptr);
			client.Create();
			
			char r_str[1000], s_str[1000];
			if (client.Connect(CA2W(IP_SERVER_192), port))
			{
				//Receive the order number of client
				int id;
				client.Receive((char*)&id, sizeof(int));
				printf("Client %d connected...\n", id + 1);

				do
				{
					//Receive message from server
					int len = client.Receive(r_str, 1000);
					r_str[len] = 0;
					printf("\tServer: %s\n", r_str);

					//Send message to server
					printf("\tClient: ");
					gets_s(s_str);
					client.Send(s_str, strlen(s_str));

				} while (strcmp(r_str, "exit") && strcmp(s_str, "exit"));
				client.Close();
			}
			
        }
    }
    else
    {
        // TODO: change error code to suit your needs
        wprintf(L"Fatal Error: GetModuleHandle failed\n");
        nRetCode = 1;
    }

    return nRetCode;
}
