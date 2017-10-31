// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "E5_TerrainApp.h"

E5_TerrainApp::E5_TerrainApp()
{
	//BaseApplication::BaseApplication();
	triangleMesh = nullptr;
	sphereMesh = nullptr;
	cubeMesh = nullptr;
	quadMesh = nullptr;
	planeMesh = nullptr;
	terrainMesh = nullptr;
	lightShader = nullptr;
	colourShader = nullptr;
	wireframe = false;
}

void E5_TerrainApp::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

	textureMgr->loadTexture("height", L"../res/height.png");
	textureMgr->loadTexture("brick", L"../res/brick.dds");

	// Create Mesh object
	//triangleMesh = new TriangleMesh(renderer->getDevice(), renderer->getDeviceContext());

	sphereMesh = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext(), 20);

	//cubeMesh = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext(), 100);

	//quadMesh = new QuadMesh(renderer->getDevice(), renderer->getDeviceContext());

	planeMesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());

	terrainMesh = new TerrainMesh(renderer->getDevice(), renderer->getDeviceContext(), 100, 200);

	//colourShader = new ColourShader(renderer->getDevice(), hwnd);

	lightShader = new LightShader(renderer->getDevice(), hwnd);

	initLight();
}

void E5_TerrainApp::initLight()
{
	m_Light = new Light;
	m_Light->setAmbientColour(0.5f, 0.5f, 0.5f, 1.0f);
	m_Light->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->setDirection(0.0, 0.0f, 0.0f);
	m_Light->setSpecularPower(16.f);
	m_Light->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->setPosition(0.0f, 0.1f, 0.0f);
	light_y = 0.0f;
}

E5_TerrainApp::~E5_TerrainApp()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if (triangleMesh)
	{
		delete triangleMesh;
		triangleMesh = 0;
	}

	if (sphereMesh)
	{
		delete sphereMesh;
		sphereMesh = 0;
	}

	if (cubeMesh)
	{
		delete cubeMesh;
		cubeMesh = 0;
	}

	if (quadMesh)
	{
		delete quadMesh;
		quadMesh = 0;
	}

	if (planeMesh)
	{
		delete planeMesh;
		planeMesh = 0;
	}

	if (terrainMesh)
	{
		delete terrainMesh;
		terrainMesh = 0;
	}

	if (colourShader)
	{
		delete colourShader;
		colourShader = 0;
	}

	if (lightShader)
	{
		delete lightShader;
		lightShader = 0;
	}
}


bool E5_TerrainApp::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}

	if (input->isLeftMouseDown())
	{
		wireframe = !wireframe;
		input->setLeftMouse(false);
	}

	if (input->isKeyDown(VK_RETURN))
	{
		wireframe = !wireframe;
		input->SetKeyUp(VK_RETURN);
	}

	/*
	static float a = 0.f;

	a += XM_PIDIV2 * timer->getTime();
	a = fmodf(a, XM_2PI);

	m_Light->setPosition(0.f, 100 + (100.f * sinf(a)), 0.f);
	*/

	light_y += XM_PIDIV2 * timer->getTime();
	light_y = fmodf(light_y, XM_2PI);

	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool E5_TerrainApp::render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	//// Clear the scene. (default cornflower blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	//// Generate the view matrix based on the camera's position.
	camera->update();

	//// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	viewMatrix = camera->getViewMatrix();

	projectionMatrix = renderer->getProjectionMatrix();

	// wireframe mode
	renderer->setWireframeMode(wireframe);

	// translation and rotation
	worldMatrix = renderer->getWorldMatrix();
	XMMATRIX matrixTranslation = XMMatrixTranslation(0.0f, 0.0, 0.0f);
	XMMATRIX matrixRotation = XMMatrixRotationX(XMConvertToRadians(180.0f));
	worldMatrix = XMMatrixMultiply(matrixRotation, matrixTranslation);
	// scaling
	XMMATRIX matrixScaling = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	worldMatrix *= matrixScaling;

	// wave's:
	float height = 1.0f;
	float frequency = 1.0f;

	//m_Light->setPosition(0.0f, sinf(light_y * 3.0f), 0.0f);
	//// Send geometry data (from mesh)
	//triangleMesh->sendData(renderer->getDeviceContext());
	//sphereMesh->sendData(renderer->getDeviceContext());
	//cubeMesh->sendData(renderer->getDeviceContext());
	//quadMesh->sendData(renderer->getDeviceContext()); // set input data in the shader programme
	//planeMesh->sendData(renderer->getDeviceContext()); // set input data in the shader programme
	terrainMesh->sendData(renderer->getDeviceContext());

	//// Set shader parameters (matrices and texture)
	//lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("default"), m_Light);
	lightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("height"), m_Light, light_y, height, frequency);

	//// Render object (combination of mesh geometry and shader process
	//lightShader->render(renderer->getDeviceContext(), triangleMesh->getIndexCount()); // output data from the shader programme
	//lightShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount()); // output data from the shader programme
	//lightShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount()); // output data from the shader programme
	//lightShader->render(renderer->getDeviceContext(), quadMesh->getIndexCount()); // output data from the shader programme
	//lightShader->render(renderer->getDeviceContext(), planeMesh->getIndexCount()); // output data from the shader programme
	lightShader->render(renderer->getDeviceContext(), terrainMesh->getIndexCount());

	// Render GUI
	gui();

	//// Present the rendered scene to the screen.
	renderer->endScene();

	return true;
}

void E5_TerrainApp::gui()
{
	// Force turn off on Geometry shader
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());

	// Render UI
	ImGui::Render();
}
