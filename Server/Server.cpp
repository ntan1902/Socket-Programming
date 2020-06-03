// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "Server.h"
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
			CSocket server;
			AfxSocketInit(nullptr);
			int port = 1234;

			server.Create(port);
			server.Listen(5);

			int num_clients;
			printf("Enter the number of client: ");
			scanf("%d", &num_clients);

			CSocket *clients = new CSocket[num_clients];

			for (int i = 0; i < num_clients; i++)
				server.Accept(clients[i]);
			
			
			server.Close();
			for (int i = 0; i < num_client; i++)
				clients[i].Close();
			delete[] clients;


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
