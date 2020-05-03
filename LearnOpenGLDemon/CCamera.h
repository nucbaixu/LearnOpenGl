#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. 
// Used as abstraction to stay away from window-system specific input methods

enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:

	Camera(float posX, float posY, float posZ,
		float upX, float upY, float upZ,
		float yaw, float pitch);
		
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f)
		, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)
		, float yaw = YAW
		, float pitch = PITCH);

	void updateCameraVectors();
	void ProcessMouseScroll(float yoffset);
	
	void ProcessMouseMovement(float xoffset, float yoffset
		, GLboolean constrainPitch = true);
	
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	
	glm::mat4 GetViewMatrix();
	
public:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Euler Angles
	float Yaw;
	float Pitch;
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;
};
#endif
