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
#ifndef __COMMANDMAP_H
#define __COMMANDMAP_H


//--------------------------------------------------
// Forward Declarations
//--------------------------------------------------
class Query;
class SocketWriter;


//--------------------------------------------------
// Type definitions
//--------------------------------------------------
typedef void (*CommandFunction)(Query &query, SocketWriter *pWriter);

typedef struct CommandMapItem
{
    const char *Command;        // name of the command
    CommandFunction Function;   // point to actual function
    unsigned int ArgCount;      // number of arguments
    const char **Args;          // argument list

} CommandMapItem;


//--------------------------------------------------
// Command/Function map
//--------------------------------------------------
extern const CommandMapItem *FindFunction(Query &query);



#endif // __COMMANDMAP_H