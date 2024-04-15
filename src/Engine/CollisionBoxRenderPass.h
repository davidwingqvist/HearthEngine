#pragma once
#include "Renderpass.h"
#include "Shader.h"

class CollisionBoxRenderPass : public IRenderpass
{
private:

	sm::Vector3 m_points[8] = {};

	sm::Vector4 m_color = {0.0f, 1.0f, 0.0f, 1.0f};
	DXPointer<ID3D11Buffer> m_colorBuffer;

	DXPointer<ID3D11Buffer> m_vertexBuffer;
	DXPointer<ID3D11Buffer> m_indexBuffer;

	DXPointer<ID3D11Texture2D> m_texture;
	DXPointer<ID3D11ShaderResourceView> m_shaderView;
	DXPointer<ID3D11RenderTargetView> m_renderTargetView;

	DXPointer<ID3D11InputLayout> m_inputLayout;

	PixelShader m_pixelShader;
	VertexShader m_vertexShader;

	bool CreateIndexBuffer();
	bool CreateVertexBuffer();
	bool CreateColorBuffer();
	bool CreateInputLayout();
	bool UpdateVertices(const CollisionBox& box);

	void SetGreenColor();

	void SetRedColor();

	bool UpdateColorBuffer();

public:

	CollisionBoxRenderPass();
	~CollisionBoxRenderPass();

	// Inherited via IRenderpass
	void Create() override;
	void Prepass() override;
	void Pass(InternalScene* currentScene) override;
	void Postpass() override;
};