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
#ifndef __SERVER_H
#define __SERVER_H


typedef struct SocketData
{
	SOCKET socket;
	struct sockaddr_in addr;

	SocketData(SOCKET s, struct sockaddr_in a)
	{
		socket = s;
		addr = a;
	};

} SocketData;


class Server
{
public:
    static Server *Get();

    enum ByteOrder
    {
        Network,
        Host
    };
    
    void Start();
    void Stop();

    bool ValidateIpAddress(sockaddr_in addr);
    void GetIpAddressList(Array<DWORD> &ipList);
    char *ConvertToAddress(DWORD dwIp, ByteOrder byteOrder);
    DWORD ConvertToIp(const char *pstrAddr);

    inline bool IsRunning() { return mbServerStarted; }
    inline void SetRunning(bool bRun) { mbServerStarted = bRun; }

private:
    Server();
    ~Server();

    static Server *mpServer;
    
    bool mbServerStarted;
};


#endif // __SERVER_H