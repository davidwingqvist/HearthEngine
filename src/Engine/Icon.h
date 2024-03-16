#pragma once
#include "Resource.h"

class Icon : public IResource
{
public:

	Icon();
	~Icon();

	ID3D11ShaderResourceView** GetShaderView();

	bool Create(const std::string& filename) override;

	std::string GetName() override;

private:

	DXPointer<ID3D11Texture2D> m_texture;
	DXPointer<ID3D11ShaderResourceView> m_textureView;
	std::string m_filename;
	int m_width = 0;
	int m_height = 0;
	int m_format = 0;

};