#include <stdbool.h>
#include <wchar.h>
#include <windows.h>

#if !defined(DEDICATED)
__declspec(dllexport) DWORD NvOptimusEnablement = 1;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
__declspec(dllexport) bool BSecureAllowed(unsigned char *p0, int i1, int i2) { return true; }
__declspec(dllexport) int CountFilesCompletedTrustCheck() { return 0; }
__declspec(dllexport) int CountFilesNeedTrustCheck() { return 0; }
__declspec(dllexport) int GetTotalFilesLoaded() { return 0; }
__declspec(dllexport) int RuntimeCheck(int i0, int i1) { return 0; }
#endif

#if defined(DEDICATED)
#define LAUNCHER_LIB "dedicated"
#define SYMBOL_NAME "DedicatedMain"
typedef int (*LauncherMain_t)(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);
#else
#define LAUNCHER_LIB "buwqcher"
#define SYMBOL_NAME "LauncherMain"
typedef int (*LauncherMain_t)(bool bSecure, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);
#endif

typedef void (*InstallGC_t)(bool dedicated);

static void ErrorMessageBox(const wchar_t *format, ...)
{
	va_list ap;
	wchar_t buffer[4096];

	va_start(ap, format);
	_vsnwprintf_s(buffer, ARRAYSIZE(buffer), ARRAYSIZE(buffer), format, ap);
	va_end(ap);

	MessageBoxW(NULL, buffer, L"csbuwq", MB_OK | MB_ICONERROR);
}

static const wchar_t *LastErrorString()
{
	static wchar_t buffer[4096];
	const int unsigned bufferSize = ARRAYSIZE(buffer);

	buffer[0] = '\0';

	int error = GetLastError();

	int result = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK, NULL, error, 0, buffer, bufferSize, NULL);
	if (!result) _snwprintf_s(buffer, bufferSize, bufferSize, L"Unknown error (%d)", error);

	return buffer;
}

static void *LoadModuleAndFindSymbol(const wchar_t *absoluteModulePath, const char *symbol)
{
	HMODULE module = LoadLibraryExW(absoluteModulePath, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if (!module)
	{
		ErrorMessageBox(L"Could not load '%s':\n%s", absoluteModulePath, LastErrorString());
		return NULL;
	}

	void *function = GetProcAddress(module, symbol);
	if (!function)
	{
		ErrorMessageBox(L"Could not find '%S' from '%s':\n%s", symbol, absoluteModulePath, LastErrorString());
		return NULL;
	}

	return function;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	wchar_t baseDir[MAX_PATH];
	wchar_t modulePath[MAX_PATH];
	const int unsigned baseDirSize = ARRAYSIZE(baseDir);

	DWORD baseDirLength = GetModuleFileNameW(NULL, baseDir, baseDirSize);
	if (!baseDirLength || baseDirLength == baseDirSize)
	{
		ErrorMessageBox(L"GetModuleFileName failed:\n%ls", LastErrorString());
		return 1;
	}

	wchar_t *slash = wcsrchr(baseDir, '\\');
	if (!slash) return 1;
	*slash = '\0';

	SetCurrentDirectoryW(baseDir);

	const wchar_t gameCoordLib[] = L"\\bin\\;";
	const int unsigned gameCoordLibSize = ARRAYSIZE(gameCoordLib);
	wchar_t replacePath[2048] = {0};
	wcscpy_s(replacePath, ARRAYSIZE(replacePath), baseDir);
	wcscat_s(replacePath, ARRAYSIZE(replacePath), gameCoordLib);

	const wchar_t *currentPath = _wgetenv(L"PATH");
	if (currentPath) wcscat_s(replacePath, ARRAYSIZE(replacePath), currentPath);

	_wputenv_s(L"PATH", replacePath);
	_snwprintf_s(modulePath, ARRAYSIZE(modulePath), ARRAYSIZE(modulePath), L"%ls\\bin\\" LAUNCHER_LIB ".dll", baseDir);
	LauncherMain_t LauncherMain = (LauncherMain_t)LoadModuleAndFindSymbol(modulePath, SYMBOL_NAME);
	if (!LauncherMain) return 1;

	_snwprintf_s(modulePath, ARRAYSIZE(modulePath), ARRAYSIZE(modulePath), L"%ls\\csgo_gc\\csgo_gc.dll", baseDir);
	InstallGC_t InstallGC = (InstallGC_t)LoadModuleAndFindSymbol(modulePath, "InstallGC");
	if (!InstallGC) return 1;

#if defined(DEDICATED)
	InstallGC(true);
#else
	InstallGC(false);
#endif

#if defined(DEDICATED)
	return LauncherMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
#else
	return LauncherMain(true, hInstance, hPrevInstance, lpCmdLine, nShowCmd);
#endif
}
