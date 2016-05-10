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

#include "configmanager.h"
#include "pluginmanager.h"
#include "configinfo.h"


//--------------------------------------------------
// Constructor/Destructor
//--------------------------------------------------
ConfigManager *ConfigManager::mpConfigManager = NULL;

ConfigManager::ConfigManager()
{
    mpConfigInfo = new ConfigInfo();

	GetModuleFileName(PluginManager::Get()->GetPluginInterface()->hDllInstance, mIniFile, sizeof(mIniFile));
	char *p = mIniFile + lstrlen(mIniFile);
	while(p >= mIniFile && *p != '.') 
		p--;
	if( ++p >= mIniFile) 
		*p = 0;
	lstrcat(mIniFile, "ini");
}

ConfigManager::~ConfigManager()
{
    if(mpConfigManager != NULL)
    {
        delete mpConfigManager;
        mpConfigManager = NULL;
    }

    if(mpConfigInfo != NULL)
    {
        delete mpConfigInfo;
        mpConfigInfo = NULL;
    }
}

ConfigManager *ConfigManager::Get()
{
    if (mpConfigManager == NULL)
        mpConfigManager = new ConfigManager();
    return mpConfigManager;
}


//--------------------------------------------------
// Functions
//--------------------------------------------------

bool ConfigManager::ValidatePassword(const char *password)
{
	if(!strcmp(GetConfig()->GetPassword(), password))
		return TRUE;
	return FALSE;
}

void ConfigManager::SetConfig(ConfigInfo &config)
{
    *mpConfigInfo = config;
}

UINT ConfigManager::ReadInt(char *key, int def)
{
    return GetPrivateProfileInt(APPNAME, key, def, mIniFile);
}

BOOL ConfigManager::WriteInt(char *key, int value)
{
    char tmp[256];
    return WritePrivateProfileString(APPNAME, key, itoa(value, tmp, 10), mIniFile);
}

DWORD ConfigManager::ReadString(char *key, char *buffer, int size)
{
    return GetPrivateProfileString(APPNAME, key, buffer, buffer, size, mIniFile);
}

BOOL ConfigManager::WriteString(char *key, char *buffer)
{
    return WritePrivateProfileString(APPNAME, key, buffer, mIniFile);
}


// Read and load data from disk
void ConfigManager::Deserialize()
{
    ConfigInfo info;

    info.dwVersion= ReadInt("Version", info.dwVersion);
    info.dwIpAddress = ReadInt("IpAddress", info.dwIpAddress);
	info.nAutoStart = ReadInt("AutoStart", info.nAutoStart);
	info.nHttpHeaders = ReadInt("HttpHeaders", info.nHttpHeaders);
	info.nPort = ReadInt("Port", info.nPort);
	info.nAllowAllIp = ReadInt("AllowAllIp", info.nAllowAllIp);
	info.nConnectionTimeout = ReadInt("ConnectionTimeout", info.nConnectionTimeout);

    ReadString("Password", info.strPassword, sizeof(info.strPassword));

    char tmp[256];
    unsigned int i, len;

    len = ReadInt("AllowIpList", 0);
    for(i = 0; i < len; i++)
    {
        sprintf(tmp, "AllowIpList%d", i);
        DWORD dwAddr = ReadInt(tmp, 0);
        info.dwAllowIpList.Add(dwAddr);
    }

    len = ReadInt("DenyIpList", 0);
    for(i = 0; i < len; i++)
    {
        sprintf(tmp, "DenyIpList%d", i);
        DWORD dwAddr = ReadInt(tmp, 0);
        info.dwDenyIpList.Add(dwAddr);
    }
    
    if (info.dwVersion == APPVERSION)
        *(this->mpConfigInfo) = info;
}


// Write data to disk.
void ConfigManager::Serialize()
{
    WriteInt("Version", mpConfigInfo->dwVersion);
    WriteInt("IpAddress", mpConfigInfo->dwIpAddress);
	WriteInt("AutoStart", mpConfigInfo->nAutoStart);
	WriteInt("HttpHeaders", mpConfigInfo->nHttpHeaders);
	WriteInt("Port", mpConfigInfo->nPort);
	WriteInt("AllowAllIp", mpConfigInfo->nAllowAllIp);
	WriteInt("ConnectionTimeout", mpConfigInfo->nConnectionTimeout);

    WriteString("Password", mpConfigInfo->strPassword);

    char tmp[256];
    unsigned int i;

    WriteInt("AllowIpList", mpConfigInfo->dwAllowIpList.Count());
    for(i = 0; i < mpConfigInfo->dwAllowIpList.Count(); i++)
    {
        sprintf(tmp, "AllowIpList%d", i);
        WriteInt(tmp, mpConfigInfo->dwAllowIpList[i]);
    }

    WriteInt("DenyIpList", mpConfigInfo->dwDenyIpList.Count());
    for(i = 0; i < mpConfigInfo->dwDenyIpList.Count(); i++)
    {
        sprintf(tmp, "DenyIpList%d", i);
        WriteInt(tmp, mpConfigInfo->dwDenyIpList[i]);
    }
}
