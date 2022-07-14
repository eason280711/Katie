#pragma once
#include "Core.h"
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

