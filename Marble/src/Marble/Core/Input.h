#pragma once

#include "Marble/Core/Core.h"

namespace Marble {

	class Input 
	{
	protected:
		Input() = default;
	public:
		Input(const Input&) = delete;
		Input& operator =(const Input&) = delete;
		
		inline static bool IsKeyPressed(int keycode) { return s_Instance->isKeyPressedImpl(keycode); }

		inline static bool IsMouseButtonPressed(int button) { return s_Instance->isMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
		
		static Scope<Input> Create();
	protected:
		virtual bool isKeyPressedImpl(int keycode) = 0;

		virtual bool  isMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
	private:
		static Scope<Input> s_Instance;
	};

}