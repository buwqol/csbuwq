@Echo off
Echo Cmake-ing
cmake --build Build --config Release

if not exist "Out" (
	Echo Creating Out directory
	Mkdir "Out"
)

Echo Copying csbuwq.exe
Copy /Y Build\Release\csbuwq.exe Out\csbuwq.exe
Echo Copying csbuwqds.exe
Copy /Y Build\Release\csbuwqds.exe Out\csbuwqds.exe

Pushd Source\Panorama\
Echo Running packer
python ..\..\packer.py
Popd

if not exist "Out\csgo" (
	Echo Creating Out\csgo directory
	Mkdir "Out\csgo"
)
if not exist "Out\csgo\panorama" (
	Echo Creating Out\csgo\panorama directory
	Mkdir "Out\csgo\panorama"
)
Echo Copying buwq.pbin
Copy /Y Source\Panorama\buwq.pbin Out\csgo\panorama\buwq.pbin