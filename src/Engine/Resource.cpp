#include "Header.h"
#include "Resource.h"
#include "Debugger.h"
#include "D3D11Context.h"

/*
* 
*   MEMORY LEAKS ARE HAPPENING HERE BECAUSE OF DIRECTX pointer
* 
* 
*/


Image2D::Image2D()
{
    m_image = nullptr;
}

Image2D::~Image2D()
{
    if (m_image)
        m_image->Release();
}

ID2D1Bitmap* const Image2D::GetImage() const
{
    return m_image;
}

bool Image2D::Create(const std::string& filename)
{
    return D2D1Core::Get().CreateImage(filename, &m_image);
}

bool Model3D::CreateVertexBuffer(std::vector<vertex_data>& modelData, submesh& mesh)
{
    if (modelData.empty())
        return false;

    D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth = static_cast<UINT>(sizeof(vertex_data) * modelData.size());
    desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA data = {};
    data.pSysMem = &modelData[0];
    data.SysMemPitch = 0;
    data.SysMemSlicePitch = 0;

    mesh.vertexCount = static_cast<UINT>(modelData.size());
    HRESULT hr = D3D11Core::Get().Device()->CreateBuffer(&desc, &data, &mesh.vertexBuffer);
    return !FAILED(hr);
}

bool Model3D::CreateIndexBuffer(std::vector<UINT>& indices, submesh& mesh)
{
    if (indices.empty())
        return false;

    D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth = static_cast<UINT>(sizeof(UINT) * indices.size());
    desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA data = {};
    data.pSysMem = &indices[0];
    data.SysMemPitch = 0;
    data.SysMemSlicePitch = 0;

    mesh.indexCount = static_cast<UINT>(indices.size());
    HRESULT hr = D3D11Core::Get().Device()->CreateBuffer(&desc, &data, &mesh.indexBuffer);
    return !FAILED(hr);
}

void Model3D::LoadBufferData(const aiScene* scene, const std::string& filename)
{
    std::vector<vertex_data> data;
    std::vector<UINT> indices;
    UINT indexOffset = 0;
    for (unsigned int i = 0; i < scene->mNumMeshes; i++)
    {

        const aiMesh* mesh = scene->mMeshes[i];
        for (unsigned int j = 0; j < mesh->mNumVertices; j++)
        {
            vertex_data mData;
            mData.position = { mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z };
            mData.texCoords = { mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y };
            mData.normal = { mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z };
            data.push_back(mData);
        }

        UINT localMaxIndex = 0;
        for (unsigned int f = 0; f < mesh->mNumFaces; f++)
        {
            const aiFace face = mesh->mFaces[f];
            if (face.mNumIndices == 3)
            {
                for (unsigned int id = 0; id < 3; id++)
                {
                    UINT faceIndex = face.mIndices[id];
                    if (faceIndex > localMaxIndex)
                        localMaxIndex = faceIndex;

                    indices.push_back(face.mIndices[id] + indexOffset);
                }
            }
        }

        indexOffset += (localMaxIndex + 1);

        submesh newMesh{};

        if (!this->CreateVertexBuffer(data, newMesh))
            DEBUG_ERROR("Couldnt create vertex buffer for model: '" + filename + "'\n");
        if (!this->CreateIndexBuffer(indices, newMesh))
            DEBUG_ERROR("Couldnt create index buffer for model: '" + filename + "'\n");

        m_meshes.push_back(newMesh);
    }


}

Model3D::Model3D()
{

}

Model3D::~Model3D()
{
    for (int i = 0; i < m_meshes.size(); i++)
    {
        if (m_meshes[i].indexBuffer)
            m_meshes[i].indexBuffer->Release();

        if(m_meshes[i].vertexBuffer)
            m_meshes[i].vertexBuffer->Release();
    }
}

void Model3D::Draw()
{
    UINT offset = 0;
    UINT stride = sizeof(vertex_data);

    D3D11Core::Get().Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    for (size_t m = 0; m < m_meshes.size(); m++)
    {
        D3D11Core::Get().Context()->IASetVertexBuffers(0, 1, &m_meshes[m].vertexBuffer, &stride, &offset);
        D3D11Core::Get().Context()->IASetIndexBuffer(m_meshes[m].indexBuffer, DXGI_FORMAT_R32_UINT, 0);
        D3D11Core::Get().Context()->DrawIndexed(m_meshes[m].indexCount, 0, 0);
    }

}

const std::string Model3D::GetName() const
{
    return m_name;
}

bool Model3D::Create(const std::string& filename)
{
    Assimp::Importer importer;

    m_name = filename;

    importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

    const aiScene* scene = importer.ReadFile
    (
        MODELPATH + filename,
        aiProcess_Triangulate |   //Triangulate every surface
        //aiProcess_JoinIdenticalVertices |   //Ignores identical veritices - memory saving  
        aiProcess_FlipWindingOrder |   //Makes it clockwise order
        aiProcess_MakeLeftHanded |	//Use a lefthanded system for the models 
        //aiProcess_CalcTangentSpace |   //Fix tangents and bitangents automatic for us
        aiProcess_FlipUVs   //Flips the textures to fit directX-style
        //aiProcess_LimitBoneWeights 		    //Limits by default to 4 weights per vertex
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        Debugger::Get().Print("Assimp couldnt open: '" + filename + "'\n", Debugger::COLOR_RED);
        importer.FreeScene();
        return false;
    }

    if (!scene->HasMeshes())
    {
        DEBUG_ERROR("Model: '" + filename  + "' has no meshes.\n");
        return false;
    }

    this->LoadBufferData(scene, filename);

    importer.FreeScene();

    return true;
}
