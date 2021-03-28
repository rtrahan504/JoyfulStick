# Start TrackIR
if ((Get-Process -Name 'TrackIR5') -eq $null)
{
	if (Test-Path -Path "C:\Program Files (x86)\NaturalPoint\TrackIR5\TrackIR5.exe" -PathType Leaf)
	{
		Start-Process -FilePath "C:\Program Files (x86)\NaturalPoint\TrackIR5\TrackIR5.exe" -PassThru;
	}
}

# Open the Game Controller dialog and try to select 
control joy.cpl

# Start JoyfulStick if it isn't already running
if ((Get-Process -Name 'JoyfulStick') -eq $null)
{	
	$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path;

	if (Test-Path -Path $scriptDir/JoyfulStick.exe -PathType Leaf)
	{
		Start-Process -FilePath "$scriptDir/JoyfulStick.exe" -PassThru;
	}
	elseif (Test-Path -Path $scriptDir/x64/Release/JoyfulStick.exe -PathType Leaf)
	{
		Start-Process -FilePath "$scriptDir/x64/Release/JoyfulStick.exe" -PassThru;
	}
	elseif (Test-Path -Path $scriptDir/x64/Debug/JoyfulStick.exe -PathType Leaf)
	{
		Start-Process -FilePath "$scriptDir/x64/Debug/JoyfulStick.exe" -PassThru;
	}
	else
	{
		Add-Type -AssemblyName PresentationCore,PresentationFramework;
		[System.Windows.MessageBox]::Show("Unable to find JoyfulStick.exe. Check the path of the executable relative to the startup script.","Error","OK","Error");
	}
    Sleep 1
}

$wshell = New-Object -ComObject wscript.shell;
$wshell.AppActivate('Game Controllers');
Sleep 1
#Add page downs if your X52 doesn't show up first in the list
#$wshell.SendKeys('{DOWN}')
$wshell.SendKeys('%P');

# Start DCS
Start-Process -FilePath "steam://rungameid/223750"