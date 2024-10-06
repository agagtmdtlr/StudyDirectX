#include "stdafx.h"
#include "Application.h"
#include "Mesh.h"
#include "UIManager.h"

Application::Application(ControllerManager* uiManager)
    :uiManager(uiManager)
{
    sphere.Initialize("sphere", Transform());

}

void Application::Initialize()
{
    // set UI and
}

Mesh* Application::GetMesh()
{
    return &sphere;
}
