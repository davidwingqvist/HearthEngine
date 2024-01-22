#pragma once
#include <d3d11.h>
#include "Window.h"

#define DC D3D11Core::Get().Context()
#define DD D3D11Core::Get().Device()

class D3D11Core
{
private:

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_context;
	IDXGISwapChain* m_swapChain;
	Window* m_window;

	bool CreateContext();
	D3D11Core();
	~D3D11Core();

public:

	static auto& Get()
	{
		static D3D11Core context;
		return context;
	}

	void Initialize(Window* pWindow);

	ID3D11Device* Device() const;
	ID3D11DeviceContext* Context() const;
	IDXGISwapChain* SwapChain() const;
	Window* GetWindow() const;

	void Present();
};

