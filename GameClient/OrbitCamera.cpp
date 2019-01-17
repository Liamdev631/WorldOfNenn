#include "OrbitCamera.h"
//#include <glm/gtc/matrix_transform.hpp>

OrbitCamera::OrbitCamera()
	: m_rotation(0), m_pitch(0), m_position(0, 0, 0), m_cameraDistance(DefaultCameraDistance)
{

}

OrbitCamera::~OrbitCamera()
{

}

void OrbitCamera::setPosition(const glm::fvec2& position)
{
	m_position.x = position.x;
	m_position.t = position.y;
}

void OrbitCamera::setPosition(const vec2f& position)
{
	m_position.x = position.x;
	m_position.y = position.y;
}

void OrbitCamera::setHeight(const float& height)
{
	m_position.z = height;
}

void OrbitCamera::setRotation(const float& angle)
{
	m_rotation = angle;
}

void OrbitCamera::addRotation(const float& angle)
{
	m_rotation += angle;
}

void OrbitCamera::setPitch(const float& angle)
{
	m_pitch = angle;
}

void OrbitCamera::setYaw(const float& angle)
{
	m_pitch += angle;
}

glm::mat4 OrbitCamera::getView() const
{
	glm::fvec3 eye = glm::fvec3(glm::cos(m_rotation), glm::sin(m_rotation), 0);
	eye *= glm::cos(m_pitch);
	eye.z = glm::sin(m_pitch);
	glm::normalize(eye);
	eye *= m_cameraDistance;
	glm::fvec3 up(0, 0, 1);
	return glm::lookAt(eye, m_position, up);
}

glm::mat4 OrbitCamera::getProjection() const
{
	return m_projectionMatrix;
}
