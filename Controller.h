#pragma once
#include "Callback.h"
class Controller
{
public:
	Controller() {}

	virtual ~Controller() {}

	virtual void Render() = 0;

	std::string label;
	unique_ptr<Callback> callback;
};

