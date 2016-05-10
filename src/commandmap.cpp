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

#include "commandmap.h"
#include "commands.h"
#include "query.h"
#include "socketwriter.h"


//--------------------------------------------------
// Command/Function map
//--------------------------------------------------
const char *gArgValidate_PasswordA[] 	= {"pass"};
const char *gArgPlay[]               	= {"p"};
const char *gArgStop[]               	= {"p"};
const char *gArgPause[]              	= {"p"};
const char *gArgNextTrack[]          	= {"p"};
const char *gArgPreviousTrack[]      	= {"p"};
const char *gArgFadeoutAndStop[]     	= {"p"};
const char *gArgGetVersion[]         	= {"p"};
const char *gArgDelete[]             	= {"p"};
const char *gArgIsPlaying[]          	= {"p"};
const char *gArgGetListLength[]      	= {"p"};
const char *gArgGetListPos[]         	= {"p"};
const char *gArgGetPlayListPos[]     	= {"p"};
const char *gArgShuffle_Status[]     	= {"p"};
const char *gArgRepeat_Status[]      	= {"p"};
const char *gArgVolumeUp[]           	= {"p"};
const char *gArgVolumeDown[]         	= {"p"};
const char *gArgFlushPlayList[]      	= {"p"};
const char *gArgGetCurrentTitle[]    	= {"p"};
const char *gArgUpdateCurrentTitle[] 	= {"p"};
const char *gArgInternet[]           	= {"p"};
const char *gArgRestart[]            	= {"p"};
const char *gArgGetAutoService[]     	= {"p"};
const char *gArgShoutCast_Connect[]  	= {"p"};
const char *gArgShoutCast_Status[]   	= {"p"};
const char *gArgExec_Visual[]        	= {"p"};
const char *gArgGetVolume[]          	= {"p"};
const char *gArgGetPlayListTitleList[] 	= {"p", "delim"};
const char *gArgGetPlayListTitle[]  	= {"p"};
const char *gArgGetPlayListTitleA[]  	= {"p", "index"};
const char *gArgGetPlayListFileList[]   = {"p", "delim"};
const char *gArgGetPlayListFile[]   	= {"p"};
const char *gArgGetPlayListFileA[]   	= {"p", "index"};
const char *gArgDeletePosA[]         	= {"p", "index"};
const char *gArgSwapPosA[]           	= {"p", "from", "to"};
const char *gArgGetOutputTimeA[]     	= {"p", "frmt"};
const char *gArgJumpToTimeA[]        	= {"p", "ms"};
const char *gArgSetPlayListPosA[]    	= {"p", "index"};
const char *gArgChDirA[]             	= {"p", "dir"};
const char *gArgPlayFileA[]          	= {"p", "file"};
const char *gArgEnqueueFileA[]          = {"p", "title", "file"};
const char *gArgPlayUrlA[]          	= {"p", "url"};
const char *gArgEnqueueUrlA[]           = {"p", "title", "url"};
const char *gArgGetInfoA[]           	= {"p", "info"};
const char *gArgShuffleA[]           	= {"p", "enable"};
const char *gArgRepeatA[]            	= {"p", "enable"};
const char *gArgSetVolumeA[]         	= {"p", "level"};
const char *gArgGetEqDatumA[]        	= {"p", "delim"};
const char *gArgSetEqDatumA[]        	= {"p", "levels"};
const char *gArgGetEqDataA[]         	= {"p", "band"};
const char *gArgSetEqDataA[]         	= {"p", "band", "level"};
const char *gArgSetAutoServiceA[]    	= {"p", "enable"};
const char *gArgHasId3Tag[]			 	= {"p"};
const char *gArgHasId3TagA[]		 	= {"p", "index"};
const char *gArgGetId3Tag[]          	= {"p", "tags", "delim"};
const char *gArgGetId3TagA[]         	= {"p", "tags", "delim", "index"};
const char *gArgHasId3v2Tag[]		 	= {"p"};
const char *gArgHasId3v2TagA[]		 	= {"p", "index"};
const char *gArgGetId3v2Tag[]        	= {"p", "tags", "delim"};
const char *gArgGetId3v2TagA[]       	= {"p", "tags", "delim", "index"};
const char *gArgGetMpegInfo[]        	= {"p", "info", "delim"}; 
const char *gArgGetMpegInfoA[]       	= {"p", "info", "delim", "index"}; 
const char *gArgGetHash[]			 	= {"p"}; 
const char *gArgGetHashA[]			 	= {"p", "index"}; 
const char *gArgGetDirty[]			 	= {"p"}; 
const char *gArgSetClean[]			 	= {"p"};
const char *gArgPlaySong[]				= {"p", "song"};
const char *gArgPickSong[]				= {"p", "song"};



const CommandMapItem gCommandMap[] =
{
    //  cmd, func, arg_count, arg_list

    {"",                    	Root,               	0,  NULL},
    {"validate_password",   	Validate_PasswordA, 	1,  gArgValidate_PasswordA},
    {"play",                	Play,               	1,  gArgPlay},
    {"stop",                	Stop,               	1,  gArgStop},
    {"pause",               	Pause,              	1,  gArgPause},
    {"next",                	NextTrack,          	1,  gArgNextTrack},
    {"prev",                	PreviousTrack,      	1,  gArgPreviousTrack},
    {"fadeoutandstop",      	FadeoutAndStop,     	1,  gArgFadeoutAndStop},
    {"getversion",          	GetVersion,         	1,  gArgGetVersion},
    {"delete",              	Delete,             	1,  gArgDelete},
    {"isplaying",           	IsPlaying,          	1,  gArgIsPlaying},
    {"getlistlength",       	GetListLength,      	1,  gArgGetListLength},
    {"getlistpos",          	GetListPos,         	1,  gArgGetListPos},
    {"getplaylistpos",      	GetPlayListPos,     	1,  gArgGetPlayListPos},
    {"shuffle_status",      	Shuffle_Status,     	1,  gArgShuffle_Status},
    {"repeat_status",       	Repeat_Status,      	1,  gArgRepeat_Status},
    {"volumeup",            	VolumeUp,           	1,  gArgVolumeUp},
    {"volumedown",          	VolumeDown,         	1,  gArgVolumeDown},
    {"flushplaylist",       	FlushPlayList,      	1,  gArgFlushPlayList},
    {"getcurrenttitle",     	GetCurrentTitle,    	1,  gArgGetCurrentTitle},
    {"updatecurrenttitle",  	UpdateCurrentTitle, 	1,  gArgUpdateCurrentTitle},
    {"internet",            	Internet,           	1,  gArgInternet},
    {"restart",             	Restart,            	1,  gArgRestart},
    {"getautoservice",      	GetAutoService,     	1,  gArgGetAutoService},
    {"shoutcast_connect",   	ShoutCast_Connect,  	1,  gArgShoutCast_Connect},
    {"shoutcast_status",    	ShoutCast_Status,   	1,  gArgShoutCast_Status},
    {"exec_visual",         	Exec_Visual,        	1,  gArgExec_Visual},
    {"getvolume",           	GetVolume,          	1,  gArgGetVolume},
    {"getplaylisttitlelist",    GetPlayListTitleList,   2,  gArgGetPlayListTitleList},
    {"getplaylisttitle",    	GetPlayListTitle,  		1,  gArgGetPlayListTitle},
    {"getplaylisttitle",    	GetPlayListTitleA,  	2,  gArgGetPlayListTitleA},
    {"getplaylistfilelist",     GetPlayListFileList,    2,  gArgGetPlayListFileList},
    {"getplaylistfile",     	GetPlayListFile,   		1,  gArgGetPlayListFile},
    {"getplaylistfile",     	GetPlayListFileA,   	2,  gArgGetPlayListFileA},
    {"deletepos",           	DeletePosA,         	2,  gArgDeletePosA},
    {"swappos",             	SwapPosA,           	3,  gArgSwapPosA},
    {"getoutputtime",       	GetOutputTimeA,     	2,  gArgGetOutputTimeA},
    {"jumptotime",          	JumpToTimeA,        	2,  gArgJumpToTimeA},
    {"setplaylistpos",      	SetPlayListPosA,    	2,  gArgSetPlayListPosA},
    {"chdir",               	ChDirA,             	2,  gArgChDirA},
    {"playfile",            	PlayFileA,          	2,  gArgPlayFileA},
    {"enqueuefile",            	EnqueueFileA,          	3,  gArgEnqueueFileA},
    {"playurl",            		PlayUrlA,          		2,  gArgPlayUrlA},
    {"enqueueurl",            	EnqueueUrlA,          	3,  gArgEnqueueUrlA},
    {"getinfo",             	GetInfoA,           	2,  gArgGetInfoA},
    {"shuffle",             	ShuffleA,           	2,  gArgShuffleA},
    {"repeat",              	RepeatA,            	2,  gArgRepeatA},
    {"setvolume",           	SetVolumeA,         	2,  gArgSetVolumeA},
    {"setautoservice",      	SetAutoServiceA,    	2,  gArgSetAutoServiceA},
    {"geteqdatum",          	GetEqDatumA,        	2,  gArgGetEqDatumA},
    {"seteqdatum",          	SetEqDatumA,        	2,  gArgSetEqDatumA},
    {"geteqdata",           	GetEqDataA,         	2,  gArgGetEqDataA},
    {"seteqdata",           	SetEqDataA,         	3,  gArgSetEqDataA},
    {"hasid3tag",           	HasId3Tag,          	1,  gArgHasId3Tag},
    {"hasid3tag",           	HasId3TagA,         	2,  gArgHasId3TagA},
    {"getid3tag",           	GetId3Tag,          	3,  gArgGetId3Tag},
    {"getid3tag",           	GetId3TagA,         	4,  gArgGetId3TagA},
    {"hasid3v2tag",         	HasId3v2Tag,        	1,  gArgHasId3v2Tag},
    {"hasid3v2tag",         	HasId3v2TagA,       	2,  gArgHasId3v2TagA},
    {"getid3v2tag",         	GetId3v2Tag,        	3,  gArgGetId3v2Tag},
    {"getid3v2tag",         	GetId3v2TagA,       	4,  gArgGetId3v2TagA},
    {"getmpeginfo",         	GetMpegInfo,        	3,  gArgGetMpegInfo},
    {"getmpeginfo",         	GetMpegInfoA,       	4,  gArgGetMpegInfoA},
    {"gethash",					GetHash,				1,  gArgGetHash},
    {"gethash",					GetHashA,				2,  gArgGetHashA},
    {"getdirty",				GetDirty,				1,  gArgGetDirty},
	{"setclean",				SetClean,				1,  gArgSetClean},
	{"playsong",				PlaySong,				2,  gArgPlay},
	{"picksong",				PickSong,				2,  gArgPickSong},
};


const CommandMapItem *FindFunction(Query &query)
{
    const CommandMapItem *pCommand = NULL;

    for(int i=0; i<ARRAYSIZE(gCommandMap); i++)
    {
        pCommand = &gCommandMap[i];
        
        // number of arguments does not match
        if (query.NumArguments() != pCommand->ArgCount)
            continue;

        // command names do not match
        if(stricmp(query.GetPath(), pCommand->Command) != 0)
            continue;

        // validate each argument exists
        bool err = false;
        for(unsigned int i =0; i < pCommand->ArgCount; i++)
        {
            const char *arg = pCommand->Args[i];
            if (!query.ContainsArgument(arg))
            {
                err = true;
                break;
            }
        }
        if (err)
            continue;

        // we have a match
        return pCommand;
    }

    return NULL;
}

