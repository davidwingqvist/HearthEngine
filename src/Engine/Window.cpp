#include "Header.h"
#include "Window.h"
#include <assert.h>
#include "InputManager.h"
#include "Debugger.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK Window::WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
		return true;
#endif

	// Engine events:
	switch (uMsg)
	{
	case WM_ACTIVATE:
		//ConfineCursor(hwnd);
		break;
	case WM_ACTIVATEAPP:
		InputManager::Get().GetKeyboard()->ProcessMessage(uMsg, wParam, lParam);
		InputManager::Get().GetMouse()->ProcessMessage(uMsg, wParam, lParam);
		break;
	case WM_NCCREATE:

		break;
	case WM_DESTROY:
		ClipCursor(nullptr);
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		ClipCursor(nullptr);
		PostQuitMessage(0);
		break;
	case WM_INPUT:
		InputManager::Get().GetMouse()->ProcessMessage(uMsg, wParam, lParam);
		break;
	case WM_KEYDOWN:
		InputManager::Get().GetKeyboard()->ProcessMessage(uMsg, wParam, lParam);
		break;
	case WM_KEYUP:
		InputManager::Get().GetKeyboard()->ProcessMessage(uMsg, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		break;
	case WM_MBUTTONDBLCLK:
		break;
	case WM_MBUTTONUP:
		break;
	case WM_MBUTTONDOWN:
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_MOUSEACTIVATE:
		// When you click activate the window, we want Mouse to ignore it.
		return MA_ACTIVATEANDEAT;
	case WM_RBUTTONUP:
		break;
	case WM_MOUSEHOVER:
		InputManager::Get().GetMouse()->ProcessMessage(uMsg, wParam, lParam);
		break;
	case WM_MOUSEWHEEL:
		break;
	case WM_SYSKEYDOWN:
		InputManager::Get().GetKeyboard()->ProcessMessage(uMsg, wParam, lParam);
		break;
	case WM_SYSKEYUP:
		InputManager::Get().GetKeyboard()->ProcessMessage(uMsg, wParam, lParam);
		break;
	case WM_SIZE:
		// https://docs.microsoft.com/en-us/windows/win32/direct3ddxgi/d3d10-graphics-programming-guide-dxgi#handling-window-resizing
		break;
	default:
		break;
	}
	

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Window::ConfineCursor(HWND hwnd)
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	MapWindowPoints(hwnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
	ClipCursor(&rect);
}

Window::Window()
	: m_hWnd(nullptr)
	, m_clientRect({})
	, m_windowDesc({})
{
}

Window::~Window()
{
	DestroyWindow(this->m_hWnd);
}

bool Window::Initialize(const Desc& desc)
{
	// Define window style.
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));

	const LPCWSTR WINDOW_CLASS = L"DefaultWindowClass";

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.lpszClassName = WINDOW_CLASS;
	wcex.lpszMenuName = nullptr;
	wcex.hbrBackground = static_cast<HBRUSH>(CreateSolidBrush(RGB(100, 149, 237)));	// Cornflower blue.
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);;
	wcex.hInstance = GetModuleHandle(NULL);
	wcex.lpfnWndProc = WinProc;

	// Register window class.
	const ATOM result = RegisterClassEx(&wcex);
	assert(result && "Failed to register window class.");

	// Retrieve desktop window.
	RECT desktop;
	const HWND hwndDesktop = GetDesktopWindow();
	GetWindowRect(hwndDesktop, &desktop);

	const int posX = (desktop.right / 2) - (desc.width / 2);
	const int posY = (desktop.bottom / 2) - (desc.height / 2);

	RECT rect{};
	rect.left = posX;
	rect.right = posX + desc.width;
	rect.top = posY;
	rect.bottom = posY + desc.height;

	m_clientRect = rect;
	int width;
	int height;
	int fullscreen = 0;
	if (fullscreen == 0)
	{
		AdjustWindowRect(&rect, WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE, FALSE);
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;
	}
	else
	{
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;
		AdjustWindowRect(&rect, WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE, FALSE);
	}

	// Create the window.
	this->m_hWnd = CreateWindowEx(0, WINDOW_CLASS, desc.title,
		WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
		posX, posY,
		width, height,
		nullptr, nullptr, desc.hInstance, nullptr);

	assert(this->m_hWnd && "Window wasn't successfully created.");

	UpdateWindow(this->m_hWnd);

	if (fullscreen == 0)
	{
		ShowWindow(this->m_hWnd, desc.nShowCmd);
	}
	else
	{
		ShowWindow(this->m_hWnd, SW_MAXIMIZE);
		SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);
		SetWindowPos(m_hWnd, HWND_TOP, 0, 0, width, height, SWP_FRAMECHANGED);
	}

	//ConfineCursor(this->m_hWnd);
	this->m_windowDesc = desc;
	InputManager::Get().Initialize(this->m_hWnd);

	return true;
}

HWND Window::GetHWnd() const
{
	return this->m_hWnd;
}

RECT Window::GetWindowClientRect() const
{
	return this->m_clientRect;
}

unsigned int Window::GetWidth() const
{
	return this->m_windowDesc.width;
}

unsigned int Window::GetHeight() const
{
	return this->m_windowDesc.height;
}

LPCWSTR Window::GetTitle() const
{
	return this->m_windowDesc.title;
}

bool Window::IsFullScreen() const
{
	return this->m_windowDesc.fullScreen;
}

void Window::SetWindowTitle(const LPCWSTR& title)
{
	this->m_windowDesc.title = title;
}

void Window::SetWindowTextBar(const std::string& text)
{
	SetWindowTextA(this->m_hWnd, text.c_str());
}

void Window::SetFullScreen(bool fullscreen)
{
	assert(this->m_hWnd && "There's no window to resize.");

	//D3D11Core::Get().SwapChain()->SetFullscreenState(true, nullptr);
}