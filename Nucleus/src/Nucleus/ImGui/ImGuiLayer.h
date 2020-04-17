#pragma once

#include "Nucleus/Core/Layer.h"
//#include "Nucleus/Events/Event.h"
#include "Nucleus/Events/MouseEvent.h"
#include "Nucleus/Events/KeyEvent.h"
#include "Nucleus/Events/ApplicationEvent.h"

namespace Nucleus {

	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0;
	};
}