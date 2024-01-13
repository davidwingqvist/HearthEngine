#pragma once
#include <d3d11.h>
#include "Window.h"
#include "Shader.h"

class PipelineManager
{
public:

	ID3D11RenderTargetView* m_backBuffer;
	ID3D11UnorderedAccessView* m_backBufferAccessView;

	ID3D11DepthStencilState* m_depthStencilStateLess;
	ID3D11Texture2D* m_depthStencilTexture;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11ShaderResourceView* m_depthBufferSRV;

	ID3D11RasterizerState* m_rasterState;
	ID3D11BlendState* m_blendStateAlphaBlending;
	ID3D11SamplerState* m_anisotropicSamplerState;
	ID3D11SamplerState* m_linearSamplerState;
	ID3D11InputLayout* m_defaultInputLayout;
	D3D11_VIEWPORT m_viewport;
	UINT m_windowWidth = 0;
	UINT m_windowHeight = 0;

	VertexShader m_baseVertexShader;
	PixelShader m_basePixelShader;

private:

	bool CreateRenderTargetView();
	bool CreateDepthStencilStates();
	bool CreateDepthBuffer();
	bool CreateRasterizerStates();
	bool CreateSamplerStates();
	bool CreateBlendStates();
	bool CreateShaders();
	bool CreateInputLayouts();
	void SetViewport();

public:

	void Initialize();

	PipelineManager();
	~PipelineManager();

	void ClearScreen();

};

