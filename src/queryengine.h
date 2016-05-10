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
#ifndef __QUERYENGINE_H
#define __QUERYENGINE_H


#define T_COLON           0x01        /* ':' */
#define T_SLASH           0x02        /* '/' */
#define T_QUESTION        0x04        /* '?' */
#define T_HASH            0x08        /* '#' */
#define T_SPACE           0x10        /* ' ' */
#define T_EQUALS          0x20        /* '=' */
#define T_AMP             0x40        /* '&' */
#define T_NUL             0x80        /* '\0' */

static const unsigned char uri_delims[256] = {
    T_NUL,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,T_SPACE,0,0,T_HASH,0,0,T_AMP,0,
    0,0,0,0,0,0,0,T_SLASH,0,0,0,0,0,0,0,0,0,0,T_COLON,0,
    0,T_EQUALS,0,T_QUESTION,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 
};

#define NOTEND_SLASH    (T_SLASH | T_NUL)
#define NOTEND_PATH     (T_QUESTION | T_SPACE | T_NUL)
#define NOTEND_EQUAL    (T_EQUALS | T_SPACE | T_NUL)
#define NOTEND_AMP      (T_AMP | T_SPACE | T_NUL)



//--------------------------------------------------
// Class Definition
//--------------------------------------------------
class SocketWriter;
class Query;

class QueryEngine
{
public:
    static QueryEngine* Get();

    bool ParseAndExecute(char *pRequest, SocketWriter *pWriter);

private:
    QueryEngine();
    ~QueryEngine();

    static QueryEngine *mpQueryEngine;

    bool CreateQuery(char *pRequest, Query &iQuery);
    void ExecuteQuery(Query &iQuery, SocketWriter *pWriter);
};


#endif //__QUERYENGINE_H