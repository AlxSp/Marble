#pragma once

namespace Nucleus {

	class GraphicsContext {
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};

}