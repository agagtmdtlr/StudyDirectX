#pragma once
#include "Controller.h"
#include "Callback.h"

class Button : public Controller
{
public:
	virtual void Render() override final;
	virtual void Action();

};



