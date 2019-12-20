#pragma once

#include "Nucleus/Core/Input.h"

namespace Nucleus {

	class WindowsInput : public Input {
	protected:
		virtual bool isKeyPressedImpl(int keycode) override;

		virtual bool  isMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;

	};

}