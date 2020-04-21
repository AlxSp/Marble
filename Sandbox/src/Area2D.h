#pragma once

#include "Marble.h"


struct LineBorder {
	glm::vec2 start;
	glm::vec2 end;
};

class Area2D
{
public:
	Area2D(float width, float height);

	void SetPosition(float x, float y) {	Position.x = x; Position.y = y;
											topBorder = Position.y + Dimension.y * 0.5f; bottomBorder = Position.y - Dimension.y * 0.5f;
											rightBorder = Position.x + Dimension.x * 0.5f; leftBorder = Position.x - Dimension.x * 0.5f;
										}
	void SetColorRGBA(int red, int green, int blue, float alpha = 1.0f) { Color = { red / 255.0f, green / 255.0f, blue / 255.0f, alpha }; }
	void OnRender() { Marble::Renderer2D::DrawQuad(Position, Dimension, { Color }); };

	float topBorder;
	float bottomBorder;
	float leftBorder;
	float rightBorder;

	glm::vec2 Dimension = { 0.0f, 0.0f };
	glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
	glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
	std::vector<LineBorder> areaBorders;
};
