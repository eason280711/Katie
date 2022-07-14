#include<Katie.h>

class Sandbox : public Katie::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}

private:

};

Katie::Application* Katie::CreateApplication()
{
	return new Sandbox();
}