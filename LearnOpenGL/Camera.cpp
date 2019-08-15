#include "Camera.h"
using namespace glm;

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
	Front(glm::vec3(0.f, 0.f, -1.f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	UpdateCameraVectors();
}
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
	Front(vec3(0.f, 0.f, -1.f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = vec3(posX, posY, posZ);
	WorldUp = vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
	UpdateCameraVectors();
}
void Camera::ProcessKeyboard(CameraMove Direction, float DeltaTime)
{
	float Velocity = MovementSpeed * DeltaTime;
	if (Direction == FORWARD)
		Position += Front * Velocity;
	if (Direction == BACKWARD)
		Position -= Front * Velocity;
	if (Direction == LEFT)
		Position -= Right * Velocity;
	if (Direction == RIGHT)
		Position += Right * Velocity;
	//add this to be a true fps camera (no flying you just can look around)
	//Position.y = 0.f;

}
void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	if (constrainPitch)
	{
		if (Pitch > 89.f)
			Pitch = 89.f;
		else if (Pitch < -89.f)
			Pitch = -89.f;
	}
	
	UpdateCameraVectors();
}
void Camera::ProcessMouseScroll(float yoffset)
{
	if (Zoom >= 1.f && Zoom <= 45.f)
		Zoom -= yoffset;
	if (Zoom <= 1.f)
		Zoom = 1.f;
	if (Zoom >= 45.f)
		Zoom = 45.f;
}
void Camera::UpdateCameraVectors()
{
	//calculate the new front vector
	vec3 front;
	front.x = cos(radians(Yaw));//it's multiplied by pitch on the website but I dont's think it should be
	front.y = sin(radians(Pitch));
	front.z = sin(radians(Yaw)) * cos(radians(Pitch));
	Front = normalize(front);
	//recalcualte up and right vectors
	Right = normalize(cross(Front, WorldUp));
	Up = normalize(cross(Right, Front));
}
mat4 Camera::GetViewMatrix()
{
	return lookAt(Position, Position + Front, Up);
}