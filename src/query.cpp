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

#include "query.h"
#include "util.h"


//--------------------------------------------------
// Construction/Destruction
//--------------------------------------------------
QueryValuePair::QueryValuePair(const char *key, size_t nkey, const char *value, size_t nval)
{
   mKey = strmemdup(key, nkey);
   mValue = strmemdup(value, nval);
   mValueRaw = strmemdup(value, nval);

   urldecode(mKey);
   urldecode(mValue);
}

QueryValuePair::~QueryValuePair()
{
    strmemfree(mKey);
    strmemfree(mValue);
}



//--------------------------------------------------
// Construction/Destruction
//--------------------------------------------------
Query::Query()
{
    mPath = NULL;
}

Query::~Query()
{
    strmemfree(mPath);

    for(unsigned int i=0; i<mValuePair.Count(); i++) 
    {
        if (mValuePair[i] != NULL)
            delete mValuePair[i];
    }
    mValuePair.Clear();
}

void Query::SetPath(char *p, size_t n)
{
    mPath = strmemdup(p, n);

    urldecode(mPath);
}

bool Query::SetArgument(char *key, size_t nkey, char *value, size_t nval)
{
    // check for size limit
    if (mValuePair.Count() >= mValuePair.MaxSize())
        return false;
    
    // check for existing key
	if (ContainsArgument(key))
		return false;

    // create new value pair
    mValuePair.Add(new QueryValuePair(key, nkey, value, nval));

    return true;
}

QueryValuePair* Query::FindPair(const char *key)
{
    for(unsigned int i=0; i<mValuePair.Count(); i++)
    {
        QueryValuePair *pair = mValuePair[i];
        if (pair != NULL)
        {
            if (stricmp(key, pair->GetKey()) == 0)
                return pair;
        }
    }
    return NULL;
}

const char *Query::GetArgument(const char *key)
{
	QueryValuePair *pair = FindPair(key);
	if (pair)
		return pair->GetValue();
    return NULL;
}

const char *Query::GetArgumentRaw(const char *key)
{
	QueryValuePair *pair = FindPair(key);
	if (pair)
		return pair->GetValueRaw();
    return NULL;
}

bool Query::ContainsArgument(const char *key)
{
    QueryValuePair *pair = FindPair(key);
    return (pair != NULL);
}

