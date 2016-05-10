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
#ifndef __QUERY_H
#define __QUERY_H


//--------------------------------------------------
// Class Definition
//--------------------------------------------------
class QueryValuePair
{
public:
    QueryValuePair(const char *key, size_t nkey, const char *value, size_t nval);
    ~QueryValuePair();

    inline const char *GetKey() const { return mKey; };
    inline const char *GetValue() const { return mValue; };
    inline const char *GetValueRaw() const { return mValueRaw; };

private:
    char *mKey;
    char *mValue;
	char *mValueRaw;
};


//--------------------------------------------------
// Class Definition
//--------------------------------------------------
class Query
{
public:
    Query();
    ~Query();

    void SetPath(char *p, size_t n);
    bool SetArgument(char *key, size_t nkey, char *value, size_t nval);
    
    inline const char *GetPath() const { return mPath; };
    unsigned int NumArguments() const { return mValuePair.Count(); };
    const char *GetArgument(const char *key);
    const char *GetArgumentRaw(const char *key);
    bool ContainsArgument(const char *key);
    
protected:
	QueryValuePair* FindPair(const char *key);

private:
    char *mPath;
    Array<QueryValuePair *> mValuePair;
};


#endif //__QUERY_H