#pragma once

#include "Marble/Renderer/OrthographicCamera.h"
#include "Marble/Core/TimeStep.h"

#include "Marble/Events/ApplicationEvent.h"
#include "Marble/Events/MouseEvent.h"

namespace Marble {

	class OrthographicCameraController {
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);
		

		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		void SetZoomLevel(float level) { m_ZoomLevel = level; m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel); }
		float GetZoomLevel() { return m_ZoomLevel; }
		glm::vec3 GetPosition() { return m_CameraPosition; }
		float GetAspectRatio() { return m_AspectRatio; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f,0.0f, 0.0f };
		float m_CameraRotation = 0.0f;

		float m_CameraTranslationSpeed = 2.5f;
		float m_CameraRotationSpeed = 180.0f;
	};
}