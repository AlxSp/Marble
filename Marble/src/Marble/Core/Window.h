#pragma once

#include "mblpch.h"

#include "Marble/Core/Core.h"
#include "Marble/Events/Event.h"

namespace Marble {
	// Struct holding properties for desktop system based window
	struct WindowProps {
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Marble Engine",
					unsigned int width = 1920,
					unsigned int height = 1080)
			: Title(title), Width(width), Height(height) {}
	}; 

	// Interface representing a desktop system based window
	class Window{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual unsigned int GetXPosition() const = 0;
		virtual unsigned int GetYPosition() const = 0;

		//Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}