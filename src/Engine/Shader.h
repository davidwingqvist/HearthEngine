#pragma once
#include <string>
#include <d3d11.h>

class IShader
{
private:
    std::string m_shaderByteCode;
    std::string m_filename;
protected:
    bool LoadShaderDataFromFile(const std::string & filename);
public:
    std::string GetShaderByteCode() const;
};

class VertexShader : public IShader
{
private:
    ID3D11VertexShader* m_shader;
public:
    VertexShader() = default;
    VertexShader(const VertexShader& other) = default;
    VertexShader(VertexShader&& other) = default;
    VertexShader& operator=(const VertexShader& other) = default;
    VertexShader& operator=(VertexShader&& other) = default;
    virtual ~VertexShader();

    bool Create(const std::string& filename);

    ID3D11VertexShader* const* GetAddressOf() const;
    ID3D11VertexShader* Get() const;
};

class PixelShader : public IShader
{
private:
    ID3D11PixelShader* m_shader;
public:
    PixelShader() = default;
    PixelShader(const PixelShader& other) = default;
    PixelShader(PixelShader&& other) = default;
    PixelShader& operator=(const PixelShader& other) = default;
    PixelShader& operator=(PixelShader&& other) = default;
    virtual ~PixelShader();

    bool Create(const std::string& filename);

    ID3D11PixelShader* const* GetAddressOf() const;
    ID3D11PixelShader* Get() const;
};

