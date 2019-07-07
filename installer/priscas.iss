; Priscas Setup Script
; Author: wchen329@wisc.edu
;

[Setup]
AppName=Priscas
AppVersion=0.2
OutputBaseFilename="priscas_setup"
VersionInfoOriginalFileName="priscas_setup.exe"
VersionInfoProductName="Priscas Setup"
VersionInfoDescription="Installs the Priscas computer architecture simulator."
VersionInfoVersion=0.2.2019.7
VersionInfoTextVersion="0.2.2019.7"
VersionInfoProductVersion=0.2.2019.7
VersionInfoProductTextVersion="0.2.2019.7"
DefaultDirName={autopf}\Priscas
DefaultGroupName=Priscas

[Components]
Name: "shell"; Description: "Priscas Shell"
Name: "simUI"; Description: "Priscas simUI"

[Files]
Source: "..\external\lib\winNT\qt\Qt5Core.dll"; DestDir: "{app}"; Components: simUI
Source: "..\external\lib\winNT\qt\Qt5Gui.dll"; DestDir: "{app}"; Components: simUI
Source: "..\external\lib\winNT\qt\Qt5Widgets.dll"; DestDir: "{app}"; Components: simUI
Source: "..\external\lib\winNT\qt\libEGL.dll"; DestDir: "{app}"; Components: simUI
Source: "..\external\lib\winNT\qt\libGLESv2.dll"; DestDir: "{app}"; Components: simUI
Source: "..\external\lib\winNT\qt\icudt52.dll"; DestDir: "{app}"; Components: simUI
Source: "..\external\lib\winNT\qt\icuin52.dll"; DestDir: "{app}"; Components: simUI
Source: "..\external\lib\winNT\qt\icuuc52.dll"; DestDir: "{app}"; Components: simUI
Source: "..\external\lib\winNT\qt\qt.conf"; DestDir: "{app}"; Components: simUI
Source: "..\external\licenses\LGPL_EXCEPTION.txt"; DestDir: "{app}\qt-licenses"; Components: simUI
Source: "..\external\licenses\LICENSE.FDL"; DestDir: "{app}\qt-licenses"; Components: simUI
Source: "..\external\licenses\LICENSE.GPL"; DestDir: "{app}\qt-licenses"; Components: simUI
Source: "..\external\licenses\LICENSE.LGPL"; DestDir: "{app}\qt-licenses"; Components: simUI
Source: "..\external\licenses\ThirdPartySoftware_Listing.txt"; DestDir: "{app}\qt-licenses"; Components: simUI
Source: "..\external\lib\winNT\qt\plugins\platforms\qwindows.dll"; DestDir: "{app}\plugins\platforms\"; Components: simUI
Source: "..\PRISCAS\Release\PRISCAS_Shell.exe"; DestDir: "{app}"; Components: simUI shell
Source: "..\build-simUI-Desktop_Qt_5_3_MSVC2010_OpenGL_32bit-Release\release\simUI.exe"; DestDir: "{app}"; Components: simUI;

