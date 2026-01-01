@Echo off
Echo Deleting old Build directory
if exist "Build" (
	Rmdir /s /q Build
)
Echo Setting up
cmake -S . -B Build -G "Visual Studio 18 2026" -A Win32