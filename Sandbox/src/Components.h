#pragma once 

// Marble Components
struct Renderable {};
//

struct Position {
	float x;
	float y;
	float z;
};

struct Size2D {
	float x = 1.0f;
	float y = 1.0f;
};

struct ColorRGBA {
	float r;
	float g;
	float b;
	float a = 1.0f;
};

