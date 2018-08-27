@echo off

rd /s /q cef_download
mkdir cef_download
cd cef_download

curl -o cef.tar.bz2 http://opensource.spotify.com/cefbuilds/cef_binary_3.3325.1758.g9aea513_windows32_minimal.tar.bz2
7z x cef.tar.bz2
7z x cef.tar
rename cef_binary_3.3325.1758.g9aea513_windows32_minimal cef_binary

rd /s /q ..\..\deps\cef_binary
move cef_binary ..\..\deps\

cd ..
rd /s /q cef_download