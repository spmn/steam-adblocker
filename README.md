# Steam Adblocker

Ad blocker for the Steam client web browser. It integrates the [Brave ad block engine][brave_github] into Steam browser,
providing support for filtering pages using ABP lists like Easylist.

Windows only, but the most part of the ad blocker code is compatible with Steam for Linux/Mac.

## Installing

1. Download the latest package from the [Releases][download] page on GitHub or 
build it yourself from source code (see [appveyor.yml][appveyor_file])
2. Extract it
3. Run `installer.bat` or copy the content of `steam_files` to `<STEAM DIRECTORY>\bin\cef\cef.win7`
4. Restart Steam if it is running


## Custom ABP filter lists

Packages from GitHub releases page come bundled with an Easylist filter list.

In order to switch to other filter list:

1. Have a local copy of the list
2. Run `serializer.exe <path/to/list>`
3. Copy the generated `ABPFilterParserData.dat` to `<STEAM DIRECTORY>\bin\cef\cef.win7`
4. Restart Steam if it is running


## How does it work (or "will I get VAC banned" section)

Steam, Steam browser and Steam games run all as separate processes. Steam Adblocker injects itself **only** into 
Steam browser processes and hooks certain functions related to the loading of web resources.

This means that your account *should be safe*. If you need 100% safety, rename `<STEAM DIRECTORY>\bin\cef\cef.win7\version.dll` to anything else before you start VAC protected games.


[brave_github]: https://github.com/brave/ad-block
[download]: #
[appveyor_file]: appveyor.yml
