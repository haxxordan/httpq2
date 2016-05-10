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

#include "socketreader.h"
#include "util.h"

//--------------------------------------------------
// Constructor/Destructor
//--------------------------------------------------
SocketReader::SocketReader(SOCKET socket)
{
    mSocket = socket;
}

SocketReader::~SocketReader()
{
}

int SocketReader::ReadToEnd()
{
    int rc;
    int pos = 0;
    int lastindex = -1;
    int size = LIMIT_REQUEST_BUFFER;

	do
	{
		// read data from the socket
		rc = recv(mSocket, &mBuffer[pos], size-pos, 0);

		// error
		if(rc == SOCKET_ERROR) 
		{	
			OutputDebugString("ReadRequest: recv failed, error reading from socket.\n");
			return -1;
		}

		// timeout ?? or gracefully closed
		if(rc == 0)
		{
			OutputDebugString("Timeout reading socket.\n");
			return 0;
		}

		pos += rc;
        lastindex = lastindexof(&mBuffer[0], HTTP_TERMCODE);

	// stop when reached buf limit or found term code
	} while( ((size-pos) > 0) &&  (lastindex == -1) ); 


	// buffer limit reached
	if((size-pos) <= 0)
	{
		OutputDebugString("Buffer size limit reached.\n");
		return -1;
	}

	// terminate the string
	mBuffer[(lastindex == -1) ? pos : lastindex] = '\0';

    return rc;
}
