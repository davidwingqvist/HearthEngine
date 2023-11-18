#include "Header.h"
#define NOMINMAX 0
#include "Engine.h"
#pragma comment(lib, "d2d1.lib")
#define STB_IMAGE_IMPLEMENTATION
#include "../../Thirdparty/stb_image/stb_image.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	// Allocate a Console window if deubg
#if _DEBUG

	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);

#endif
	Engine engine("peeky.jpg");

	engine.Build();
	engine.Start();

#if _DEBUG

	fclose(fp);
	FreeConsole();

#endif


	return 0;
}

