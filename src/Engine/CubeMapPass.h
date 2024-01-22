#pragma once
#include "Renderpass.h"
#include "Shader.h"

class CubeMapPass : public IRenderpass
{
private:

	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;
	UINT m_nrOfIndices = 0;

	ID3D11Texture2D* m_cubeMap = nullptr;
	ID3D11ShaderResourceView* m_cubeMapView = nullptr;
	ID3D11SamplerState* m_sampler = nullptr;
	ID3D11InputLayout* m_inputLayout = nullptr;
	ID3D11DepthStencilState* m_lessEqualState = nullptr;
	ID3D11RasterizerState* m_rasterState = nullptr;

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
	void Pass(Scene* currentScene) override;
	void Postpass() override;
};