#include "GameManager.h"
#include "RenderEngineSWStepped.h"
#include <iostream>
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
GameManager::GameManager()
{
	std::cout << "Attempting to init glfw" << std::endl;
	if (!glfwInit())
	{
		std::cout << "Couldn't init glfw" << std::endl;
		// Initialization failed
		throw;
	}
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//glfwWindowHint(GLFW_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_VERSION_MINOR, 2);;
	//glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
	std::cout << "Loading window" << std::endl;
	//Window_Handle = glfwCreateWindow(1920, 1080, "My Title", glfwGetPrimaryMonitor(), NULL);
	Window_Handle = glfwCreateWindow(800, 800, "My Title",NULL, NULL);
	if (!Window_Handle)
	{
		std::cout << "Window load failed" << std::endl;
		throw;
	}
	std::cout << "Setting up opengl" << std::endl;
	glfwSetFramebufferSizeCallback(Window_Handle, framebuffer_size_callback);
	glfwMakeContextCurrent(Window_Handle);
	glfwSwapInterval(0);
	std::cout << "Init glew" << std::endl;
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW! I'm out!" << std::endl;
		throw;
	}
	std::cout << "Init world" << std::endl;
	//	waterengine = std::make_unique<WaterEngine>();
	world = std::make_unique<World>();
	std::cout << "Init GPU sys" << std::endl;
//	world->waterengine.Init(Window_Handle);
	//world->AddWater(glm::dvec2(100,100));
	std::cout << "Init render engine" << std::endl;
	renderengine = std::make_unique<RenderEngineSWStepped>(Window_Handle);
	int width, height;
	glfwGetFramebufferSize(Window_Handle, &width, &height);
	glViewport(0, 0, width, height);
}


GameManager::~GameManager()
{
	glfwDestroyWindow(Window_Handle);
	glfwTerminate();
}

void GameManager::Run()
{
	std::cout << "Starting game" << std::endl;
	DtCounter = std::chrono::high_resolution_clock::now();
	while (Running)
	{
		Update();
		Render();
		glfwSwapBuffers(Window_Handle);
		glfwPollEvents();
		if (FrameCount++ == 100)
		{
			auto now = std::chrono::high_resolution_clock::now();
			auto dt = now - StartTime;
			StartTime = now;
			std::cout << "Frametime:" << (dt.count() / (100.0 * 1000000000)) << "\n fps:" << ((100.0 * 1000000000) / dt.count()) << "\n";
			//std::cout << "Particle count:" << world->waterengine.ParticleCount << "\n";
			FrameCount = 0;
		}
		auto end = std::chrono::high_resolution_clock::now();
		DeltaTime = (end - DtCounter).count()/1e9;
		DtCounter = end;
	}
}

void GameManager::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderengine->RenderWorld(*world);
	/*renderengine->RotateCounter += renderengine->DRotate * DeltaTime;
	if (renderengine->RotateCounter > 360) {
		renderengine->RotateCounter -= 360;
	}*/
	//world->waterengine.Render();
}
void GameManager::PollInput()
{
	if (glfwGetMouseButton(Window_Handle, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS || glfwGetMouseButton(Window_Handle, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
	}
	else {
	}
	float speed = 30 * DeltaTime;
	if (glfwGetKey(Window_Handle, GLFW_KEY_W)){
		renderengine->cam.tilt += speed;
	}
	if (glfwGetKey(Window_Handle, GLFW_KEY_S)){
		renderengine->cam.tilt -= speed;
	}
	if (glfwGetKey(Window_Handle, GLFW_KEY_D)){
		renderengine->cam.yaw += speed;
	}
	if (glfwGetKey(Window_Handle, GLFW_KEY_A)){
		renderengine->cam.yaw -= speed;
	}
	if (glfwGetKey(Window_Handle, GLFW_KEY_R)){
	}
	if (glfwGetKey(Window_Handle, GLFW_KEY_1)){
	}
	if (glfwGetKey(Window_Handle, GLFW_KEY_2)){
	}
	if (glfwGetKey(Window_Handle, GLFW_KEY_3)){
	}
	if (glfwGetKey(Window_Handle, GLFW_KEY_4)){
	}
	if (glfwGetKey(Window_Handle, GLFW_KEY_5)){
	}

}
void GameManager::Update()
{
	PollInput();

//	renderengine->cam.yaw += DeltaTime * 30;
	//this->waterengine->Update(DeltaTime);
	world->Update(DeltaTime);
	renderengine->cam.Position.x = world->grid.SizeX / 2;
	renderengine->cam.Position.y = world->grid.SizeY / 2;
	if (glfwWindowShouldClose(Window_Handle)) {
		Running = false;
	}
}