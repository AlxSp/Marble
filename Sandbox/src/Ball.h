#pragma once

#include "Marble.h"
/*
auto DoCirclesOverlap = [](float x1, float y1, float r1, float x2, float y2, float r2) {
	return (x1 - x2)* (x1 - x2) + (y1 - y2) * (y1 - y2) <= (r1 + r2) * (r1 + r2);
};
*/

class Ball {

public:
	Ball(glm::vec3 position = glm::vec3(0.0f), float mass = 1.0f, float rotation = 0.0f, glm::vec4 color = glm::vec4(1.0f));
		
	~Ball() = default;
	void SetSize(float size) { m_Size = { size , size }; Radius = size / 2.0f; }
	void SetMass(float mass) { Mass = mass; SetSize(Mass); }

	void SetVelocity(glm::vec2 velocity) { Velocity.x = velocity.x; Velocity.y = velocity.y; CalculateRotation(); }
	void SetAcceleration(glm::vec2 acceleration) { Acceleration.x = acceleration.x; Acceleration.y = acceleration.y; CalculateRotation();}
	void CalculateRotation();

	void Collided();

	void OnUpdate(Marble::TimeStep ts);
	void OnRender();

	Marble::Ref<Marble::Texture2D> m_Texture;

	bool collided = false;

	int id;

	float m_Rotation; 
	glm::vec2 m_Size;
	glm::vec4 m_Color;

	float Mass;
	float Radius;
	glm::vec3 Position     = { 0.0f, 0.0f, 0.0f };
	glm::vec2 Velocity     = { 0.0f, 0.0f };
	glm::vec2 Acceleration = { 0.0f, 0.0f };
};