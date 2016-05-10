/*
 * Winamp httpQ Plugin
 * Copyright (C) 1999-2003 Kosta Arvanitis
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Kosta Arvanitis (karvanitis@hotmail.com)
 */
#include "httpq.h"

#include <commctrl.h>
#include <process.h>

#include "util.h"
#include "server.h"
#include "configmanager.h"
#include "configdialog.h"
#include "queryengine.h"
#include "socketreader.h"
#include "socketwriter.h"


//--------------------------------------------------
// Global Variables
//--------------------------------------------------
SOCKET  giServerSocket = NULL;
bool    gbServerStarted = false;
bool    gbShutdownPending = false;


//--------------------------------------------------
// Global Function Definitions
//--------------------------------------------------
void    ServerLoop();
void    HandleClientSocket(void *pData);
void    RunServer(void *pData);
void    Shutdown();


//--------------------------------------------------
// Constructor/Destructor
//--------------------------------------------------
Server *Server::mpServer = NULL;

Server::Server()
{
    mbServerStarted = false;
}

Server::~Server()
{
    if (mpServer != NULL)
    {
        delete mpServer;
        mpServer = NULL;
    }
}

Server *Server::Get()
{
    if (mpServer == NULL)
        mpServer = new Server();
    return mpServer;
}

//--------------------------------------------------
// Functions
//--------------------------------------------------
void Server::Start()
{
	gbServerStarted = true;
	HANDLE hThread = (HANDLE)_beginthread(RunServer, 0, NULL);
	SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);
}


void Server::Stop()
{
	if(!mbServerStarted)
		return;

	try 
	{
        Shutdown();
    }
    catch(...)
    {
        OutputDebugString("Exception caught: Server::Stop failed.\n");
    }
}


void RunServer(void *pData)
{
	try
    {
		// start server, assume started
        Server::Get()->SetRunning(true);
        
        // launch server loop (blocking call)
        ServerLoop(); 
    }
    catch(...)
    {
        // server failed to start
		OutputDebugString("Exception caught: ServerLoop failed.\n");

		// stop server
        Server::Get()->Stop();
    }

	// update ui, ignonred if not visible
	//SendMessage(gConfigWnd, APP_EN_START, 0, (LPARAM)(BOOL)TRUE);
	//SendMessage(gConfigWnd, APP_EN_STOP, 0, (LPARAM)(BOOL)FALSE);

	// server has shut down
	OutputDebugString("Server: Server Shutdown.\n");
	
    Server::Get()->SetRunning(false);
}

//--------------------------------------------------
// Main server loop
//--------------------------------------------------
void ServerLoop()
{
    WSADATA	WSAData;                    
    struct sockaddr_in addrLocal, addrFrom;
	int nFromLen, rc, nTimeout;
    fd_set readfds, writefds, exceptfds;
    unsigned short nPort = ConfigManager::Get()->GetConfig()->GetPort();
	unsigned long ioctl_opt = 0;
	SOCKET iClientSocket = INVALID_SOCKET;
	gbShutdownPending = FALSE;


	// attempt to initiate windows sockets
 	rc = WSAStartup(WINSOCKVERSION, &WSAData);
	if(0 != rc)
	{
		OutputDebugString("ServerLoop: WSAStartup failed.\n");
		goto Shutdown;
	}
	
    // fill out the server socket address data. 
    addrLocal.sin_family = AF_INET;
    addrLocal.sin_addr.s_addr = ConfigManager::Get()->GetConfig()->GetIpAddress();

	// Port must be in network byte order
	addrLocal.sin_port = htons(nPort);

    // create a tcp/ip socket bound to the local server.
    giServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(giServerSocket == INVALID_SOCKET)
    {
		OutputDebugString("ServerLoop: Allocating socket failed\n");
		goto Shutdown;
    }

    // associate a local address and port combination with the socket just created. 
    rc = bind(giServerSocket, (struct sockaddr*)&addrLocal, sizeof(addrLocal));
	if(rc == SOCKET_ERROR)
    {
        OutputDebugString("ServerLoop: Binding socket failed.\n");
		goto Shutdown;
    }

	// allow max number incomming connections (queued up)
	rc = listen(giServerSocket, SOMAXCONN);
	if(SOCKET_ERROR == rc) 
	{
        OutputDebugString("ServerLoop: Listen socket failed.\n");
		goto Shutdown;
	}

	// set the socket to blocking mode
	rc = ioctlsocket(giServerSocket, FIONBIO, &ioctl_opt);
	if(SOCKET_ERROR == rc) 
	{
        OutputDebugString("ServerLoop: I/O Control socket failed. Shuting down server.\n");
		goto Shutdown;
    }

	// set the timeout period for recv calls. 
	// ONLY works with winsock 2.0 so dont shutdown on failure
	nTimeout = ConfigManager::Get()->GetConfig()->GetConnectionTimeout();
	rc = setsockopt(giServerSocket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&nTimeout, sizeof(int));
	if(rc == SOCKET_ERROR)
	{
		OutputDebugString("ServerLoop: setsockopt SO_RCVTIMEO failed.\n");
	}

	// initialize sets of sockets to zero
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	FD_ZERO(&exceptfds);

	// add server socket to read set
	FD_SET(giServerSocket, &readfds);
	
	// start the server loop 
	while(TRUE)
    { 
		// blocking call, waits for a socket connection
		rc = select(0, &readfds, NULL, NULL,  NULL);

		if(gbShutdownPending)
			goto Shutdown;
		
		// socket erorr
		if(SOCKET_ERROR == rc)
        {
			// if select fails repeatedly this could cause a problem?
			OutputDebugString("ServerLoop: Select Error socket failed.\n");
			continue;
		}

		// timeout error
		if(0 == rc)
        {
			OutputDebugString("ServerLoop: Select timed out.\n");
			continue;
        }

		// socket in read set
		if(FD_ISSET(giServerSocket, &readfds))
		{
			// size of sockaddr struct
			nFromLen = sizeof(addrFrom);

			// allow incomming remote client connection
			iClientSocket = accept(giServerSocket, (struct sockaddr*)&addrFrom, &nFromLen);

			// check client socket
			if(INVALID_SOCKET != iClientSocket)
			{
				// spaw client socket handler
				SocketData *data = new SocketData(iClientSocket, addrFrom);
				_beginthread(HandleClientSocket, 0, (void *)data);
			}
		}
	}

Shutdown:
    // shutdown
    if (!gbShutdownPending)
    {
        Shutdown();
    }
}


//--------------------------------------------------
// Shutdown server
//--------------------------------------------------
void Shutdown()
{
    int rc;
    LINGER linger;

    // set shutdown flags
	gbShutdownPending = TRUE;

    // shutdown server and winsock
    linger.l_onoff = 1;
    linger.l_linger = 0;

    rc = setsockopt(giServerSocket, SOL_SOCKET, SO_LINGER, (char *)&linger, sizeof(linger));
	if(rc == SOCKET_ERROR)
	{
		OutputDebugString("ServerLoop: setsockopt SO_LINGER failed.\n");
	}

    rc = closesocket(giServerSocket);
    if(rc != 0)
    {
		OutputDebugString("Shutdown: closesocket failed.\n");
    }

    rc = WSACleanup();
    if(rc != 0)
    {
		OutputDebugString("Shutdown: WSACleanup failed.\n");
    }

    // notify of changes
    Server::Get()->SetRunning(FALSE);
    ConfigDialog::Get()->Refresh();
}


//--------------------------------------------------
// Thread handler for individual connections
//--------------------------------------------------
void HandleClientSocket(void *pData)
{
	SocketData *data = (SocketData *)pData;
	SOCKET iClientSocket = data->socket;
    struct sockaddr_in iSockAddr = data->addr;

    SocketReader *pReader = NULL;
    SocketWriter *pWriter = NULL;

	// check client socket
	if(iClientSocket == INVALID_SOCKET)
	{
		OutputDebugString("Invalid Socket.\n");
		goto Shutdown;
	}

	// check the ip
    if(!Server::Get()->ValidateIpAddress(iSockAddr))
	{
		OutputDebugString("Ip address was rejected.\n");
		goto Shutdown;
	}
	
	// allocate request buffer
    pReader = new SocketReader(iClientSocket);

	// socket is ready for read
    if(pReader->ReadToEnd() <= 0)
    {
		OutputDebugString("Error reading socket.\n");
		goto Shutdown;
    }

    // allocate reply buffer
    pWriter = new SocketWriter(iClientSocket);

	// write the http headers
    if(ConfigManager::Get()->GetConfig()->IsHttpHeadersEnabled())
	{
        if (pWriter->WriteHeader() <= 0)
        {
			OutputDebugString("Error writing to socket: HTTP Headers.\n");
			goto Shutdown;
        }
    }
	
    // handle commands contained in the request buffer
    QueryEngine::Get()->ParseAndExecute(pReader->GetBuffer(), pWriter);
    

Shutdown: 
	// close client socket
	closesocket(iClientSocket);

    // free
    if(pReader)
        delete pReader;
    if(pWriter)
        delete pWriter;

    // free thread data
	if(data)
		delete data;
}


//--------------------------------------------------
// Check valid ip against ip vectors
//--------------------------------------------------
bool Server::ValidateIpAddress(sockaddr_in addr)
{
    DWORD dwIp = MAKEIPADDRESS(
			addr.sin_addr.S_un.S_un_b.s_b1,
			addr.sin_addr.S_un.S_un_b.s_b2,
			addr.sin_addr.S_un.S_un_b.s_b3,
			addr.sin_addr.S_un.S_un_b.s_b4);

    if (ConfigManager::Get()->GetConfig()->IsAllowAllIpEnabled())
    {
        if (ConfigManager::Get()->GetConfig()->IsDeniedIp(dwIp))
            return false;
        return true;
    }
    else
    {
        if (ConfigManager::Get()->GetConfig()->IsAllowedIp(dwIp))
            return true;
        return false;
    }
    
}


// Get a list of ip addrs for this machine
void Server::GetIpAddressList(Array<DWORD> &ipList)
{
    WSADATA WSAData;
    HOSTENT *phost;
    char hostname[128];
    int rc;

	rc = WSAStartup(WINSOCKVERSION, &WSAData);
    if (rc == 0)
    {
        rc = gethostname(hostname, 128);
        if (rc == 0)
        {
            phost = gethostbyname(hostname);
            if (phost)
            {
                for(char **p = phost->h_addr_list; *p; p++) 
                {
                    DWORD ip = *(DWORD*)(*p);
                    if (ipList.Add(ip) < 0)
                        break;
                }
            }
        }
    }

    WSACleanup();

    /*
    WSADATA	WSAData;
    HOSTENT *phost;

	int rc = WSAStartup(WINSOCKVERSION, &WSAData);
    if(rc == 0)
    {
	    sockaddr_in sin;
        sin.sin_family = AF_INET;
	    sin.sin_addr.S_un.S_addr = INADDR_ANY;
	    phost = gethostbyaddr((char*)&sin.sin_addr, 4, PF_INET);

        if (phost)
        {
            for(char **p = phost->h_addr_list; *p; p++) 
            {
                DWORD ip = *(DWORD*)(*p);
                if (ipList.Add(ip) < 0)
                    break;
            }
        }
    }
		
    WSACleanup();
    */
}


// convert internet host/network address to an internet dot address
char *Server::ConvertToAddress(DWORD dwIp, ByteOrder byteOrder)
{
    // convert 32-bit value from network to host byte order
    if(byteOrder == ByteOrder::Network)
		dwIp = ntohl(dwIp);	

	in_addr inetAddr;
	inetAddr.s_addr = dwIp;

	// convert internet host address to an internet dot address
    return inet_ntoa(inetAddr);
}

// convert internet dot address to unsigned long
DWORD Server::ConvertToIp(const char *pstrAddr)
{
    return (DWORD)inet_addr(pstrAddr);
}
