#include "Application.h"
#include "Katie/Event/ApplicationEvent.h"
#include "Katie/Log/Log.h"
#include "ktpch.h"

namespace Katie {
	Application::Application() {};
	Application::~Application() {};

	void Application::Run()
	{
		KT_CORE_TRACE("{0}","Katie Engine Ver 1.0.0 !");
		while (true) {};
	};
}