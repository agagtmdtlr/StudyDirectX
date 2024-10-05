#pragma once
#include "UI.h"
#include "Callback.h"

class UIButton : public UI
{
public:
	virtual void Render() override final;
	virtual void Action();

};



