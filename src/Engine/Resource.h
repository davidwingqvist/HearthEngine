#pragma once
#include <string>
#include "Direct2DContext.h"

class IResource
{
private:

public:

	IResource() = default;
	virtual ~IResource() = 0 {};

	virtual bool Create(const std::string& filename) = 0;
};

class Image2D : public IResource
{
private:

	ID2D1Bitmap* m_image;

public:

	Image2D();
	~Image2D();

	ID2D1Bitmap* const GetImage() const;

	// Inherited via IResource
	virtual bool Create(const std::string& filename) override;

};

class Model3D : public IResource
{
private:

	struct submesh
	{
		ID3D11Buffer* vertexBuffer = nullptr;
		UINT vertexCount = 0;
		ID3D11Buffer* indexBuffer = nullptr;
		UINT indexCount = 0;
	};
	std::vector<submesh> m_meshes;
	std::string m_name;

	bool CreateVertexBuffer(std::vector<vertex_data>& modelData, submesh& mesh);

	bool CreateIndexBuffer(std::vector<UINT>& indices, submesh& mesh);

	void LoadBufferData(const aiScene* scene, const std::string& filename);

public:

	Model3D();
	~Model3D();

	void Draw();
	const std::string GetName() const;

	// Inherited via IResource
	virtual bool Create(const std::string& filename) override;
};
