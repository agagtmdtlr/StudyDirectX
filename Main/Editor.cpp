#include "stdafx.h"
#include "Editor.h"
#include "Level.h"
#include "Mesh.h"
#include "Sphere.h"
#include "StaticMesh.h"
Editor::Editor(ControllerManager* uiManager)
	:Application(uiManager)
{
	level = make_unique<Level>();
}

void CreateSponza(Editor* editor)
{
	shared_ptr<StaticMesh> mesh = make_shared<StaticMesh>(StaticMesh());
	mesh->LoadMesh("sponza.asset");
	Transform transform;
	transform.position.y = -1.0;
	transform.rotation.y = 180;

	mesh->Initialize(transform);
	editor->level->AddMesh(mesh);
}

void CreateCube(Editor* editor)
{
	//shared_ptr<Cube> cube = make_shared<Cube>();
	//cube->LoadMesh("");
	Transform t;
	t.position.x = 2.f;
	t.rotation.y = 45.f;
	t.rotation.x = 45.f;


	shared_ptr<StaticMesh> mesh = make_shared<StaticMesh>(StaticMesh());
	mesh->LoadMesh("cube.asset");
	mesh->Initialize(t);
	editor->level->AddMesh(mesh);
}


void CreateSphere(Editor* editor)
{
	shared_ptr<Sphere> sphere = make_shared<Sphere>();
	sphere->LoadMesh("");
	Transform t;
	t.position.x = -2.f;
	sphere->Initialize(t);
	editor->level->AddMesh(sphere);
}
void CreateBunny(Editor* editor)
{
	shared_ptr<StaticMesh> mesh = make_shared<StaticMesh>(StaticMesh());
	mesh->LoadMesh("bunny.asset");
	Transform transform;
	transform.position.y = -1.0;
	transform.rotation.y = 180;

	mesh->Initialize(transform);
	editor->level->AddMesh(mesh);
}



void Editor::InitializeApplication()
{
	level->LoadLevel("");
	CreateSponza(this);
	//CreateBunny(this);
	//CreateCube(this);
	//CreateSphere(this);

	
}

void Editor::UpdateApplication()
{
	level->UpdateScene();
}
