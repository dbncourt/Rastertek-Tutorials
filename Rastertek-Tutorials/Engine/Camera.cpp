////////////////////////////////////////////////////////////////////////////////
// Filename: Camera.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Camera.h"


Camera::Camera()
{
	this->m_position = { 0.0f, 0.0f, 0.0f };
	this->m_rotation = { 0.0f, 0.0f, 0.0f };
}

Camera::Camera(const Camera& other)
{
}

Camera::~Camera()
{
}


void Camera::SetPosition(D3DXVECTOR3 position)
{
	this->m_position = position;
}

D3DXVECTOR3 Camera::GetPosition()
{
	return this->m_position;
}

void Camera::SetRotation(D3DXVECTOR3 rotation)
{
	this->m_rotation = rotation;
}

D3DXVECTOR3 Camera::GetRotation()
{
	return this->m_rotation;
}

void Camera::Render()
{
	D3DXVECTOR3 up;
	D3DXVECTOR3 eye;
	D3DXVECTOR3 lookAt;
	D3DXMATRIX rotationMatrix;

	//Setup the vector that points upwards
	up = { 0.0f, 1.0f, 0.0f };

	// Setup the position of the camera in the world.
	eye = this->m_position;

	// Setup where the camera is looking by default.
	lookAt = { 0.0f, 0.0f, 1.0f };

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	float pitch = this->m_rotation.x * 0.0174532925f;
	float yaw = this->m_rotation.y * 0.0174532925f;
	float roll = this->m_rotation.z * 0.0174532925f;

	//Create the rotation matrix from the yaw, pitch and roll values
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	//Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	//Translate the rotated camera position to the location of the viewer
	lookAt += eye;

	//Finally create the view matrix from the three updated vectors
	D3DXMatrixLookAtLH(&this->m_viewMatrix, &eye, &lookAt, &up);
}

void Camera::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = this->m_viewMatrix;
}