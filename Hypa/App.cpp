#include "App.h"

namespace Hypa {
	App::App() {
		Events = std::make_shared<EventSystem>();
		window = std::make_shared<Window>(Events);
		rAPIsystem = std::make_shared<RenderingAPISystem>();
		VulkanAPI = std::make_shared<Vulkan>(window, Events);
		OpenGLAPI = std::make_shared<OpenGL>(window, Events);
		rAPIsystem->AddAPI(VulkanAPI);
		rAPIsystem->AddAPI(OpenGLAPI);
		rAPIsystem->SwitchRenderingAPI("OpenGL");
		Layerdispatch = std::make_shared<LayerDispatch>();
		Rendering3d = std::make_shared<Rendering3D>(window, rAPIsystem);
		Layerdispatch->AddLayer(Rendering3d);

		Layerdispatch->DispatchLayerAttach();
	}

	bool App::Update() {
		Events->ResetEvents();
		window->Update();
		Events->AddEvent(std::make_shared<AppTickEvent>());
		Events->AddEvent(std::make_shared<AppUpdateEvent>());
		Events->DispatchEvent();
		window->ProcessEvents();
		Layerdispatch->DispatchLayerRender();
		Events->AddEvent(std::make_shared<AppRenderEvent>());
		return !window->shouldClose();
	}

	std::shared_ptr<Window> App::GetWindow() {
		return window;
	}

	std::shared_ptr<EventSystem> App::GetEventSystem() {
		return Events;
	}

	std::shared_ptr<RenderingAPISystem> App::GetRenderingAPISystem() {
		return rAPIsystem;
	}

	std::shared_ptr<LayerDispatch> App::GetLayerDispatch() {
		return Layerdispatch;
	}

	std::shared_ptr<OpenGL> App::GetOpenGL() {
		return OpenGLAPI;
	}

	std::shared_ptr<Vulkan> App::GetVulkan() {
		return VulkanAPI;
	}

	std::shared_ptr<Rendering3D> App::GetRendering3D() {
		return Rendering3d;
	}
}