#pragma once
#include "Renderpass.h"

class CubeMapPass : public IRenderpass
{
private:

	ID3D11Texture2D* m_cubeMap;
	ID3D11ShaderResourceView* m_cubeMapView;

public:

	CubeMapPass() = default;
	~CubeMapPass();

	// Inherited via IRenderpass
	void Create() override;
	void Prepass() override;
	void Pass(Scene* currentScene) override;
	void Postpass() override;
};