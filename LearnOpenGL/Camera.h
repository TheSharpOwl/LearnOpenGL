#pragma once

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

#include<vector>

//Default Camera Values
/*
* Yaw is initialized to -90.0 degrees because zero results in a direction vector pointing
* to the right so we initially rotate a little bit to the left
*/
const float YAW = -90.f;
const float PITCH = 0.f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.f;

enum CameraMove
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};
class Camera
{
public:
	//camera attributes
	glm::vec3 Position, Front, Up, Right, WorldUp;
	//euler angles
	float Yaw, Pitch;
	//Camera options
	float MovementSpeed, MouseSensitivity, Zoom;

	//constructor
	Camera(glm::vec3 position = glm::vec3(0.f,0.f,0.f), glm::vec3 up = glm::vec3(0.f, 1.f, 0.f), float yaw = YAW, float pitch = PITCH);
	//contructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	glm::mat4 GetViewMatrix();

	void ProcessKeyboard(CameraMove Direction, float DeltaTime);

	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

	//process the input recieved from mouse scroll-wheel events, we only need vertical axis
	void ProcessMouseScroll(float yoffset);
private:

	void UpdateCameraVectors();
};

