#pragma once

class Model3D;

struct object2D
{
	float x;
	float y;
	float width;
	float height;

	object2D()
	{
		x = 0;
		y = 0;
		width = 0;
		height = 0;
	}

	object2D(const float& x, const float& y, const float& width, const float& height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}

	object2D(const std::string& filename)
	{
		x = 0;
		y = 0;
		width = 0;
		height = 0;
	}
};

struct transform
{
	sm::Vector3 pos;
	sm::Vector3 rotation;
	sm::Vector3 scale;

	sm::Matrix GetMatrix()
	{
		return sm::Matrix::CreateWorld(pos, pos.Forward, sm::Vector3(0, 1, 0));
	}
};

struct vertex_data
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	float u = 0.0f;
	float v = 0.0f;
};

struct model
{
	Model3D* data;
};
