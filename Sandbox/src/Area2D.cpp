#include "Area2D.h"

Area2D::Area2D(float width, float height)
{
	Dimension = { width , height };

	SetPosition(0, 0);
	// areaBorders.push_back({ {Position.x + Dimension.x * 0.5f, Position.y + Dimension.y * 0.5f}, {Position.x - Dimension.x * 0.5f, Position.y + Dimension.y * 0.5f} });
	// areaBorders.push_back({ {Position.x - Dimension.x * 0.5f, Position.y + Dimension.y * 0.5f}, {Position.x - Dimension.x * 0.5f, Position.y - Dimension.y * 0.5f} });
	// areaBorders.push_back({ {Position.x - Dimension.x * 0.5f, Position.y - Dimension.y * 0.5f}, {Position.x + Dimension.x * 0.5f, Position.y - Dimension.y * 0.5f} });
	// areaBorders.push_back({ {Position.x + Dimension.x * 0.5f, Position.y - Dimension.y * 0.5f}, {Position.x + Dimension.x * 0.5f, Position.y + Dimension.y * 0.5f} });
	
}
