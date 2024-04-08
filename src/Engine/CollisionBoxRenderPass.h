#pragma once
#include "Renderpass.h"
#include "Shader.h"

class CollisionBoxRenderPass : public IRenderpass
{
private:

	DXPointer<ID3D11Buffer> m_vertexBuffer;
	DXPointer<ID3D11Texture2D> m_texture;
	DXPointer<ID3D11ShaderResourceView> m_shaderView;
	DXPointer<ID3D11RenderTargetView> m_renderTargetView;

	PixelShader m_pixelShader;
	VertexShader m_vertexShader;

public:

	CollisionBoxRenderPass();
	~CollisionBoxRenderPass();

	// Inherited via IRenderpass
	void Create() override;
	void Prepass() override;
	void Pass(InternalScene* currentScene) override;
	void Postpass() override;
};