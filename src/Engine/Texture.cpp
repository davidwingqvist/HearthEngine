#include "Header.h"
#include "Texture.h"
#include "../../Thirdparty/stb_image/stb_image.h"
#include "D3D11Context.h"
#include "Debugger.h"

Texture::Texture()
{
}

Texture::~Texture()
{
    if (m_texture)
        m_texture->Release();
    if (m_textureView)
        m_textureView->Release();
}

const ID3D11ShaderResourceView* Texture::GetShaderView() const
{
    return m_textureView;
}

bool Texture::Create(const std::string& filename)
{
    const std::string path = TEXTUREPATH + filename;
    unsigned char* image = nullptr;
    image = stbi_load(path.c_str(), &m_width, &m_height, &m_format, STBI_rgb_alpha);
    
    D3D11_TEXTURE2D_DESC desc{};
    desc.Width = m_width;
    desc.Height = m_height;
    desc.MiscFlags = 0;
    desc.MipLevels = 0;
    desc.ArraySize = 1;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS;
    desc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA data{};
    data.pSysMem = image;

    HRESULT hr = D3D11Core::Get().Device()->CreateTexture2D(&desc, &data, &m_texture);
    if (FAILED(hr))
    {
        DEBUG_ERROR("Couldnt create texture for " + filename + " !\n")
    }
    else
    {
        hr = D3D11Core::Get().Device()->CreateShaderResourceView(m_texture, 0, &m_textureView);
    }

    stbi_image_free(image);
    return !FAILED(hr);
}
