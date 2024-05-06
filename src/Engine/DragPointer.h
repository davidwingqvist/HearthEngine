#pragma once

class DragPointer
{
public:

	DragPointer();

	// Draw pointers at the currentEntity position.
	void Draw(const recs::Entity& currentEntity);

	void SetRegistry(recs::recs_registry* registry);

private:

	recs::recs_registry* m_currentRegistry = nullptr;
	std::shared_ptr<Model3D> m_pointerModel = nullptr;

};