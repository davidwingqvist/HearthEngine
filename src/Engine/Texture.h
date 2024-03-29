#pragma once
#include "Resource.h"

class Texture : public IResource
{
private:

	DXPointer<ID3D11Texture2D> m_texture;
	DXPointer<ID3D11ShaderResourceView> m_textureView;
	std::string m_filename;
	int m_width = 0;
	int m_height = 0;
	int m_format = 0;

public:

	Texture();

	~Texture();

	ID3D11ShaderResourceView** GetShaderView();

	// Sets the texture to the pixelshader slot 0
	void SetAsTexture();

	std::string GetName() override;

	// Inherited via IResource
	bool Create(const std::string& filename) override;
};