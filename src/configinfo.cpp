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

#include "array.h"
#include "configinfo.h"


//--------------------------------------------------
// Constructor/Destructor
//--------------------------------------------------
ConfigInfo::ConfigInfo()
{
    dwVersion =          APPVERSION;
    dwIpAddress =	 	 DEFAULT_IP_ADDRESS;
	nAutoStart =		 DEFAULT_AUTO_START_ENABLED;
	nHttpHeaders =		 DEFAULT_HTTP_HEADERS_ENABLED;
	nPort =				 DEFAULT_PORT;
	nAllowAllIp =	     DEFAULT_ALLOW_ALL_IP_ENABLED;
	nConnectionTimeout = DEFAULT_CONNECTION_TIMEOUT;

    dwAllowIpList.Clear();
    dwAllowIpList.Clear();

    int len = strlen(DEFAULT_PASSWORD);
    strncpy(strPassword, DEFAULT_PASSWORD, len);
    strPassword[len] = '\0';
}


//--------------------------------------------------
// Assignment operator
//--------------------------------------------------
ConfigInfo& ConfigInfo::operator=(const ConfigInfo &info)
{
    if (this == &info) 
        return *this;

    this->dwVersion =           info.dwVersion;
    this->dwIpAddress =         info.dwIpAddress;
	this->nAutoStart =          info.nAutoStart;
	this->nHttpHeaders =        info.nHttpHeaders;
	this->nPort =               info.nPort;
	this->nAllowAllIp =         info.nAllowAllIp;
	this->nConnectionTimeout =  info.nConnectionTimeout;
    
    strcpy(this->strPassword, info.strPassword);

    this->dwAllowIpList = info.dwAllowIpList;
    this->dwDenyIpList = info.dwDenyIpList;

	return *this;
}


ConfigInfo::~ConfigInfo()
{
}

