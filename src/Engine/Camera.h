
class Camera
{
private:

	Camera_data m_matData;
	ID3D11Buffer* m_buffer;
	sm::Vector3 m_position;

	void Update();
	bool SetUpBuffer();
	bool UpdateBuffer();
public:

	Camera();
	~Camera();

	// Set the position of the camera in the world scene.
	void SetPosition(sm::Vector3 pos);

	// Activate this camera to be the main camera.
	void Activate();
};