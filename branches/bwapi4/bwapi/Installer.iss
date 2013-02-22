;This is an INNO Setup script
;You should download INNO Setup and INNO Script Studio v2 to modify and compile it

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "..\Release_Binary\Starcraft\SNP_DirectIP.snp"; DestDir: "{code:BroodwarPath}\"; Flags: ignoreversion; Components: Binaries\BWAPI; Check: GetBroodwarPath
Source: "..\Release_Binary\Starcraft\bwapi-data\BWAPI.dll"; DestDir: "{code:BroodwarPath}\bwapi-data"; Flags: ignoreversion; Components: Binaries\BWAPI; Check: GetBroodwarPath
Source: "..\Release_Binary\Starcraft\bwapi-data\bwapi.ini"; DestDir: "{code:BroodwarPath}\bwapi-data\"; Flags: ignoreversion; Components: Binaries\BWAPI; Check: GetBroodwarPath
Source: "..\Release_Binary\Starcraft\bwapi-data\BWAPId.dll"; DestDir: "{code:BroodwarPath}\bwapi-data\"; Flags: ignoreversion; Components: Binaries\BWAPI; Check: GetBroodwarPath
Source: "..\Release_Binary\Starcraft\bwapi-data\Multiple Instance Hack.bat"; DestDir: "{code:BroodwarPath}\bwapi-data\"; Flags: ignoreversion; Components: Binaries\BWAPI; Check: GetBroodwarPath
Source: "..\Release_Binary\Starcraft\bwapi-data\data\Broodwar.map"; DestDir: "{code:BroodwarPath}\bwapi-data\data\"; Flags: ignoreversion; Components: Binaries\BWAPI; Check: GetBroodwarPath
Source: "..\Release_Binary\Starcraft\bwapi-data\data\starcraft.bsdiff"; DestDir: "{code:BroodwarPath}\bwapi-data\data\"; Flags: ignoreversion; Components: Binaries\BWAPI; Check: GetBroodwarPath
Source: "..\Release_Binary\ChangeLog.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Release_Binary\COPYING"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Release_Binary\README"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Release_Binary\vcredist_x86.exe"; DestDir: "{app}"; Flags: ignoreversion deleteafterinstall
Source: "..\Release_Binary\include\*"; DestDir: "{app}\include\"; Flags: ignoreversion createallsubdirs recursesubdirs; Components: Library\Headers
Source: "..\Release_Binary\lib\*"; DestDir: "{app}\lib\"; Flags: ignoreversion createallsubdirs recursesubdirs; Components: Library\Headers
Source: "..\Release_Binary\Chaoslauncher\*"; DestDir: "{app}\Chaoslauncher\"; Flags: ignoreversion createallsubdirs recursesubdirs; Components: Chaoslauncher
Source: "..\Release_Binary\MPQdraft\*"; DestDir: "{app}\MPQDraft\"; Flags: ignoreversion createallsubdirs recursesubdirs; Components: MPQDraft
Source: "..\Release_Binary\AIModuleLoader\*"; DestDir: "{app}\AIModuleLoader\"; Flags: ignoreversion createallsubdirs recursesubdirs; Components: Library\Examples
Source: "..\Release_Binary\ExampleAIClient\*"; DestDir: "{app}\ExampleAIClient\"; Flags: ignoreversion createallsubdirs recursesubdirs; Components: Library\Examples
Source: "..\Release_Binary\ExampleAIModule\*"; DestDir: "{app}\ExampleAIModule\"; Flags: ignoreversion createallsubdirs recursesubdirs; Components: Library\Examples
Source: "..\Release_Binary\ExampleTournamentModule\*"; DestDir: "{app}\ExampleTournamentModule\"; Flags: ignoreversion createallsubdirs recursesubdirs; Components: Library\Examples
Source: "..\Release_Binary\ExampleProjects.sln"; DestDir: "{app}"; Flags: ignoreversion; Components: Library\Examples
Source: "..\Release_Binary\Starcraft\bwapi-data\data\bspatch.exe"; DestDir: "{code:BroodwarPath}\bwapi-data\data\"; Flags: ignoreversion; Components: Binaries\BWAPI; Check: GetBroodwarPath

[Setup]
AppName=BWAPI
AppId={{5025BB16-9672-4C23-979D-2DC1B276CC5E}
LicenseFile=C:\Projects\BWAPI\branches\bwapi4\Release_Binary\COPYING
InfoBeforeFile=C:\Projects\BWAPI\branches\bwapi4\Release_Binary\ChangeLog.txt
RestartIfNeededByRun=False
AppPublisherURL=http://bwapi.googlecode.com
AppSupportURL=http://bwapi.googlecode.com
AppUpdatesURL=http://bwapi.googlecode.com
VersionInfoDescription=Brood War Application Programming Interface
VersionInfoProductName=BWAPI
MinVersion=0,5.01
DefaultDirName={pf}\BWAPI
UsePreviousSetupType=False
UsePreviousTasks=False
UsePreviousLanguage=False
ShowTasksTreeLines=True
AlwaysShowGroupOnReadyPage=True
AlwaysShowDirOnReadyPage=True
UsePreviousGroup=False
Uninstallable=no
FlatComponentsList=False
WizardSmallImageFile=instSmall.bmp
SetupIconFile=instIcon.ico
WizardImageFile=instLarge.bmp
SolidCompression=True
Compression=lzma2/ultra
InternalCompressLevel=ultra
DisableProgramGroupPage=yes
AppVersion=4.0.0 Beta
ShowLanguageDialog=no
AlwaysShowComponentsList=False
ShowComponentSizes=False

[Components]
Name: "Binaries"; Description: "Binaries"
Name: "Binaries\BWAPI"; Description: "BWAPI Binaries (Requires Starcraft: Broodwar)"; Types: full custom; Check: GetBroodwarPath
Name: "Binaries\vcredist"; Description: "Microsoft Visual C++ 2010 Redistributable"; Types: custom full
Name: "Library"; Description: "Interface"; Types: custom full
Name: "Library\Headers"; Description: "Library & Header Files"; Types: custom full
Name: "Library\Examples"; Description: "Example Projects"; Types: custom full
Name: "Chaoslauncher"; Description: "Chaoslauncher (Plugin Loader) & Plugins"; Types: full custom
Name: "MPQDraft"; Description: "MPQDraft (Mod Manager & Plugin Loader)"; Types: custom full

[Messages]
WelcomeLabel2=This will install [name/ver] on your computer.

[Run]
Filename: "{app}\Chaoslauncher\Chaoslauncher.exe"; WorkingDir: "{app}\Chaoslauncher\"; Flags: nowait postinstall skipifdoesntexist; Description: "Run Chaoslauncher"; Components: Chaoslauncher
Filename: "{app}\MPQdraft\MPQDraft.exe"; WorkingDir: "{app}\MPQdraft\"; Flags: nowait postinstall unchecked skipifdoesntexist; Description: "Run MPQDraft"; Components: MPQDraft
Filename: "{app}\vcredist_x86.exe"; Parameters: "-quiet"; WorkingDir: "{app}"; Description: "Visual C++ 2010 Redistributable"; Components: Binaries\vcredist

[ThirdParty]
UseRelativePaths=True

[Types]
Name: "full"; Description: "Full Installation"
Name: "custom"; Description: "Custom Installation"; Flags: iscustom

[Registry]
Root: "HKCU"; Subkey: "Software\Battle.net\Configuration"; ValueType: multisz; ValueName: "Battle.net gateways"; ValueData: "{code:InstallGateway|sc.theabyss.ru;+7;The Abyss (ICCUP);}"; Tasks: ICCUP
Root: "HKCU"; Subkey: "Software\Battle.net\Configuration"; ValueType: multisz; ValueName: "Battle.net gateways"; ValueData: "{code:InstallGateway|games.podolsk.ru;+8;Games Podolsk;}"; Tasks: Podolsk

[Tasks]
Name: "ICCUP"; Description: "The Abyss (ICCUP)"; GroupDescription: "Install Additional Battle.net Gateways:"
Name: "Podolsk"; Description: "Games Podolsk"; GroupDescription: "Install Additional Battle.net Gateways:"

[Code]
var sBroodwarPath : String;

// Get the location of the Starcraft folder.
function GetBroodwarPath(): Boolean;
var
  i:      Integer;
  len:    Integer;

begin
  sBroodwarPath := '';

  // Search for InstallPath
  RegQueryStringValue( HKCU, 'SOFTWARE\Blizzard Entertainment\Starcraft', 'InstallPath', sBroodwarPath );
  len := Length(sBroodwarPath);

  // Search in alternative location
  if len = 0 then
  begin
    RegQueryStringValue( HKLM, 'SOFTWARE\Blizzard Entertainment\Starcraft', 'InstallPath', sBroodwarPath );
    len := Length(sBroodwarPath);
  end

  // If it was not found
  if len = 0 then
  begin
    // @TODO: Ask user for the install path
  end

  Result := len > 0;
end;

//  Returns the path
function BroodwarPath(Param: String) : String;
begin
   Result := sBroodwarPath;
end;

// Returns the Nth string (0-based index) given a null-terminated set of strings
function GetStringAt(S: String; N: Integer) : String;
var
  idx : Integer;
  sCurrent : String;
begin
  // init
  idx := 0;
  sCurrent := Copy(S, 1, Length(S));

  // algo
  while N > 0 do
  begin
    N := N - 1;
    idx := Pos(#0,sCurrent);
    idx := idx + 1;

    sCurrent := Copy(sCurrent, idx, Length(sCurrent));
  end

  idx := Pos(#0,sCurrent);
  Result := Copy(sCurrent, 1, idx);
end;



// Install the ICCUP gateway
function InstallGateway(Param: String) : String;
var
  sDefaultGateways, sGateways : String;
  sTmp : String;
  idx : Integer;
begin
  // Parse parameters
  StringChangeEx(Param, ';', #0, True);
  
  // Assign defaults
  sDefaultGateways := '1000'#0'01'#0'uswest.battle.net'#0'8'#0'U.S. West'#0'useast.battle.net'#0'6'#0'U.S. East'#0'asia.battle.net'#0'-9'#0'Asia'#0'europe.battle.net'#0'-1'#0'Europe'#0;
  sGateways := sDefaultGateways;

  // Retrieve the existing gateways string (if any)
  RegQueryMultiStringValue( HKCU, 'SOFTWARE\Battle.net\Configuration', 'Battle.net gateways', sGateways );

  // Check validity of first component
  sTmp := GetStringAt(sGateways, 0);
  if ( Length(sTmp) = 0 ) then  //  (validity not complete, but should be good)
    sGateways := sDefaultGateways;

  // Check validity of second component
  sTmp := GetStringAt(sGateways, 1);
  if ( Length(sTmp) = 0 ) then  //  (validity not complete, but should be good)
    sGateways := sDefaultGateways;

  // Search for existing entry
  idx := 2;
  repeat
    sTmp := GetStringAt(sGateways, idx);
    idx := idx + 3;
    //MsgBox(sTmp, mbInformation, MB_OK);
  until (CompareText(Trim(sTmp), Trim(GetStringAt(Param,0))) = 0) OR (Length(sTmp) = 0);

  // If desired server was not found
  if ( Length(sTmp) = 0 ) then
  begin
    // Append it to the existing gateways
    Insert(Param, sGateways, Length(sGateways)+1);
  end

  Result := sGateways;
end;
