# IPM - ISO Powershell Module
This is a rudimentary tool that I made for learning purposes, it uses the IMAPI2 C++ library for creating ISO files from directories and it's contents, a task I found myself doing quite often since I like to backup my stuff in that way.

# So, C++ for a powershell module?
Sure, just take a look... Interested in using unmanaged C++ for the task? Just check https://github.com/kenpb/unmanagedcpp-powershell-module

# Instalation
- Download the repository
- Compile it using Visual Studio 2013 or newer
- The dll should be at ```/x64``` folder
- Create a folder in ```$home\Documents\WindowsPowerShell\Modules``` called ipm and copy the files
- Load it with ```Import-Module ipm``` you sould see it when entering ```Get-Module```
