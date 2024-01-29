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
}

ID3D11ShaderResourceView** Texture::GetShaderView()
{
    return m_textureView.GetAddressOf();
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
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS;
    desc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA data{};
    data.pSysMem = image;
    data.SysMemPitch = static_cast<UINT>(m_width * m_format);
    data.SysMemSlicePitch = 0;

    HRESULT hr = D3D11Core::Get().Device()->CreateTexture2D(&desc, &data, m_texture.GetAddressOf());
    if (FAILED(hr))
    {
        DEBUG_ERROR("Couldnt create texture for " + filename + "\n")
    }
    else
    {
        hr = D3D11Core::Get().Device()->CreateShaderResourceView(m_texture.Get(), 0, m_textureView.GetAddressOf());
    }

    stbi_image_free(image);
    return !FAILED(hr);
}
