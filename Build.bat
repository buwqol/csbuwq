@Echo off
cmake --build Build --config Release

if not exist "Out" (
	mkdir "Out"
)

COPY /Y Build\Release\csbuwq.exe Out\csbuwq.exe