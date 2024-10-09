#pragma once
#include "Callback.h"
struct Event
{

};

class Controller
{
public:
	Controller() {}

	virtual ~Controller() {}
	virtual void Render() = 0;

	std::string label;
	std::unique_ptr<Callback> callback;

	std::any model;

};

