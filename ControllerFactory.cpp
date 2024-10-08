#include "stdafx.h"
#include "ControllerFactory.h"

#include "Controller.h"
#include "Button.h"
#include "GizmoController.h"
#include "MaterialController.h"
#include "CameraController.h"

unique_ptr<Controller> ControllerFactory::CreateUI(const std::string type)
{
    unique_ptr<Controller> ui = nullptr;
    if (type == "button")
    {
        ui = make_unique<Button>();
    }
    else if(type == "gizmo")
    {
        ui = make_unique<GizmoController>();
    }
    else if (type == "material")
    {
        ui = make_unique<MaterialController>();
    }
    else if (type == "camera")
    {
        ui = make_unique<CameraController>();
    }
    return std::move(ui);
}
