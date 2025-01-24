@echo off
set repository="%cd%"

:restart

taskkill /f /im "workbenchApp.exe"
rmdir /s /q "P:/temp"

timeout 1 /nobreak

cd /d "%~dp0DabsFramework\Workbench"

for /f "tokens=2,*" %%a in ('reg query "HKCU\SOFTWARE\Bohemia Interactive\Dayz Tools" /v "path" 2^>nul') do (
    set "dayz_tools=%%b"
)

set "dayz_tools=C:\Program Files (x86)\Steam\steamapps\common\DayZ Tools"

:: Check if the last part of the dayz_tools path is "DayZ Tools"
if "%dayz_tools:~-10%"=="DayZ Tools" (
    set "profile_path=%homedrive%%homepath%\Documents\DayZ"
) else (
    set "profile_path=%homedrive%%homepath%\Documents\DayZ Exp"
)

start "" /b "%dayz_tools%\Bin\Workbench\workbenchApp.exe" "-profiles=%profile_path% -repository=\"%repository%\"" -newErrorsAreWarnings=1

exit
