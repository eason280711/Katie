#include "Application.h"
#include "Katie/Event/ApplicationEvent.h"
#include "Katie/Log/Log.h"
#include "ktpch.h"
#include<vsg/all.h>
#include <vsgXchange/all.h>
#include"RenderImGui.h"
#include"Katie/RunTimeGUI/GUI.h"
#include"config_app.h"

namespace Katie {
	Application::Application() {};
	Application::~Application() {};

	void Application::Run()
	{
		KT_CORE_TRACE("{0}","Katie Engine Ver 1.0.0 !");
		// set up defaults and read command line arguments to override them
		auto options = vsg::Options::create();
		options->sharedObjects = vsg::SharedObjects::create();
		options->fileCache = vsg::getEnv("VSG_FILE_CACHE");

		options->add(vsgXchange::all::create());

		auto windowTraits = vsg::WindowTraits::create();
		windowTraits->windowTitle = "Katie Engine";

		//load model
		auto vsg_scene = vsg::Group::create();
		vsg::ref_ptr<vsg::EllipsoidModel> ellipsoidModel;
		vsg::Path filename = std::string(ASSET_ROOT) + "models/tkg.fbx";
		auto node = vsg::read_cast<vsg::Node>(filename, options);
		vsg_scene->addChild(node);

		// create the viewer and assign window(s) to it
		auto viewer = vsg::Viewer::create();

		vsg::ref_ptr<vsg::Window> window(vsg::Window::create(windowTraits));
		if (!window)
		{
			std::cout << "Could not create windows." << std::endl;
			return;
		}
		viewer->addWindow(window);

		// compute the bounds of the scene graph to help position camera
		vsg::ComputeBounds computeBounds;
		vsg_scene->accept(computeBounds);
		vsg::dvec3 centre = (computeBounds.bounds.min + computeBounds.bounds.max) * 0.5;
		double radius = vsg::length(computeBounds.bounds.max - computeBounds.bounds.min) * 0.6;

		// These are set statically because the geometry in the class is expanded in the shader
		double nearFarRatio = 0.01;

		// set up the camera
		auto lookAt = vsg::LookAt::create(centre + vsg::dvec3(0.0, -radius * 3.5, 0.0), centre, vsg::dvec3(0.0, 0.0, 1.0));

		vsg::ref_ptr<vsg::ProjectionMatrix> perspective;
		if (ellipsoidModel)
		{
			perspective = vsg::EllipsoidPerspective::create(lookAt, ellipsoidModel, 30.0, static_cast<double>(window->extent2D().width) / static_cast<double>(window->extent2D().height), nearFarRatio, 0.0);
		}
		else
		{
			perspective = vsg::Perspective::create(30.0, static_cast<double>(window->extent2D().width) / static_cast<double>(window->extent2D().height), nearFarRatio * radius, radius * 400.5);
		}

		auto camera = vsg::Camera::create(perspective, lookAt, vsg::ViewportState::create(window->extent2D()));

		// The commandGraph will contain a 2 stage renderGraph 1) 3D scene 2) ImGui (by default also includes clear depth buffers)
		auto commandGraph = vsg::CommandGraph::create(window);
		auto renderGraph = vsg::RenderGraph::create(window);
		commandGraph->addChild(renderGraph);

		// create the normal 3D view of the scene
		renderGraph->addChild(vsg::View::create(camera, vsg_scene));
		renderGraph->addChild(vsgImGui::RenderImGui::create(window,Katie::getGUI()));

		// add close handler to respond the close window button and pressing escape
		viewer->addEventHandler(vsg::CloseHandler::create(viewer));

		viewer->addEventHandler(vsg::Trackball::create(camera, ellipsoidModel));

		viewer->assignRecordAndSubmitTaskAndPresentation({ commandGraph });

		viewer->compile();

		while (viewer->advanceToNextFrame())
		{

			viewer->handleEvents();

			viewer->update();

			viewer->recordAndSubmit();

			viewer->present();
		}
	};
}