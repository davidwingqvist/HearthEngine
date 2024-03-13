#pragma once
#include "Renderpass.h"
#include "Shader.h"

class CubeMapPass : public IRenderpass
{
private:

	DXPointer<ID3D11Buffer> m_vertexBuffer;
	DXPointer<ID3D11Buffer> m_indexBuffer;
	UINT m_nrOfIndices = 0;

	DXPointer<ID3D11Texture2D> m_cubeMap;
	DXPointer<ID3D11ShaderResourceView> m_cubeMapView;
	DXPointer<ID3D11SamplerState> m_sampler;
	DXPointer<ID3D11InputLayout> m_inputLayout;
	DXPointer<ID3D11DepthStencilState> m_lessEqualState;
	DXPointer<ID3D11RasterizerState> m_rasterState;

	PixelShader m_pixelShader;
	VertexShader m_vertexShader;

	bool CreateBoxBuffers();
	bool CreateBoxTexture();

	bool CreateSampler();
	bool CreateInputLayout();
	bool CreateRasterizerState();
	bool CreateDepthStencil();

public:

	CubeMapPass();
	~CubeMapPass();

	// Inherited via IRenderpass
	void Create() override;
	void Prepass() override;
	void Pass(InternalScene* currentScene) override;
	void Postpass() override;
};