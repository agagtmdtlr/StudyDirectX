#pragma once
#include "Application.h"
#include "Level.h"
class Editor :
    public Application
{
public:
    Editor(ControllerManager* uiManager);
    virtual void InitializeApplication() override;
    virtual void UpdateApplication() override;


    unique_ptr<class Level> level;
};

