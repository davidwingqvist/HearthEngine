#pragma once
#include "Resource.h"

class Texture : public IResource
{
private:

	ID3D11Texture2D* m_texture = nullptr;
	ID3D11ShaderResourceView* m_textureView = nullptr;
	int m_width = 0;
	int m_height = 0;
	int m_format = 0;

public:

	Texture();

	~Texture();

	const ID3D11ShaderResourceView* GetShaderView() const;

	// Inherited via IResource
	bool Create(const std::string& filename) override;
};