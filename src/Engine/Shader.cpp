#include "Header.h"
#include "Shader.h"
#include <sstream>
#include <fstream>
#include "D3D11Context.h"
#include "Debugger.h"

// Fix this later, start path is different from Solution and actual application.
const std::string csoPATH = "x64/Debug/";

bool IShader::LoadShaderDataFromFile(const std::string& filename)
{
    m_filename = filename;

    std::ifstream reader;

    std::string shaderData;
    reader.open(csoPATH + filename + ".cso", std::ios::binary | std::ios::ate);
    if (!reader.is_open())
    {
        DEBUG_ERROR("Failed to open " + filename + ".cso\n");
        return false;
    }
    reader.seekg(0, std::ios::end);
    shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
    reader.seekg(0, std::ios::beg);
    shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());
    m_shaderByteCode = shaderData;
    reader.close();

    return true;
}

std::string IShader::GetShaderByteCode() const
{
    return m_shaderByteCode;
}

VertexShader::~VertexShader()
{
    if (m_shader)
        m_shader->Release();
}

bool VertexShader::Create(const std::string& filename)
{
    HRESULT hr = S_FALSE;
    if (LoadShaderDataFromFile(filename))
    {
        hr = D3D11Core::Get().Device()->CreateVertexShader(GetShaderByteCode().c_str(), GetShaderByteCode().length(), nullptr, &m_shader);
    }

    return !FAILED(hr);
}

ID3D11VertexShader* const* VertexShader::GetAddressOf() const
{
    return &m_shader;
}

ID3D11VertexShader* VertexShader::Get() const
{
    return m_shader;
}

PixelShader::~PixelShader()
{
    if (m_shader)
        m_shader->Release();
}

bool PixelShader::Create(const std::string& filename)
{
    HRESULT hr = S_FALSE;
    if (LoadShaderDataFromFile(filename))
    {
        hr = D3D11Core::Get().Device()->CreatePixelShader(GetShaderByteCode().c_str(), GetShaderByteCode().length(), nullptr, &m_shader);
    }

    return !FAILED(hr);
}

ID3D11PixelShader* const* PixelShader::GetAddressOf() const
{
    return &m_shader;
}

ID3D11PixelShader* PixelShader::Get() const
{
    return m_shader;
}
