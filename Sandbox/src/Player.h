#pragma once

#include "Nucleus.h"

class Player {
public: 
	Player(const glm::vec3& position, const float& view, const float& zoomLevel)
		: CameraPosition(position), CameraView(view, 1), ZoomLevel(zoomLevel) {};
	~Player() = default;

	void SetCameraPosition(glm::vec3 position) { CameraPosition = position; }
	glm::vec2 GetCameraPosition() { return CameraPosition; }

	void SetZoomLevel(const float& zoomLevel) { ZoomLevel = zoomLevel; }
	float GetZoomLevel() { return ZoomLevel; }

	void SetCameraView(glm::vec2 view) { CameraView = view; }
	glm::vec2 GetCameraView() { return CameraView * ZoomLevel * 2.0f; }

private:
	glm::vec2 CameraPosition;
	glm::vec2 CameraView;
	float ZoomLevel;
};