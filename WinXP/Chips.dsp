# Microsoft Developer Studio Project File - Name="Chips" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Chips - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Chips.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Chips.mak" CFG="Chips - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Chips - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Chips - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Chips - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "." /I "..\Common" /I "..\..\GapiDraw\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 gd104.lib jojosip.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /libpath:"..\..\GapiDraw\lib\xp"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "Chips - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "." /I "..\Common" /I "..\..\GapiDraw\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 gd104.lib jojosip.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc" /pdbtype:sept /libpath:"..\..\GapiDraw\lib\xp"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Chips - Win32 Release"
# Name "Chips - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\Common\Block.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\Chips.cpp
# End Source File
# Begin Source File

SOURCE=.\Chips.rc
# End Source File
# Begin Source File

SOURCE=..\Common\Creature.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\GameState.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\Level.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Common\Block.h
# End Source File
# Begin Source File

SOURCE=..\Common\Cell.h
# End Source File
# Begin Source File

SOURCE=..\Common\Chips.h
# End Source File
# Begin Source File

SOURCE=..\Common\Creature.h
# End Source File
# Begin Source File

SOURCE=..\Common\Defs.h
# End Source File
# Begin Source File

SOURCE=..\Common\Device.h
# End Source File
# Begin Source File

SOURCE=..\Common\Element.h
# End Source File
# Begin Source File

SOURCE=..\Common\GameState.h
# End Source File
# Begin Source File

SOURCE=..\..\GapiDraw\include\GapiDraw.h
# End Source File
# Begin Source File

SOURCE=..\..\GapiDraw\include\GapiDrawApplication.h
# End Source File
# Begin Source File

SOURCE=..\..\GapiDraw\include\GapiDrawExtension.h
# End Source File
# Begin Source File

SOURCE=..\..\GapiDraw\include\JoJoSIP.h
# End Source File
# Begin Source File

SOURCE=..\Common\Level.h
# End Source File
# Begin Source File

SOURCE=..\Common\Slip.h
# End Source File
# Begin Source File

SOURCE=..\Common\Tile.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\200.BMP
# End Source File
# Begin Source File

SOURCE=.\BACKGROUND.bmp
# End Source File
# Begin Source File

SOURCE=.\Chips.ico
# End Source File
# Begin Source File

SOURCE=.\INFOWND.BMP
# End Source File
# Begin Source File

SOURCE=.\keyboard.bmp
# End Source File
# Begin Source File

SOURCE=.\mask_keyboard.bmp
# End Source File
# Begin Source File

SOURCE=.\OBJ24_4.bmp
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\tileback.bmp
# End Source File
# Begin Source File

SOURCE=.\Title.bmp
# End Source File
# End Group
# End Target
# End Project
