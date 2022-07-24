$CEFBUILD = "84.4.1+gfdc7504+chromium-84.0.4147.105"

Remove-Item "..\deps\cef_binary" -Force -Recurse -ErrorAction Ignore
Remove-Item "cef_download" -Force -Recurse -ErrorAction Ignore
New-Item "cef_download" -ItemType directory | Out-Null

Invoke-WebRequest -Uri "https://cef-builds.spotifycdn.com/cef_binary_${CEFBUILD}_windows64_minimal.tar.bz2" -OutFile "cef_download\cef_x64.tar.bz2"
tar xvfj cef_download\cef_x64.tar.bz2 -C cef_download 2>$null
New-Item "cef_download\cef_binary\Release\x64" -ItemType directory | Out-Null
Move-Item cef_download\cef_binary_${CEFBUILD}_windows64_minimal\Release\libcef.lib cef_download\cef_binary\Release\x64
Move-Item cef_download\cef_binary_${CEFBUILD}_windows64_minimal\include cef_download\cef_binary

Invoke-WebRequest -Uri "https://cef-builds.spotifycdn.com/cef_binary_${CEFBUILD}_windows32_minimal.tar.bz2" -OutFile "cef_download\cef_x86.tar.bz2"
tar xvfj cef_download\cef_x86.tar.bz2 -C cef_download 2>$null
New-Item "cef_download\cef_binary\Release\Win32" -ItemType directory | Out-Null
Move-Item cef_download\cef_binary_${CEFBUILD}_windows32_minimal\Release\libcef.lib cef_download\cef_binary\Release\Win32

(Get-Content cef_download\cef_binary\include\internal\cef_string.h) -Replace '#define CEF_STRING_TYPE_UTF16 1', '// #define CEF_STRING_TYPE_UTF16 1' | Set-Content cef_download\cef_binary\include\internal\cef_string.h

Move-Item cef_download\cef_binary ..\deps
Remove-Item "cef_download" -Force -Recurse
