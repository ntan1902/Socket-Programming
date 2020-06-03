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
			char sAddr[1000];
			int port = 1234;
			
			printf("Enter IP address of server: ");
			gets_s(sAddr);
			printf("IP: %s \n", sAddr);

			AfxSocketInit(nullptr);
			client.Create();
			
			char r_str[1000], s_str[1000];
			if (client.Connect(CA2W(sAddr), port))
			{
				printf("Connected...\n");
				do
				{
					int len = client.Receive(r_str, 100, 0);
					r_str[len] = 0;
					printf("\tServer: %s", r_str);
					printf("\tClient: ");
					gets_s(s_str);
					client.Send(s_str, strlen(s_str), 0);
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
