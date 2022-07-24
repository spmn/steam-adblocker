@echo off

setlocal enabledelayedexpansion

set CEFBUILD=84.4.1+gfdc7504+chromium-84.0.4147.105
set CEFBUILD_ESCAPED=!CEFBUILD:+=%%2B!

rd /s /q ..\deps\cef_binary
rd /s /q cef_download
mkdir cef_download
cd cef_download

curl -o cef_x64.tar.bz2 https://cef-builds.spotifycdn.com/cef_binary_%CEFBUILD_ESCAPED%_windows64_minimal.tar.bz2
7z x cef_x64.tar.bz2
7z x cef_x64.tar
mkdir cef_binary\Release\x64
move cef_binary_%CEFBUILD%_windows64_minimal\Release\libcef.lib cef_binary\Release\x64\
move cef_binary_%CEFBUILD%_windows64_minimal\include cef_binary

curl -o cef_x86.tar.bz2 https://cef-builds.spotifycdn.com/cef_binary_%CEFBUILD_ESCAPED%_windows32_minimal.tar.bz2
7z x cef_x86.tar.bz2
7z x cef_x86.tar
mkdir cef_binary\Release\Win32
move cef_binary_%CEFBUILD%_windows32_minimal\Release\libcef.lib cef_binary\Release\Win32\

move cef_binary ..\..\deps\
cd ..
rd /s /q cef_download
