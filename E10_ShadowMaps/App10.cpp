// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App10.h"

App10::App10()
{
	//BaseApplication::BaseApplication();
	terrainMesh = nullptr;
	cubeMesh = nullptr;
	sphereMesh = nullptr;
	depthShader = nullptr;
	shadowShader = nullptr;
	renderTexture = nullptr;
	light = nullptr;
}

void App10::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

	// Create Mesh object
	terrainMesh = new TerrainMesh(renderer->getDevice(), renderer->getDeviceContext());
	cubeMesh = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());
	sphereMesh = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());

	// Shader handlers
	depthShader = new DepthShader(renderer->getDevice(), hwnd);
	shadowShader = new ShadowShader(renderer->getDevice(), hwnd);

	renderTexture = new RenderTexture(renderer->getDevice(), SHADOWMAP_WIDTH, SHADWOMAP_HEIGHT, SCREEN_NEAR, SCREEN_DEPTH);

	// Light
	initLight();
}

void App10::initLight()
{
	light = new Light;
	light->setAmbientColour(0.15f, 0.15f, 0.15f, 1.0f);
	light->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	light->setLookAt(0.0f, 0.0f, 0.0f);
	light->setPosition(-5.0f, 5.0f, -5.0f);
	light->generateProjectionMatrix(SCREEN_NEAR, SCREEN_DEPTH);
}

App10::~App10()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if (terrainMesh)
	{
		delete terrainMesh;
		terrainMesh = 0;
	}

	if (cubeMesh)
	{
		delete cubeMesh;
		cubeMesh = 0;
	}

	if (sphereMesh)
	{
		delete sphereMesh;
		sphereMesh = 0;
	}

	if (depthShader)
	{
		delete depthShader;
		depthShader = 0;
	}

	if (shadowShader)
	{
		delete shadowShader;
		shadowShader = 0;
	}

	if (renderTexture)
	{
		delete shadowShader;
		shadowShader = 0;
	}

	if (light)
	{
		delete light;
		light = 0;
	}
}

void App10::RenderToTexture()
{
	XMMATRIX worldMatrix, lightViewMatrix, lightProjectionMatrix;

	renderTexture->setRenderTarget(renderer->getDeviceContext());

	renderTexture->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view and projection matrices from the camera and Direct3D objects.
	light->generateViewMatrix();
	worldMatrix = renderer->getWorldMatrix();
	lightViewMatrix = light->getViewMatrix();
	lightProjectionMatrix = light->getProjectionMatrix();
	//light->generateViewMatrix
	//worldMatrix = renderer->getWorldMatrix();
	//viewMatrix = camera->getViewMatrix();
	//projectionMatrix = renderer->getProjectionMatrix();

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Render Sphere
	// Reset the world Matrix
	worldMatrix = renderer->getWorldMatrix();
	// Send geometry data (from mesh)
	sphereMesh->sendData(renderer->getDeviceContext());
	// Set shader parameters (matrices and texture)
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	// Render object (combination of mesh geometry and shader process
	depthShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());

	//// Reset the world Matrix
	//worldMatrix = renderer->getWorldMatrix();
	//// Render Cube
	//// Send geometry data (from mesh)
	//cubeMesh->sendData(renderer->getDeviceContext());
	//// Set shader parameters (matrices and texture)
	//depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	//// Render object (combination of mesh geometry and shader process
	//depthShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());

	// Render floor
	// Reset the world Matrix
	worldMatrix = renderer->getWorldMatrix();
	// Send geometry data (from mesh)
	terrainMesh->sendData(renderer->getDeviceContext());
	// Set shader parameters (matrices and texture)
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	// Render object (combination of mesh geometry and shader process
	depthShader->render(renderer->getDeviceContext(), terrainMesh->getIndexCount());

	renderer->setBackBufferRenderTarget();

	renderer->resetViewport();
}

void App10::RenderScene()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix;

	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	light->generateViewMatrix();
	viewMatrix = camera->getViewMatrix();
	worldMatrix = renderer->getWorldMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	lightViewMatrix = light->getViewMatrix();
	lightProjectionMatrix = light->getProjectionMatrix();

	// Render floor
	// Send geometry data (from mesh)
	terrainMesh->sendData(renderer->getDeviceContext());
	// Set shader parameters (matrices and texture)
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
		textureMgr->getTexture("brick"), renderTexture->getShaderResourceView(), light);
	// Render object (combination of mesh geometry and shader process
	shadowShader->render(renderer->getDeviceContext(), terrainMesh->getIndexCount());

	// Render sphere
	// Send geometry data (from mesh)
	sphereMesh->sendData(renderer->getDeviceContext());
	// Set shader parameters (matrices and texture)
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
		textureMgr->getTexture("brick"), renderTexture->getShaderResourceView(), light);
	// Render object (combination of mesh geometry and shader process
	shadowShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());

	// Render GUI
	gui();

	// Present the rendered scene to the screen.
	renderer->endScene();
}

bool App10::frame()
{
	bool result;
	static float lightPositionX = -5.0f;

	// update the position of the light each frame
	lightPositionX += 0.05f;
	if (lightPositionX > 5.0f) 
	{ 
		lightPositionX = -5.0f;  
	}
	
	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}

	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App10::render()
{
	// render it normally to the texture...
	RenderToTexture();
	// ...then render it again to the back buffer
	RenderScene();

	return true;
}

void App10::gui()
{
	// Force turn off on Geometry shader
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());

	// Render UI
	ImGui::Render();
}
