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

#include "queryengine.h"
#include "query.h"
#include "util.h"
#include "socketwriter.h"
#include "commandmap.h"
#include "configmanager.h"


//--------------------------------------------------
// Construction/Destruction
//--------------------------------------------------
QueryEngine *QueryEngine::mpQueryEngine = NULL;

QueryEngine::QueryEngine()
{
}

QueryEngine::~QueryEngine()
{
    if (mpQueryEngine != NULL)
    {
        delete mpQueryEngine;
        mpQueryEngine = NULL;
    }
}

QueryEngine *QueryEngine::Get()
{
    if (mpQueryEngine == NULL)
        mpQueryEngine = new QueryEngine();
    return mpQueryEngine;
}

//--------------------------------------------------
// Functions
//--------------------------------------------------
bool QueryEngine::ParseAndExecute(char *pRequest, SocketWriter *pWriter)
{
    Query query;
    bool bRet;

    bRet = CreateQuery(pRequest, query);
    if (!bRet)
    {
        // write error code
        pWriter->WriteError();
        goto Done;
    }

    ExecuteQuery(query, pWriter);

Done:
    return bRet;
}


bool QueryEngine::CreateQuery(char *pRequest, Query &query)
{
    char *s = pRequest;
    
    while ((uri_delims[*(unsigned char *)s] & NOTEND_SLASH) == 0) {
        ++s;
    }

    if (*s != '/')
        return FALSE;
    
    // start of path
    char *command = ++s;
    int ncommand = 0;

    while ((uri_delims[*(unsigned char *)s] & NOTEND_PATH) == 0)
        ++s;

    // end of path
    ncommand = s-command;

    // allocate the path
    query.SetPath(command, ncommand);


    // start of query string
    if (*s == '?')
    {
        do
        {
            // start of key
            char *key = ++s;
            int nkey = 0;

            while ((uri_delims[*(unsigned char *)s] & NOTEND_EQUAL) == 0) 
                ++s;
        
            // end of key
            if (*s == '=')
                nkey = s-key;
            else
                break; // error?
        
            // start of value
            char *val = ++s;
            int nval = 0;

            while ((uri_delims[*(unsigned char *)s] & NOTEND_AMP) == 0)
                ++s;

            // end of value
            nval = s-val;

            // allocate value pair
            query.SetArgument(key, nkey, val, nval);

        } while(*s == '&');
    }

    return true;
}


void QueryEngine::ExecuteQuery(Query &query, SocketWriter *pWriter)
{
    const CommandMapItem *cmdItem;

    cmdItem = FindFunction(query);

    // function was not found, write error code and return
    if(cmdItem == NULL)
    {
        pWriter->WriteError();
        goto Done;
    }

    // if the function requires a password validate it. if the validation
    // fails write an error code and return
    {
        const char *pswd = query.GetArgument("p");
        if(pswd)
        {
            if(!ConfigManager::Get()->ValidatePassword(pswd))
            {
                pWriter->WriteError();
                goto Done;
            }
        }
    }

    // success; execute function
    cmdItem->Function(query, pWriter);

Done:
    return;
}
