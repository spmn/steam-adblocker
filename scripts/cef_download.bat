@echo off

set CEFBUILD=3.3440.1806.g65046b7

rd /s /q ..\deps\cef_binary
rd /s /q cef_download
mkdir ..\deps\cef_binary
mkdir cef_download
cd cef_download

curl -o cef_x86.tar.bz2 http://opensource.spotify.com/cefbuilds/cef_binary_%CEFBUILD%_windows32_minimal.tar.bz2
7z x cef_x86.tar.bz2
7z x cef_x86.tar
rename cef_binary_%CEFBUILD%_windows32_minimal Win32
move Win32 ..\..\deps\cef_binary\

curl -o cef_x64.tar.bz2 http://opensource.spotify.com/cefbuilds/cef_binary_%CEFBUILD%_windows64_minimal.tar.bz2
7z x cef_x64.tar.bz2
7z x cef_x64.tar
rename cef_binary_%CEFBUILD%_windows64_minimal x64
move x64 ..\..\deps\cef_binary\

cd ..
rd /s /q cef_download