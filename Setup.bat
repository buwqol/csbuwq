@Echo off
if exist "Build" (
	rmdir /s /q Build
)
cmake -S . -B Build -G "Visual Studio 18 2026" -A Win32