@echo off
for /f "tokens=2*" %%A in ('reg query "HKEY_CURRENT_USER\Software\Valve\Steam" /v SteamPath 2^>nul') do set STEAMDIR=%%B

if "%STEAMDIR%" == "" (
  echo "Steam not found."
  goto END
)

if not exist steam_files\ (
  echo steam_files directory not found.
  echo Make sure you downloaded this package from AppVeyor/GitHub releases page.
  goto END
)

if not exist redist\vc_redist.x86.exe (
  echo VC++Redist x86 2017 installer not found.
  echo.
  goto INSTALL_ADBLOCKER
)
"redist\vc_redist.x86.exe" /install /passive /norestart

:INSTALL_ADBLOCKER
set STEAMCEFDIR=%STEAMDIR%\bin\cef\cef.win7
copy steam_files\* "%STEAMCEFDIR%" /y > nul 2>&1

if %ERRORLEVEL% neq 0 (
  echo Can't install Steam Adblocker files.
  echo Close Steam if it is running.
  goto END
)

echo Steam Adblocker has been successfully installed.
echo Restart Steam if it is running.

:END
echo.
echo.
pause