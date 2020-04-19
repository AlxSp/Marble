#pragma once

#include "Marble/Core/Layer.h"
//#include "Marble/Events/Event.h"
#include "Marble/Events/MouseEvent.h"
#include "Marble/Events/KeyEvent.h"
#include "Marble/Events/ApplicationEvent.h"

namespace Marble {

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