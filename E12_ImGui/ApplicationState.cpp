// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "ApplicationState.h"

ApplicationState::ApplicationState()
{
	graphicsApp = nullptr;
}

void ApplicationState::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

	// Create mesh object (triangle), initialise basic colour shader and set colour variable for UI controls.
	graphicsApp = new GraphicsApp();
}


ApplicationState::~ApplicationState()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if (graphicsApp)
	{
		delete graphicsApp;
		graphicsApp = 0;
	}
}


bool ApplicationState::frame()
{
	bool result;

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

//void ApplicationState::triangleColourShader()
//{
//	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
//
//	//// Clear the scene. (default blue colour)
//	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);
//
//	//// Generate the view matrix based on the camera's position.
//	camera->update();
//
//	//// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
//	worldMatrix = renderer->getWorldMatrix();
//	viewMatrix = camera->getViewMatrix();
//	projectionMatrix = renderer->getProjectionMatrix();
//
//	renderer->setWireframeMode(isWireframe);
//
//	//// Send geometry data (from mesh)
//	mesh->sendData(renderer->getDeviceContext());
//	//// Set shader parameters (matrices and texture)
//	colourShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, clear_col);
//	//// Render object (combination of mesh geometry and shader process
//	colourShader->render(renderer->getDeviceContext(), mesh->getIndexCount());
//
//	// Render GUI
//	trianglegui();
//
//	//// Present the rendered scene to the screen.
//	renderer->endScene();
//}
//
//void ApplicationState::trianglegui()
//{
//	// Force turn off on Geometry shader and force fill rendering
//	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
//	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
//	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);
//	renderer->setWireframeMode(false);
//
//	// Build UI
//	ImGui::Text("FPS: %.2f", timer->getFPS());
//	ImGui::ColorEdit3("Colour", (float*)&clear_col);
//	ImGui::Checkbox("Wireframe", &isWireframe);
//	ImGui::Checkbox("Triangle Colour Shader", &isTriangleColourShader);
//
//	// Render UI
//	ImGui::Render();
//}

bool ApplicationState::render()
{
	graphicsApp->GraphicsApp::render();

	return true;
}

//void ApplicationState::gui()
//{
//	// Force turn off on Geometry shader and force fill rendering
//	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
//	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
//	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);
//	renderer->setWireframeMode(false);
//
//	// Build UI
//	ImGui::Text("FPS: %.2f", timer->getFPS());
//	ImGui::Checkbox("Triangle Colour Shader", &isTriangleColourShader);
//
//	// Render UI
//	ImGui::Render();
//}

