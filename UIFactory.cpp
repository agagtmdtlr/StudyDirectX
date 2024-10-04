#include "stdafx.h"
#include "UIFactory.h"
#include "UI.h"
#include "UIButton.h"
#include "UIGizmo.h"
#include "UIMaterial.h"

unique_ptr<UI> UIFactory::CreateUI(const std::string type)
{
    unique_ptr<UI> ui = nullptr;
    if (type == "button")
    {
        ui = make_unique<UIButton>();
    }
    else if(type == "gizmo")
    {
        ui = make_unique<UIGizmo>();
    }
    else if (type == "material")
    {
        ui = make_unique<UIMaterial>();
    }
    return ui;
}
