
class Camera
{
private:

	camera_data m_matData{};
	ID3D11Buffer* m_buffer;
	sm::Vector3 m_position{};
	sm::Vector3 m_lookAt{};
	sm::Vector3 m_right{};

	float m_pitch = 0.0f;
	float m_yaw = 0.0f;

	float m_speed = 1.0f;

	void Update();
	void UpdateRotation();
	bool SetUpBuffer();
	bool UpdateBuffer();
public:

	Camera();
	~Camera();

	// Set the position of the camera in the world scene.
	void SetPosition(const sm::Vector3& pos);
	// Update position based on current position.
	void SetPosition(const float& x = 0, const float& y = 0, const float& z = 0);

	// Update the position of the camera based on keyboard input
	void Move();

	// Activate this camera to be the main camera.
	void Activate() const;
};