// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "RenderToTextureExample.h"

RenderToTextureExample::RenderToTextureExample()
{
	lightShader = nullptr;
	textureShader = nullptr;

	light = nullptr;

	renderTexture = nullptr;
	orthoMesh = nullptr;
}

RenderToTextureExample::~RenderToTextureExample()
{
	if (lightShader)
	{
		delete lightShader;
		lightShader = 0;
	}

	if (textureShader)
	{
		delete textureShader;
		textureShader = 0;
	}

	if (light)
	{
		delete light;
		light = 0;
	}

	if (renderTexture)
	{
		delete renderTexture;
		renderTexture = 0;
	}

	if (orthoMesh)
	{
		delete orthoMesh;
		orthoMesh = 0;
	}
}

void RenderToTextureExample::init(D3D * renderer, HWND hwnd)
{
	initShader(renderer, hwnd);
	initVariables();
	initLight();
}

void RenderToTextureExample::initShader(D3D * renderer, HWND hwnd)
{
	lightShader = new LightShader(renderer->getDevice(), hwnd);

	textureShader = new TextureShader(renderer->getDevice(), hwnd);

	// Create light source (for normal scene rendering)
	initLight();

	// RenderTexture, OrthoMesh and shader set for different renderTarget
	//renderTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	renderTexture = new RenderTexture(renderer->getDevice(), 800, 600, 0.1f, 200.0f);

	// ortho size and position set based on window size
	// 200x200 pixels (standard would be matching window size for fullscreen mesh
	// Position default at 0x0 centre window, to offset change values (pixel)
	orthoMesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), 200, 150, -412, 225);
}

void RenderToTextureExample::initVariables()
{
}

void RenderToTextureExample::initLight()
{
	light = new Light;
	light->setAmbientColour(0.5f, 0.5f, 0.5f, 1.0f);
	light->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	light->setDirection(0.0, 0.0f, 0.0f);
	light->setSpecularPower(16.f);
	light->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	light->setPosition(0.0f, 0.1f, 0.0f);
	light_y = 0.0f;
}

void RenderToTextureExample::renderToTexture(D3D* renderer, Camera* camera, TextureManager* textureMgr)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Set the render target to be the render to texture.
	renderTexture->setRenderTarget(renderer->getDeviceContext());

	// Clear the render to texture.
	renderTexture->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 1.0f, 1.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	// Set primitive topology
	D3D_PRIMITIVE_TOPOLOGY d3d11_primitive_topology;
	if (d3d11_primitive_topology_trianglelist_) d3d11_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	else d3d11_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	mesh_->sendData(renderer->getDeviceContext(), d3d11_primitive_topology);
	lightShader->setShaderParameters
	(
		renderer->getDeviceContext(),
		worldMatrix, viewMatrix, projectionMatrix,
		textureMgr->getTexture("default"),
		light,
		over_time_
	);
	// Render object (combination of mesh geometry and shader process
	lightShader->render(renderer->getDeviceContext(), mesh_->getIndexCount());

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();
}

void RenderToTextureExample::renderScene(D3D* renderer, Camera* camera, TextureManager* textureMgr)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoViewMatrix, orthoMatrix;

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	// wireframe mode
	renderer->setWireframeMode(wireframe_);

	// Set primitive topology
	D3D_PRIMITIVE_TOPOLOGY d3d11_primitive_topology;
	if (d3d11_primitive_topology_trianglelist_) d3d11_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	else d3d11_primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

	// Send geometry data (from mesh)
	mesh_->sendData(renderer->getDeviceContext(), d3d11_primitive_topology);
	// Set shader parameters (matrices and texture)
	lightShader->setShaderParameters
	(
		renderer->getDeviceContext(), 
		worldMatrix, viewMatrix, projectionMatrix, 
		textureMgr->getTexture("default"), 
		light, 
		over_time_
	);
	// Render object (combination of mesh geometry and shader process
	lightShader->render(renderer->getDeviceContext(), mesh_->getIndexCount());

	// Render to ortho mesh
	// Turn off the Z buffer to begin all 2D rendering. //////////////////////////
	renderer->setZBuffer(false);
	// ortho matrix for 2D rendering
	orthoMatrix = renderer->getOrthoMatrix();
	orthoViewMatrix = camera->getOrthoViewMatrix();

	// Set primitive topology
	orthoMesh->sendData(renderer->getDeviceContext(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix,
		renderTexture->getShaderResourceView()
	);
	textureShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());
	// Enable Z buffering after rendering //////////////////////////////////////////
	renderer->setZBuffer(true);
}

void RenderToTextureExample::render(D3D* renderer, Camera* camera, TextureManager* textureMgr)
{
	// render it normally to the texture...
	renderToTexture(renderer, camera, textureMgr);
	// ...then render it again to the back buffer
	renderScene(renderer, camera, textureMgr);
}

void RenderToTextureExample::gui(Camera * camera)
{
	// render only if the example is active
	if (example_)
	{
		ImGui::Begin("Geometry Shader Example", &example_);
		if (ImGui::Button("Reset Example"))
		{
			resetExample(camera);
		}
		ImGui::Checkbox("Wireframe", &wireframe_);
		ImGui::SliderFloat3("Scale", (float*)&scale_, -10.0f, 10.0f);
		if (ImGui::Button("Reset Scale")) scale_ = XMFLOAT3(1.0f, 1.0f, 1.0f);
		if (ImGui::Checkbox("Primitive Topology Trianglelist", &d3d11_primitive_topology_trianglelist_))
			d3d11_primitive_topology_pointlist_ = false;
		if (ImGui::Checkbox("Primitive Topology Pointlist", &d3d11_primitive_topology_pointlist_))
			d3d11_primitive_topology_trianglelist_ = false;

		// what mesh to render
		if (ImGui::Checkbox("Triangle Mesh", &triangle_mesh_))
		{
			set_mesh_choice(MESH_CHOICE::TRIANGLE);
		}
		if (ImGui::Checkbox("Sphere Mesh", &sphere_mesh_))
		{
			set_mesh_choice(MESH_CHOICE::SPHERE);
		}
		if (ImGui::Checkbox("Cube Mesh", &cube_mesh_))
		{
			set_mesh_choice(MESH_CHOICE::CUBE);
		}
		if (ImGui::Checkbox("Quad Mesh", &quad_mesh_))
		{
			set_mesh_choice(MESH_CHOICE::QUAD);
		}
		if (ImGui::Checkbox("Plane Mesh", &plane_mesh_))
		{
			set_mesh_choice(MESH_CHOICE::PLANE);
		}

		ImGui::End();
	}
}

void RenderToTextureExample::resetExample(Camera * camera)
{
	// set cube mesh
	set_mesh_choice(MESH_CHOICE::CUBE);
	// reset geometry shader scale_
	scale_ = XMFLOAT3(1.0f, 1.0f, 1.0f);
	// reset geometry shader wireframe mode
	wireframe_ = false;
	// reset geometry shader primitive topology
	d3d11_primitive_topology_trianglelist_ = true;
	d3d11_primitive_topology_pointlist_ = false;
	// set the camera
	camera->setPosition(13.0f, 4.0f, -22.0f);
	camera->setRotation(0.0f, -30.0f, 7.0f);
}
