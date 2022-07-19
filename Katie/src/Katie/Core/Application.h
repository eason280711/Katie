#pragma once
#include "Katie/Core/Core.h"
#include "Katie/Event/Event.h"
namespace Katie {
	class KATIE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	Application* CreateApplication();
}

