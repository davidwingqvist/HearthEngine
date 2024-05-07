#pragma once

class DragPointer
{
public:

	DragPointer();

	// Draw pointers at the currentEntity position.
	void Draw(const recs::Entity& currentEntity);

	void SetRegistry(recs::recs_registry* registry);

	void StartUp();

private:

	recs::recs_registry* m_currentRegistry = nullptr;
	std::shared_ptr<Model3D> m_pointerModel = nullptr;
	std::shared_ptr<Texture> m_pointerTexture = nullptr;

	Transform m_transforms[3];

	DXPointer<ID3D11Buffer> m_modelBuffer;

	bool CreateModelBuffer();

};