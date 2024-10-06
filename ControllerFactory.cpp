#include "stdafx.h"
#include "ControllerFactory.h"

#include "Controller.h"
#include "ButtonController.h"
#include "GizmoController.h"
#include "MaterialController.h"

unique_ptr<Controller> ControllerFactory::CreateUI(const std::string type)
{
    unique_ptr<Controller> ui = nullptr;
    if (type == "button")
    {
        ui = make_unique<ButtonController>();
    }
    else if(type == "gizmo")
    {
        ui = make_unique<GizmoController>();
    }
    else if (type == "material")
    {
        ui = make_unique<MaterialController>();
    }
    return std::move(ui);
}
