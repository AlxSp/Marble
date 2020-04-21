#include "Ball.h"
//#include <cmath>
#include "Marble/ECS/EntityIndex.h"



Ball::Ball(glm::vec3 position, float mass, float rotation, glm::vec4 color)
	: Position(position), m_Color(color), m_Size(glm::vec2(mass, mass)), Mass(mass), m_Rotation(rotation), Velocity(glm::vec3(0.0f))
{
	m_Texture = Marble::Texture2D::Create("assets/textures/forward_circle.png");
}

void Ball::CalculateRotation()
{
	m_Rotation = atan2f(Velocity.y, Velocity.x);
}


void Ball::Collided()
{
	CalculateRotation();
	m_Color = { 1.0f, 0.0f, 0.0f, 1.0f };
}

void Ball::OnUpdate(Marble::TimeStep ts)
{
	if (collided) {
		Collided();
	}
	else
		m_Color = glm::vec4(1.0f);
	//m_Velocity *= 0.99f;
	collided = false;
	//Velocity += Acceleration * ts.GetSeconds();
	Position += glm::vec3{Velocity.x, Velocity.y, 0.0f} * ts.GetSeconds();
	
	if (fabs(Velocity.x * Velocity.x + Velocity.y * Velocity.y) < 0.01f) {
		Velocity = { 0.0f, 0.0f };
	}
	
}

void Ball::OnRender()
{
	Marble::Renderer2D::DrawRotatedQuad(Position, m_Size , m_Rotation, m_Texture, 1.0f, m_Color);
}

