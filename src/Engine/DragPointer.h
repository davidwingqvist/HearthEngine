#pragma once

class Camera;

class DragPointer
{
public:

	DragPointer();

	// Draw pointers at the currentEntity position.
	void Draw(const recs::Entity& currentEntity);

	void SetRegistry(recs::recs_registry* registry);
	void SetCamera(Camera* camera);

	void StartUp();

private:

	Camera* m_camera = nullptr;
	recs::recs_registry* m_currentRegistry = nullptr;
	std::shared_ptr<Model3D> m_pointerModel = nullptr;
	std::shared_ptr<Texture> m_pointerTextures[3];

	Transform m_transforms[3];
	InternalBox m_hitBoxes[3];

	int m_prevMouseX = 0;
	int m_currMouseX = 0;

	int m_prevMouseY = 0;
	int m_currMouseY = 0;

	DXPointer<ID3D11Buffer> m_modelBuffer;

	bool CreateModelBuffer();

};