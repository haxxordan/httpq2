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
#ifndef __CONFIGINFO_H
#define __CONFIGINFO_H

class ConfigInfo
{
public:
    friend class ConfigManager;

    ConfigInfo();
	ConfigInfo& operator=(const ConfigInfo &info);
    ~ConfigInfo();

    inline DWORD Version() const { return dwVersion; };
    inline DWORD GetIpAddress() const { return dwIpAddress; };
    inline bool IsAutoStartEnabled() const { return (nAutoStart == 1); };
    inline bool IsHttpHeadersEnabled() const { return (nHttpHeaders == 1); }
    inline bool IsAllowAllIpEnabled() const { return (nAllowAllIp == 1); }
    inline bool IsDenyAllIpEnabled() const { return (nAllowAllIp == 0); }
    inline int GetPort() const { return nPort; }
    inline int GetConnectionTimeout() const { return nConnectionTimeout; }
    inline const char *GetPassword() const { return &strPassword[0]; }
    inline bool IsAllowedIp(DWORD ip) const { return dwAllowIpList.Contains(ip); };
    inline bool IsDeniedIp(DWORD ip) const { return dwDenyIpList.Contains(ip); };

    inline void EnableAllowAllIp() { (nAllowAllIp = 1); }
    inline void EnableDenyAllIp() { (nAllowAllIp = 0); }
    inline void SetIpAddress(DWORD ipaddr) { dwIpAddress = ipaddr; };
    inline void SetAutoStart(bool enable) { nAutoStart = enable?1:0; };
    inline void SetHttpHeaders(bool enable) { nHttpHeaders = enable?1:0; };
    inline void SetAllowAllIp(bool enable) { nAllowAllIp = enable?1:0; };
    inline void SetPort(int port) { nPort = port; }
    inline void SetConnectionTimeout(int timeout) { nConnectionTimeout = timeout; }
    inline void SetPassword(const char *pass) { strcpy(strPassword, pass); }
    inline unsigned int AddAllowedIp(DWORD ip) { return dwAllowIpList.Add(ip); };
    inline unsigned int AddDeniedIp(DWORD ip) { return dwDenyIpList.Add(ip); };

    inline DWORD GetAllowedIp(int n) const { return dwAllowIpList[n]; };
    inline DWORD GetDeniedIp(int n) const { return dwDenyIpList[n]; };
    inline unsigned int RemoveAllowedIp(DWORD ip) { return dwAllowIpList.Remove(ip); };
    inline unsigned int RemoveDeniedIp(DWORD ip) { return dwDenyIpList.Remove(ip); };
    inline unsigned int NumAllowedIp() const { return dwAllowIpList.Count(); };
    inline unsigned int NumDeniedIp() const { return dwDenyIpList.Count(); };

private:
    DWORD   dwVersion;
    DWORD	dwIpAddress;
	int		nAutoStart;
	int		nHttpHeaders;
	int		nPort;
	int		nAllowAllIp;
	int		nConnectionTimeout;
	char	strPassword[MAX_STRING];
	Array<DWORD> dwAllowIpList;
	Array<DWORD> dwDenyIpList;
};


#endif //__CONFIGINFO_H