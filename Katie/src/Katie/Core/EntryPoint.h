#pragma once

#ifdef KT_PLATFORM_WINDOWS

extern Katie::Application* Katie::CreateApplication();

int main(int argc, char** argv)
{
	Katie::Log::Init();

	auto app = Katie::CreateApplication();
	app->Run();
	delete app;
}

#endif