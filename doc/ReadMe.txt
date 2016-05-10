=====================================================

	Winamp httpq Plugin 
	
	http://httpq.sourceforge.net/
	

	Version:	Version 3.1
	Author:		Kosta Arvanitis
	Contact:	karvanitis@hotmail.com

		

                        (c) 1999-2003 Kosta Arvanitis		
=====================================================


1.	Introduction
2.	License
3.	Install
4.	Config
5.	HOW-TO
6.	Commands
7. 	Updates
	








1. Introduction
===============

	Httpq allows users to send commands to their winamp player over http. 
	
	Httpq is intended as a backend for a web based control of winamp. Together with
	web tools such as PERL or PHP any user could easily put together a web based control
	for their winamp player. Shoutcast admin may find this tool particularly useful.

	Feel free to contact the author about any bugs or improvment request.


2. License
==========

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
	
		

3. Install
==========

	Run the install program.
	
	The install program will copy the file gen_httpq.dll into your winamp plugins 
	directory. For  example "c:\program files\winamp\plugins". You may need to restart 
	winamp if already runnig.



4. Config
=========

	Before you can properly use it you must configure httpq. To configure httpq goto
	the winamp preferences -> Plug-ins -> General Purpose 
	select Winamp Httpq Plugin and click configure.

	
	o	Port: 4800 (default)

		This is the port that httpq will be listening on. Make sure that this port is
		not already in use. 

	o	Password: pass (default)

		This is the password that is sent along with every http request. The default is "pass"
		so be sure to change it.

	o	Start Service Automatically : checked (default)

		This tells httpq whether or not to start automatically when winamp starts. 
		If checked it will start when winamp is loaded. If not it will not start until
		you click start.

	o	Start 
		
		Start the service.

	o	Stop 

		Stop the service.

	o	Apply

		Save the changes you have made. If you are making a change to the port you will have to 
		start and stop the service.



5. HOW-TO
=========

	Once youve got httpq configured and running you can use any web browser to send it 
	commands.

	The basic format for sending messages to httpq is:

		http://localhost:4800/command?p=password&a=attribute

			o localhost is the IP of you machine
			o 4800 is the port
			o command is one of many commands (see 6)
			o p is your password (required)
			o a is a command attribute (not always required)
	


6. Commands
===========

	The listing of commands is now only available online at 

	http://httpq.sourceforge.net/
	


7. Updates & Bug Fixes
======================

	Version 3.1
	===========
	
		- New commands

			o GETEQDATUM
			o SETEQDATUM
			o HASID3TAG
			o HASID3V2TAG
			o GETID3TAG
			o GETID3V2TAG
			o GETMPEGINFO
			o GETHASH
			o GETDIRTY
			o SETCLEAN
			o SWAPPOS
			
		- Added support for ID3v2 Tags.
		- Removed unnecessary delimiters.
		- Fixed bug where queries we being converted to lowercase.
			

	Version 3.0
	===========
	
		- First open source release
		- Updated gui
		- Updated command interface


	Version 2.1
	===========

		- New Features

			o Advanced Settings
			o Help feature for UI
			o HTTP headers include server tag
		
		- New commands
			
			o DELETEPOS
			o GETMPEGINFO
			o GETMPEGINFO_FRAMEPOS
			o GETMPEGINFO_FILELENGTH
			o GETMPEGINFO_TRACKLENGTH
			o GETMPEGINFO_VERSION
			o GETMPEGINFO_LAYER
			o GETMPEGINFO_BITRATE
			o GETMPEGINFO_FREQUENCY
			o GETMPEGINFO_MODE
			o GETMPEGINFO_CRC
			o GETMPEGINFO_COPYRIGHT
			o GETMPEGINFO_ORIGINAL
			o GETMPEGINFO_EMPHASIS
			o EXEC_VISUAL


	Version 2.0
	===========

		- Additional user defined options.
			- Allow user to disable http protocol headers
			in return data

		- Improved network reliability and performance


	Version 1.9
	===========

		o	GETID3TAG_ALBUM
		- Fixed small bug in id3tag support
		- Updated ui, by removing edit button from security tab.
		- Created new logo, added to about tab
		- Minor code improvments

	Version 1.8
	===========

		o	VALIDATE_PASSWORD

		o	GETID3TAG_SONGNAME
		o	GETID3TAG_ARTIST
		o	GETID3TAG_YEAR
		o	GETID3TAG_GENRE
		o	GETID3TAG_COMMENT
		
		o	FLUSHPLAYLIST
		o	GETCURRENTTITLE	
		o	UPDATECURRENTTITLE
		o	INTERNET

		o	FADEOUTANDSTOP

		o	RESTART
		o	GETAUTOSERVICE
		o	SETAUTOSERVICE

	Version 1.7
	===========

		o	SETVOLUME
		o	GETEQDATA
		o	SETEQDATA
		o	GETID3TAG

		- Return values have been changed to simply return 0 on error,
		and 1 on success unless otherwise notted.


	Version 1.6
	===========
	
		o	VOLUMEUP
		o	VOLUMEDOWN
		o	SHUFFLESTATUS
		o	PLAYLIST


	Version 1.3
	===========

		o	Added two new functions.
				- SHUFFLE
				- REPEAT

		o	Updated functions
				- PLAYFILE
				- ISPLAYING
			