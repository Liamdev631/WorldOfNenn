#pragma once
#include "Global.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

constexpr float DefaultCameraDistance = 12.f;

class OrbitCamera
{
private:
	float m_rotation, m_pitch, m_cameraDistance;
	glm::fvec3 m_position;
	glm::mat4 m_projectionMatrix = glm::perspective(
		glm::radians(90.f), // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
		4.0f / 3.0f,       // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
		0.1f,              // Near clipping plane. Keep as big as possible, or you'll get precision issues.
		256.0f             // Far clipping plane. Keep as little as possible.
	);

public:
	OrbitCamera();
	~OrbitCamera();

	void setPosition(const glm::fvec2& position);
	void setPosition(const vec2f& position);
	void setHeight(const float& height);
	void setRotation(const float& angle);
	void addRotation(const float& angle);
	void setPitch(const float& angle);
	void setYaw(const float& angle);

	glm::mat4 getView() const;
	glm::mat4 getProjection() const;
};

