#pragma once

#include "Nucleus/Core/Layer.h"
//#include "Nucleus/Events/Event.h"
#include "Nucleus/Events/MouseEvent.h"
#include "Nucleus/Events/KeyEvent.h"
#include "Nucleus/Events/ApplicationEvent.h"

namespace Nucleus {

	class NUCLEUS_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0;
	};
}