#pragma once
#include "Example.h"
#include "GeometryShader.h"

class GeometryExample : public Example
{
public:
	GeometryExample();
	~GeometryExample();

	void init(D3D* renderer, HWND hwnd);
	void initShader(D3D* renderer, HWND hwnd);
	//void initLight();
	void render(D3D* renderer, Camera* camera, TextureManager* textureMgr);

	// shader handler
	MESH mesh_choice = MESH::CUBE;
	GeometryShader* shader;
	BaseMesh* mesh;
	bool d3d11_primitive_topology_trianglelist, d3d11_primitive_topology_pointlist;
};
