
class Camera
{
private:

	camera_data m_matData{};
	ID3D11Buffer* m_buffer;
	sm::Vector3 m_position{};
	sm::Vector3 m_lookAt{};
	sm::Vector3 m_right{};

	sm::Vector3 m_lockedPos = {};

	float m_pitch = 0.0f;
	float m_yaw = 0.0f;

	float m_speed = 1.0f;
	float m_sensitivty = 0.5f;

	void Update();
	void UpdateRotation();
	void UpdateRotationButDontPush();
	bool SetUpBuffer();
	bool UpdateBuffer();
public:

	Camera();
	~Camera();

	void ResetValues();

	// Set the position of the camera in the world scene.
	void SetPosition(const sm::Vector3& pos);
	// Update position based on current position.
	void SetPosition(const float& x = 0, const float& y = 0, const float& z = 0);

	// Update the position of the camera based on keyboard input
	void Move();

	// Update the position based on the movement of mouse.
	void MoveWithMouse();

	void MoveAroundLockedPosition();

	// Update the position back and forth depending on scroll wheel value.
	void MoveWithScrollWheel();

	// Activate this camera to be the main camera.
	void Activate() const;

	const sm::Vector3& GetPosition() const;

	// Return the matrixes used for the camera.
	const camera_data& GetData() const;

	float& GetSensitivty();
};

class GameCamera
{
public:

	GameCamera();
	~GameCamera();

	void UpdateInfo(Transform* t, CameraPoint* p);

	void Activate() const;

private:

	bool UpdateBuffer();
	bool SetupBuffer();

	camera_data m_matData{};
	ID3D11Buffer* m_buffer;
	sm::Vector3 m_position{};
	sm::Vector3 m_lookAt{};

};