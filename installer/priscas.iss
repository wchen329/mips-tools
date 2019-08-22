; Priscas Setup Script
; Author: wchen329@wisc.edu
;

[Setup]
AppName=Priscas
AppVerName=Priscas
AppVersion=0.3
OutputBaseFilename="priscas_setup"
VersionInfoProductName="Priscas Setup"
VersionInfoDescription="Installs the Priscas computer architecture simulator."
VersionInfoVersion=0.3.9.552
VersionInfoTextVersion="0.3.9.552"
VersionInfoProductVersion=0.3.9.552
VersionInfoProductTextVersion="0.3.9.552"
DefaultDirName={autopf}\Priscas
DefaultGroupName=Priscas
DisableWelcomePage=no
LicenseFile=../LICENSE.txt

[Tasks]
Name: shelldesktopicon; Description: "Create a desktop icon for Priscas Shell";
Name: simuidesktopicon; Description: "Create a desktop icon for Priscas simUI";

[Icons]
Name: "{group}\Priscas simUI"; Filename: "{app}\simUI.exe"
Name: "{group}\Priscas Shell"; Filename: "{app}\pshell.exe"
Name: "{group}\Uninstall"; Filename: "{uninstallexe}"
Name: "{commondesktop}\Priscas simUI"; Filename: "{app}\simUI.exe"; Tasks: simuidesktopicon
Name: "{commondesktop}\Priscas Shell"; Filename: "{app}\pshell.exe"; Tasks: shelldesktopicon

[Files]
Source: "..\external\lib\winNT\qt\Qt5Core.dll"; DestDir: "{app}"
Source: "..\external\lib\winNT\qt\Qt5Gui.dll"; DestDir: "{app}"
Source: "..\external\lib\winNT\qt\Qt5Widgets.dll"; DestDir: "{app}"
Source: "..\external\lib\winNT\qt\libEGL.dll"; DestDir: "{app}"
Source: "..\external\lib\winNT\qt\libGLESv2.dll"; DestDir: "{app}"
Source: "..\external\lib\winNT\qt\icudt52.dll"; DestDir: "{app}"
Source: "..\external\lib\winNT\qt\icuin52.dll"; DestDir: "{app}"
Source: "..\external\lib\winNT\qt\icuuc52.dll"; DestDir: "{app}"
Source: "..\external\lib\winNT\qt\qt.conf"; DestDir: "{app}"
Source: "..\external\licenses\LGPL_EXCEPTION.txt"; DestDir: "{app}\qt-licenses"
Source: "..\external\licenses\LICENSE.FDL"; DestDir: "{app}\qt-licenses"
Source: "..\external\licenses\LICENSE.GPL"; DestDir: "{app}\qt-licenses"
Source: "..\external\licenses\LICENSE.LGPL"; DestDir: "{app}\qt-licenses"
Source: "..\external\licenses\ThirdPartySoftware_Listing.txt"; DestDir: "{app}\qt-licenses"
Source: "..\external\lib\winNT\qt\plugins\platforms\qwindows.dll"; DestDir: "{app}\plugins\platforms\"
Source: "..\external\lib\winNT\qt\plugins\imageformats\qico.dll"; DestDir: "{app}\plugins\imageformats\"
Source: "..\PRISCAS\Release\pshell.exe"; DestDir: "{app}"
Source: "..\build-simUI-Desktop_Qt_5_3_MSVC2010_OpenGL_32bit-Release\release\simUI.exe"; DestDir: "{app}"

