#pragma once

#include "Nucleus/Core/Core.h"

namespace Nucleus {

	class NUCLEUS_API Input {
	public:
		inline static bool IsKeyPressed(int keycode) { return s_Instance->isKeyPressedImpl(keycode); }

		inline static bool IsMouseButtonPressed(int button) { return s_Instance->isMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
		

	protected:
		virtual bool isKeyPressedImpl(int keycode) = 0;

		virtual bool  isMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
	private:
		static Input* s_Instance;
	};

}