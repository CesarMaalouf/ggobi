; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
AppName=ggobi
AppVerName=ggobi_0.9
AppPublisher=ggobi
AppPublisherURL=http://www.ggobi.org
AppSupportURL=http://www.ggobi.org
AppUpdatesURL=http://www.ggobi.org
DefaultDirName={pf}\ggobi
DefaultGroupName=ggobi
AllowNoIcons=yes
AlwaysCreateUninstallIcon=yes
LicenseFile=C:\cygwin\home\duncan\Projects\ggobi\LICENSING\license.txt
; uncomment the following line if you want your installation to run on NT 3.51 too.
; MinVersion=4,3.51

[Tasks]
Name: "desktopicon"; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"; MinVersion: 4,4

[Files]
Source: "C:\cygwin\home\duncan\Projects\ggobi\ggobi.exe"; DestDir: "{app}"; CopyMode: alwaysoverwrite
Source: "C:\cygwin\home\duncan\libs\*.*"; DestDir: "{app}"; CopyMode: alwaysoverwrite
Source: "C:\cygwin\home\duncan\Projects\ggobi\data\*.*"; DestDir: "{app}"; CopyMode: alwaysoverwrite

[INI]
Filename: "{app}\ggobi.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.ggobi.org"

[Icons]
Name: "{group}\ggobi"; Filename: "{app}\ggobi.exe"
Name: "{group}\ggobi on the Web"; Filename: "{app}\ggobi.url"
Name: "{userdesktop}\ggobi"; Filename: "{app}\ggobi.exe"; MinVersion: 4,4; Tasks: desktopicon

[Run]
Filename: "{app}\ggobi.exe"; Parameters: "flea.dat"; Description: "Launch ggobi"; Flags: nowait postinstall skipifsilent runminimized

[UninstallDelete]
Type: files; Name: "{app}\ggobi.url"

