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

#include "socketwriter.h"


//--------------------------------------------------
// Constructor/Destructor
//--------------------------------------------------
SocketWriter::SocketWriter(SOCKET socket)
{
    mSocket = socket;
}

SocketWriter::~SocketWriter()
{
}

int SocketWriter::WriteHeader()
{
    char tmp[1024];

    sprintf(tmp, "%sServer: %s %s\r\n%s%s", 
	    HTTP_HEADER_OK, 
	    APPNAME, VERSION,
	    HTTP_HEADER_CONTENT,
	    HTTP_HEADER_TERM);

    return SendSocketData(tmp);
}

int SocketWriter::WriteError()
{
    char tmp[2];
    sprintf(tmp, "%s", HTTPQ_REPLY_ERROR);
    return SendSocketData(tmp);
}

int SocketWriter::WriteSuccess()
{
    char tmp[2];
    sprintf(tmp, "%s", HTTPQ_REPLY_OK);
    return SendSocketData(tmp);
}

int SocketWriter::WriteReturnCode(int rc)
{
    char tmp[128];
    sprintf(tmp, "%d", rc);
    return SendSocketData(tmp);
}

int SocketWriter::WriteData(const char *pData)
{
    return SendSocketData(pData);
}

int SocketWriter::SendSocketData(const char *pData)
{
	int rc;
	
	// write to socket
	rc = send(mSocket, pData, strlen(pData), 0);
	
	// error in send
	if(rc == SOCKET_ERROR) 
	{
		OutputDebugString("send failed.\n");
		return -1;
	}

	// return number of bytes written
	return rc;
}
