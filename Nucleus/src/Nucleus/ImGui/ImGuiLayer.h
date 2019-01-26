#pragma once

#include "Nucleus/Layer.h"

namespace Nucleus {

	class NUCLEUS_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();

		void OnUpdate();
		void OnEvent(Event& event);

	private:
		float m_Time = 0.0;
	};
}