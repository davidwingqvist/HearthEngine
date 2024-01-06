#include "Header.h"
#define NOMINMAX 0
#include "Engine.h"
#pragma comment(lib, "d2d1.lib")
#define STB_IMAGE_IMPLEMENTATION
#include "../../Thirdparty/stb_image/stb_image.h"


#ifdef _DEBUG

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Engine engine("peeky.jpg");
	engine.Start();

	return 0;
}


#else
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	// Allocate a Console window if deubg
	Engine engine("peeky.jpg");

	engine.Start();


	return 0;
}
#endif


