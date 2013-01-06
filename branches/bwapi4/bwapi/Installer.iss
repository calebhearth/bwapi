;This is an INNO Setup script
;You should download INNO Setup and INNO Script Studio v2 to modify and compile it

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "..\Release_Binary\Starcraft\SNP_DirectIP.snp"; DestDir: "{code:BroodwarPath}\"; Flags: ignoreversion; Components: Binaries\BWAPI ; Check: GetBroodwarPath
Source: "..\Release_Binary\Starcraft\bwapi-data\BWAPI.dll"; DestDir: "{code:BroodwarPath}\bwapi-data"; Flags: ignoreversion; Components: Binaries\BWAPI ; Check: GetBroodwarPath
Source: "..\Release_Binary\Starcraft\bwapi-data\bwapi.ini"; DestDir: "{code:BroodwarPath}\bwapi-data\"; Flags: ignoreversion; Components: Binaries\BWAPI ; Check: GetBroodwarPath
Source: "..\Release_Binary\Starcraft\bwapi-data\BWAPId.dll"; DestDir: "{code:BroodwarPath}\bwapi-data\"; Flags: ignoreversion; Components: Binaries\BWAPI ; Check: GetBroodwarPath
Source: "..\Release_Binary\Starcraft\bwapi-data\Multiple Instance Hack.bat"; DestDir: "{code:BroodwarPath}\bwapi-data\"; Flags: ignoreversion; Components: Binaries\BWAPI ; Check: GetBroodwarPath
Source: "..\Release_Binary\Starcraft\bwapi-data\data\Broodwar.map"; DestDir: "{code:BroodwarPath}\bwapi-data\data\"; Flags: ignoreversion; Components: Binaries\BWAPI ; Check: GetBroodwarPath
Source: "..\Release_Binary\Starcraft\bwapi-data\data\starcraft.bsdiff"; DestDir: "{code:BroodwarPath}\bwapi-data\data\"; Flags: ignoreversion; Components: Binaries\BWAPI ; Check: GetBroodwarPath
Source: "..\Release_Binary\ChangeLog.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Release_Binary\COPYING"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Release_Binary\README"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Release_Binary\iccup_reg.exe"; DestDir: "{app}"; Flags: ignoreversion deleteafterinstall
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
Source: "..\Release_Binary\Starcraft\bwapi-data\data\bspatch.exe"; DestDir: "{code:BroodwarPath}\bwapi-data\data\"; Flags: ignoreversion; Components: Binaries\BWAPI ; Check: GetBroodwarPath

[Setup]
AppName=Brood War Application Programming Interface (BWAPI)
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
Name: "Binaries\BWAPI"; Description: "BWAPI Binaries (Requires Starcraft: Broodwar)"; Types: full custom ; Check: GetBroodwarPath
Name: "Binaries\vcredist"; Description: "Microsoft Visual C++ 2010 Redistributable"; Types: custom full
Name: "Binaries\iccup"; Description: "International Cyber Cup Battle.net Server Gateway"; Types: custom full
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
Filename: "{app}\iccup_reg.exe"; WorkingDir: "{app}"; Description: "Register ICCup Battle.net Gateway"; Components: Binaries\iccup

[ThirdParty]
UseRelativePaths=True

[Types]
Name: "full"; Description: "Full Installation"
Name: "custom"; Description: "Custom Installation"; Flags: iscustom

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
    // do something to location the Starcraft install path
  end

  Result := len > 0;
end;

//  Returns the path
function BroodwarPath(Param: String) : String;
begin
   Result := sBroodwarPath;
end;
